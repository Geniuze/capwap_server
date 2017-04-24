#include <iostream>
using namespace std;

#include <list>
#include <string>
#include <vector>

extern "C" {
#include "libubus.h"
#include "libubox/blobmsg.h"
#include "libubox/blobmsg_json.h"
#include "libubox/ustream.h"
}

#include "log.h"
#include "dbi.h"
#include "dstring.h"
#include "business.h"
#include "capwap.h"
#include "ap_list.h"
#include "server.h"
#include "interface.h"
#include "ubus.h"
#include "config_manager.h"

list<CBusiness*> businesses;
struct uloop_timeout business_timeout;

// 更新AP的状态到数据库
static int update_station_state(struct ap_dev *ap)
{
    string key_value, cond;
    key_value.append(STRING_STATE"=" + toString(ap->state));
    cond.append(STRING_AP_MAC"='" + toString(ap->hw_addr) + "'");

    DBI::Update(TO_STR(AP_LIST), key_value.c_str(), cond.c_str());

    return 0;
}

CBusiness *create_business()
{
    try
    {
        return new CBusiness;
    }
    catch(...)
    {
        dlog(LOG_ERR, "%s.%d alloc memroy error.", __FUNC__, __LINE__);
        return NULL;
    }
    return NULL;
}

CBusiness::CBusiness()
{
    ap = NULL;
    business_type = CAPWAP_BUSINESS_MAX;
    src_info = "";
    src_packet = NULL;
}

CBusiness::~CBusiness()
{
    if (src_packet)
    {
        SAFE_DELETE(src_packet);
    }
}

int CBusiness::Process()
{
    int ret = BUSINESS_FAIL;


    if (NULL == ap)
    {
        dlog(LOG_ERR, "%s.%d {ap dev is null type %d}",
             __FUNC__, __LINE__, business_type);
        return ret;
    }

    switch(business_type)
    {
    case CAPWAP_BUSINESS_DISCOVERY:
        ret = business_discovery_process();
        break;
    case CAPWAP_BUSINESS_JOIN:
        ret = business_join_process();
        break;
    case CAPWAP_BUSINESS_CONFIGURE:
        ret = business_configure_process();
        break;
    case CAPWAP_BUSINESS_DATA_CHECK:
        ret = business_data_check_process();
        break;
    case CAPWAP_BUSINESS_DATA_TRANSFER:
        ret = business_data_transfer_process();
        break;
    case CAPWAP_BUSINESS_ECHO:
        ret = business_echo_process();
        break;
    case CAPWAP_BUSINESS_INIT_CONFIG:
        ret = business_init_config_process();
        break;
    case CAPWAP_BUSINESS_AP_CONFIG_RSP:
        ret = business_ap_config_rsp();
        break;
    case CAPWAP_BUSINESS_WLAN_CONFIG_RSP:
        ret = business_wlan_config_rsp();
        break;
    case CAPWAP_BUSINESS_WTP_EVENT_REQ:
        ret = business_wtp_event_req();
        break;
    case CAPWAP_BUSINESS_AP_INFO_REQ:
        ret = business_ap_info_req();
        break;
    case CAPWAP_BUSINESS_AP_LEAVE:
        ret = business_ap_leave();
        break;
    case CAPWAP_BUSINESS_NOTIFY_STATUS:
        ret = business_notify_status();
        break;
    case CAPWAP_BUSINESS_CONFIG_UPDATE_RSP:
        ret = business_config_update_rsp();
        break;
    case CAPWAP_BUSINESS_MODIFY:
        ret = business_modify();
        break;
    case CAPWAP_BUSINESS_ADD_WLAN:
        ret = business_add_wlan();
        break;
    case CAPWAP_BUSINESS_DEL_WLAN:
        ret = business_del_wlan();
        break;
    default:
        dlog(LOG_ERR, "%s.%d unknown this business-type %d", __FUNC__, __LINE__, business_type);
        break;
    }

    return ret;
}

int CBusiness::business_discovery_process()
{
    CCapwapDiscoveryRsp *prsp = NULL;
    CBuffer buffer;
    string inter_addr = "";
    kvlist kv;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    prsp = (CCapwapDiscoveryRsp *)capwap_get_packet(CAPWAP_PACKET_TYPE_DISCOVERY_RSP);
    if (NULL == prsp)
    {
        return BUSINESS_FAIL;
    }

    // 配置发送到AP的消息元类型组合
    prsp->ac_name.setValid(true);
    prsp->ipv4_addr.setValid(true);
    prsp->pay_load.setValid(true);
    prsp->pay_load.ap_trans.setValid(true);

    // 配置发送到AP的值
    inter_addr = GetInterfaceAddress("eth0");

    // TODO 确定ac的配置信息（ac名称，接口ip，转发方式)
    SetValue(kv, STRING_AC_NAME, "dunchong");
    SetValue(kv, STRING_CTRL_IPV4_ADDR, inter_addr);
    // SetValue(kv, STRING_AP_TRANS_TYPE, toString(0));

    prsp->LoadFrom(kv);

    buffer.extend();
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char *)buffer.GetBuffer(), buffer.GetOffset());
    SAFE_DELETE(prsp);

    return BUSINESS_SUCCESS;
}

int CBusiness::business_join_process()
{

    kvlist kv = ParseString(src_info);
    DBResult result;
    string cond = "";
    char wan_ip[32] = {0};
    CCapwapJoinRsp *prsp = NULL;
    CBuffer buffer;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    //  TODO 确定是否AP数量达到最大值，是否允许join

    // 更新AP结构中的内容
    inet_ntop(AF_INET, &ap->cl->peer_addr.sin_addr.s_addr, wan_ip, sizeof(wan_ip));

    ap_dev_set_hw_addr(ap, GetValue(kv, STRING_AP_MAC).c_str());
    ap_dev_set_dev_model(ap, GetValue(kv, STRING_DEV_MODEL).c_str());
    ap_dev_set_serial_number(ap, GetValue(kv, STRING_SERIAL_NUMBER).c_str());
    ap_dev_set_company_id(ap, GetValue(kv, STRING_COMPANY_ID).c_str());
    ap_dev_set_hardware_version(ap, GetValue(kv, STRING_HARDWARE_VERSION).c_str());
    ap_dev_set_software_version(ap, GetValue(kv, STRING_SOFTWARE_VERSION).c_str());
    ap_dev_set_lan_ip(ap, GetValue(kv, STRING_LAN_IPADDR).c_str());
    ap_dev_set_wan_ip(ap, wan_ip);

    // 确定AP有几个射频模块，下发配置时对应射频下发对应的射频配置
    ap_dev_set_radios(ap, kv);

    cond.append(STRING_AP_MAC"='" + string(ap->hw_addr) + "'");
    DBI::Query(TO_STR(AP_LIST), "*", result, cond.c_str());

    switch(result.size())
    {
    case 0:
    {
        string field = "", value = "";

        field.append(STRING_AP_MAC",");
        field.append(STRING_SERIAL_NUMBER",");
        field.append(STRING_WAN_IPADDR",");
        field.append(STRING_LAN_IPADDR",");
        field.append(STRING_DEV_MODEL",");
        field.append(STRING_COMPANY_ID",");
        field.append(STRING_HARDWARE_VERSION",");
        field.append(STRING_SOFTWARE_VERSION",");
        field.append(STRING_STATE",");
        field.append(STRING_CREATE_TIME);

        value.append("'" + string(ap->hw_addr) + "',");
        value.append("'" + string(ap->serial_number) + "',");
        value.append("'" + string(ap->wan_ip) + "',");
        value.append("'" + string(ap->lan_ip) + "',");
        value.append("'" + string(ap->dev_model) + "',");
        value.append("'" + string(ap->company_id) + "',");
        value.append("'" + string(ap->hardware_version) + "',");
        value.append("'" + string(ap->software_version) + "',");
        value.append(toString(ap->state) + ",");
        value.append(toString(time(NULL)));

        DBI::Insert(TO_STR(AP_LIST), field.c_str(), value.c_str());
        break;
    }
    case 1:
    {
        string key_value;
        ap_dev_load_from_db(ap, result[0]);
        key_value.append(STRING_LEAVE_TIME"=NULL,");
        key_value.append(STRING_ONLINE_TIME"=" + toString(time(NULL)) + ",");
        key_value.append(STRING_STATE"=" + toString(ap->state) + "");

        DBI::Update(TO_STR(AP_LIST), key_value.c_str(), cond.c_str());
        break;
    }
    default:
        dlog(LOG_ERR, "MULTI ROW DATA in DATABASES THIS MAC %s", ap->hw_addr);
        return BUSINESS_FAIL;
    }

    prsp = (CCapwapJoinRsp *)capwap_get_packet(CAPWAP_PACKET_TYPE_JOIN_RSP);
    if (NULL == prsp)
    {
        return BUSINESS_FAIL;
    }

    // TODO AP上线升级开关是否开启 版本号对比

    prsp->result.setValid(true);

    // 设置回复给AP的消息元类型及值
    kv.clear();
    SetValue(kv, STRING_RESULT_CODE, toString(0));

    prsp->LoadFrom(kv);
    buffer.extend();
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char *)buffer.GetBuffer(), buffer.GetOffset());

    // 添加到ap列表
    add_ap_dev(ap);

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_configure_process()
{
    // configure status req 的内容不需要处理，直接下发分组中的配置
    CCapwapConfigureStatusRsp *prsp = NULL;
    int i = 0;
    string cond = "";
    CBuffer buffer;
    kvlist kv;
    kvlist kv_2g_config;
    kvlist kv_5g_config;

    update_station_state(ap);

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    // 查找2G射频信息
    cond = STRING_NAME"=(select "TO_STR(STRING_RADIO_2G_STRATEGY_NAME)" from "TO_STR(GROUP_LIST)
                " where "STRING_NAME"=(select "TO_STR(STRING_GROUP_NAME)" from "TO_STR(AP_LIST)
                " where "STRING_AP_MAC"='" + string(ap->hw_addr) + "'))";

    Add(kv, data_line_from_db(TO_STR(RADIO_2G_LIST), cond, "", "0"));

    if (ap->max_radios == 2)
    {
        // 查找5G射频信息
        cond = STRING_NAME"=(select "TO_STR(STRING_RADIO_5G_STRATEGY_NAME)" from "TO_STR(GROUP_LIST)
            " where "STRING_NAME"=(select "TO_STR(STRING_GROUP_NAME)" from "TO_STR(AP_LIST)
            " where "STRING_AP_MAC"='" + string(ap->hw_addr) + "'))";
        Add(kv, data_line_from_db(TO_STR(RADIO_5G_LIST), cond, "", "1"));
    }

    // TODO 数据库结构不应该在这里体现，需要单独的文件来存放，使用宏控制
    for (int i=0; i<ap->max_radios; i++)
    {
        int radio_id = i+1;
        SetValue(kv, STRING_RADIO_ID + toString(i), toString(radio_id));
    }
    // TODO 设置echo超时的时间和间隔
    SetValue(kv, STRING_ECHO_INTERVAL, toString(10));
    SetValue(kv, STRING_ECHO_COUNT, toString(3));

    DumpKv(kv);

    prsp = (CCapwapConfigureStatusRsp *)capwap_get_packet(CAPWAP_PACKET_TYPE_CONFIG_STATUS_RSP);
    if (prsp == NULL)
        return BUSINESS_FAIL;

    // 将所有的消息元扩展为最大radios数，后面依据radio的类型发送对应的dsctrl 或者ofdmctrl

    prsp->radio_admin_states.resize(ap->max_radios);
    prsp->radio_confs.resize(ap->max_radios);
    prsp->radio_infos.resize(ap->max_radios);
    prsp->mac_operations.resize(ap->max_radios);
    prsp->tx_powers.resize(ap->max_radios);
    prsp->ds_ctrls.resize(ap->max_radios);
    prsp->ofdm_ctrls.resize(ap->max_radios);
    prsp->pay_loads.resize(ap->max_radios);

    for (i = 0; i<ap->max_radios; i++)
    {
        prsp->radio_admin_states[i].setValid(true);
        prsp->radio_confs[i].setValid(true);
        prsp->radio_infos[i].setValid(true);
        prsp->mac_operations[i].setValid(true);
        prsp->tx_powers[i].setValid(true);

        if (ap->radios[i].radio_type & RADIO_TYPE_11A) // 5G radio
            prsp->ofdm_ctrls[i].setValid(true);
        else // 2G radio
            prsp->ds_ctrls[i].setValid(true);

        prsp->pay_loads[i].setValid(true);
        prsp->pay_loads[i].radio_conf.setValid(true);
        prsp->pay_loads[i].abp_radio_conf.setValid(true);
        prsp->pay_loads[i].radio_probe_conf.setValid(true);
        prsp->pay_loads[i].packet_power_conf.setValid(true);
        prsp->pay_loads[i].channel_reuse_conf.setValid(true);
    }
    // config status response除了radio配置之外还有一些其它配置项，默认放到第一个vendor内
    prsp->pay_loads[0].ap_space_info.setValid(true);
    prsp->pay_loads[0].echo_conf.setValid(true);
    prsp->pay_loads[0].traffic_statics_conf.setValid(true);

    prsp->LoadFrom(kv);

    buffer.extend(10240);
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_data_check_process()
{
    CCapwapChangeStateRsp *prsp = NULL;
    CBuffer buffer;

    update_station_state(ap);

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    prsp = (CCapwapChangeStateRsp *)capwap_get_packet(CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_RSP);
    if (prsp == NULL)
        return BUSINESS_FAIL;

    buffer.extend();
    prsp->Assemble(buffer);
    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_data_transfer_process()
{
    CCapwapDataTransferRsp *prsp = NULL;
    CBuffer buffer;
    kvlist kv;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    prsp = (CCapwapDataTransferRsp *)capwap_get_packet(CAPWAP_PACKET_TYPE_DATA_TRANSFER_RSP);
    if (prsp == NULL)
        return BUSINESS_FAIL;

    prsp->result.setValid(true);
    SetValue(kv, STRING_RESULT_CODE, "0");

    buffer.extend();
    prsp->LoadFrom(kv);
    prsp->Assemble(buffer);

    // 发送此报文会导致AP永远无法上线
    // ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_echo_process()
{
    CCapwapEchoRsp *prsp = NULL;
    CBuffer buffer;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    prsp = (CCapwapEchoRsp*)capwap_get_packet(CAPWAP_PACKET_TYPE_ECHO_RSP);
    if (NULL == prsp)
        return BUSINESS_FAIL;

    ap->echo_timeout_cnt = 0;
    buffer.extend();
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_init_config_process()
{
    // run 更新为收到AP的第一个echo request
    update_station_state(ap);

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    if (BUSINESS_SUCCESS != business_init_ap_config())
    {
        dlog(LOG_ERR, "%s.%d init ap config error", __FUNC__, __LINE__);
    }
    if (BUSINESS_SUCCESS != business_init_wlan_config())
    {
        dlog(LOG_ERR, "%s.%d init wlan config error", __FUNC__, __LINE__);
    }


    return BUSINESS_SUCCESS;
}

int CBusiness::business_init_ap_config()
{
    CCapwapAPConfReq *preq = NULL;
    CBuffer buffer;
    vector<string> group_config;
    vector<string> wp_config;
    vector<string> rfg_config;
    vector<string> lan_portal_config;
    vector<string> rate_set_config;
    DBResult result;
    string cond;
    kvlist kv;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    // 获取对应分组
    cond = STRING_NAME"=(select "TO_STR(STRING_GROUP_NAME) " from "TO_STR(AP_LIST)" where "STRING_AP_MAC"='" + toString(ap->hw_addr) + "')";
    Add(kv, data_line_from_db(TO_STR(GROUP_LIST), cond, "", ""));

    // 获取无线定位策略
    cond = STRING_NAME"='" + GetValue(kv, TO_STR(STRING_WP_STRATEGY_NAME)) + "'";
    Add(kv, data_line_from_db(TO_STR(WP_LIST), cond, "", ""));

    // 获取有线策略
    cond = STRING_NAME"='" + GetValue(kv, TO_STR(STRING_LAN_STRATEGY_NAME)) + "'";
    Add(kv, data_line_from_db(TO_STR(LAN_PORTAL_LIST), cond, "", ""));

    // 获取频谱导航策略
    cond = STRING_NAME"='" + GetValue(kv, TO_STR(STRING_RFG_STRATEGY_NAME)) + "'";
    Add(kv, data_line_from_db(TO_STR(RFG_LIST), cond, "", ""));

    // 获取速率集设置策略
    cond = STRING_NAME"='" + GetValue(kv, TO_STR(STRING_RATE_SET_STRATEGY_NAME)) + "'";
    Add(kv, data_line_from_db(TO_STR(RATE_SET_LIST), cond, "", ""));

    cond = STRING_NAME"='" + GetValue(kv, TO_STR(STRING_NTP_STRATEGY_NAME)) + "'";
    Add(kv, data_line_from_db(TO_STR(NTP_SERVER_LIST), cond, "", ""));

    cond = STRING_NAME"='" + GetValue(kv, TO_STR(STRING_PORTAL_PARAM_CUSTOM_STRATEGY_NAME)) + "'";
    Add(kv, data_line_from_db(TO_STR(PORTAL_CUSTOM_LIST), cond, "", ""));

    DumpKv(kv);

    preq = (CCapwapAPConfReq *)capwap_get_packet(CAPWAP_PACKET_TYPE_AP_CONFIG_REQ);
    if (NULL == preq)
        return BUSINESS_FAIL;

    // preq->outside_auth_conf.setValid(true); //小AC不支持外部radius配置
    // preq->nat_dhcp_conf.setValid(true); //小AC不支持配置DHCP及NAT
    preq->report_station_enable.setValid(true); // Y
    preq->roming_conf.setValid(true);           // Y
    preq->wp_conf.setValid(true);               // Y
    preq->rfg_conf.setValid(true);              // Y
    preq->ap_lb_conf.setValid(true);            // Y
    preq->rate_set_conf.setValid(true);         // Y
    preq->low_rssi_conf.setValid(true);         // Y
    preq->connection_mode_conf.setValid(true);  // Y
    preq->lan_vlan_conf.setValid(true);         // Y
    preq->report_station_interval.setValid(true); // Y
    preq->audit_appri_conf.setValid(true);      // N dunchong not support
    preq->lan_portal_conf.setValid(true);       // Y
    preq->pay_load.setValid(true);
    preq->pay_load.portal_custom.setValid(true);// Y
    preq->pay_load.time_stamp.setValid(true);   // Y
    preq->pay_load.by_pass.setValid(true);      // Y
    preq->pay_load.ntp_server.setValid(true);   // Y

    preq->LoadFrom(kv);
    buffer.extend();
    preq->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(preq);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_init_wlan_config()
{
    CCapwapWlanConfigReq *preq = NULL;
    string cond;
    size_t wlan_count;
    size_t wlan_2g_count;
    DBResult result;
    kvlist kv;
    CBuffer buffer;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    cond = STRING_NAME"=(select "TO_STR(STRING_WLAN_2G_STRATEGY_NAME)" from "TO_STR(GROUP_LIST)
     " where "STRING_NAME"=(select "TO_STR(STRING_GROUP_NAME)" from "TO_STR(AP_LIST)
     " where "STRING_AP_MAC"='" + toString(ap->hw_addr) + "'))";
    Add(kv, data_multi_line_from_db(TO_STR(WLAN_LIST), cond, "", "", 0));

    wlan_count = toInt(GetValue(kv, STRING_COUNT));
    wlan_2g_count = wlan_count;
    for (size_t i=0 ;i<wlan_count; i++)
    {
        SetValue(kv, STRING_RADIO_ID + toString(i), "1");
    }

    if (ap->max_radios == 2)
    {
        cond = STRING_NAME"=(select "TO_STR(STRING_WLAN_5G_STRATEGY_NAME)" from "TO_STR(GROUP_LIST)
            " where "STRING_NAME"=(select "TO_STR(STRING_GROUP_NAME)" from "TO_STR(AP_LIST)
            " where "STRING_AP_MAC"='" + toString(ap->hw_addr) + "'))";

        Add(kv, data_multi_line_from_db(TO_STR(WLAN_LIST), cond, "", "", wlan_2g_count));
        wlan_count = wlan_2g_count + toInt(GetValue(kv, STRING_COUNT));
    }

    for (size_t i=wlan_2g_count ;i<wlan_count; i++)
    {
        SetValue(kv, STRING_RADIO_ID + toString(i), "2");
    }

    for (size_t i=0; i<wlan_count; i++)
    {
        cond = STRING_NAME"=\"" + GetValue(kv, TO_STR(STRING_WLAN_SECURE_STRATEGY) + toString(i)) + "\"";
        Add(kv, data_line_from_db(TO_STR(WLAN_SECURE_LIST), cond, "", toString(i)));
    }

    SetValue(kv, STRING_COUNT, toString(wlan_count));
    DumpKv(kv);

    preq = (CCapwapWlanConfigReq *)capwap_get_packet(CAPWAP_PACKET_TYPE_WLAN_CONFIG_REQ);
    if (NULL == preq)
        return BUSINESS_FAIL;

    preq->add_wlans.resize(wlan_count);
    for(size_t i=0; i<wlan_count; i++)
    {
        preq->add_wlans[i].setValid(true);
    }
    preq->pay_loads.resize(wlan_count);
    for(size_t i=0; i<wlan_count; i++)
    {
        preq->pay_loads[i].setValid(true);
        preq->pay_loads[i].vlan_conf.setValid(true);
        preq->pay_loads[i].mcast_enhance.setValid(true);
        preq->pay_loads[i].wx_auth.setValid(true);
        preq->pay_loads[i].wlan_info.setValid(true);
    }


    preq->LoadFrom(kv);
    buffer.extend(102400);
    preq->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(preq);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_ap_config_rsp()
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    return BUSINESS_SUCCESS;
}
int CBusiness::business_wlan_config_rsp()
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    return BUSINESS_SUCCESS;
}

int CBusiness::business_add_station()
{
    kvlist kv = ParseString(src_info);

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    ubus_add_station(kv);

    // TODO 发送ubus消息到auth
    return BUSINESS_SUCCESS;
}
int CBusiness::business_multi_add_station()
{
    kvlist kv = ParseString(src_info);
    uint32_t count = toInt(GetValue(kv, STRING_COUNT));
    kvlist kv_multi_add;
    string macs,ssids,ips,radios;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    if (0 == count)
        return BUSINESS_SUCCESS;

    SetValue(kv_multi_add, STRING_USER_COUNT, toString(count));
    SetValue(kv_multi_add, STRING_AP_MAC, GetValue(kv, STRING_AP_MAC"0"));

    for (size_t i=0; i<count; i++)
    {
        macs.append(GetValue(kv, STRING_STA_MAC + toString(i)) + ";");
        ssids.append(GetValue(kv, TO_STR(STRING_WLAN_ESSID) + toString(i)) + ";");
        ips.append(GetValue(kv, STRING_USER_IP + toString(i)) + ";");
        radios.append(GetValue(kv, STRING_RADIO_ID + toString(i)) + ";");
    }

    SetValue(kv_multi_add, STRING_STA_MAC, macs);
    SetValue(kv_multi_add, TO_STR(STRING_WLAN_ESSID), ssids);
    SetValue(kv_multi_add, STRING_USER_IP, ips);
    SetValue(kv_multi_add, STRING_RADIO_ID, radios);

    DumpKv(kv_multi_add);

    ubus_multi_add_station(kv_multi_add);

    return BUSINESS_SUCCESS;
}
int CBusiness::business_del_station()
{
    kvlist kv = ParseString(src_info);
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    ubus_del_station(kv);

    return BUSINESS_SUCCESS;
}

int CBusiness::business_wtp_event_req()
{
    CCapwapWTPEventReq *preq = (CCapwapWTPEventReq *)packet();
    CCapwapWTPEventRsp *prsp = NULL;
    CBuffer buffer;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    if (preq == NULL)
        return BUSINESS_FAIL;

    if (preq->add_station.isValid())
        business_add_station();
    else if (preq->del_station.isValid())
        business_del_station();
    else if (preq->pay_load.sta_state.isValid())
        business_multi_add_station();
    else if (preq->pay_load.actl_sta_info.isValid())
        business_multi_process_station();
    else if (preq->pay_load.actl_user_info.isValid())
        business_user_info();
    else
    {
        dlog(LOG_ERR, "%s.%d AP %s REQUEST SOMETHING BY WTP EVENT REQUEST BUT NO BUSINESS PROCESS!",
             __FUNC__, __LINE__, ap->hw_addr);
    }

    prsp = (CCapwapWTPEventRsp *)capwap_get_packet(CAPWAP_PACKET_TYPE_WTP_EVENT_RSP);
    if (prsp == NULL)
        return BUSINESS_FAIL;

    prsp->setSequenceNum(preq->getSequenceNum());
    buffer.extend();
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    // 注意：不要释放preq，此对象需要在capwap_state中释放
    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_ap_info_req()
{
    CCapwapAPInfoRsp *prsp = NULL;
    CBuffer buffer;
    kvlist kv;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    prsp = (CCapwapAPInfoRsp *)capwap_get_packet(CAPWAP_PACKET_TYPE_AP_INFO_RSP);
    if (NULL == prsp)
        return BUSINESS_FAIL;

    prsp->result.setValid(true);
    SetValue(kv, STRING_RESULT_CODE, toString(0));

    prsp->LoadFrom(kv);
    buffer.extend();
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_ap_leave()
{
    string cond;
    string key_value;

    update_station_state(ap);

    dlog(LOG_DEBUG, "%s.%d {AP %s %s %s:%d}", __FUNC__, __LINE__,
         ap->hw_addr, ap->lan_ip, ap->wan_ip, ntohs(ap->cl->peer_addr.sin_port));

    key_value.append(STRING_LEAVE_TIME"=" + toString(time(NULL)) + ",");
    key_value.append(STRING_ONLINE_TIME"=NULL");
    cond.append(STRING_AP_MAC"='" + toString(ap->hw_addr) + "'");

    DBI::Update(TO_STR(AP_LIST), key_value.c_str(), cond.c_str());

    // 清空任务列表中该AP的任务
    flush_business(ap);

    del_ap_dev(ap);

    return BUSINESS_SUCCESS;
}

int CBusiness::business_notify_status()
{
    CCapwapConfigUpdateReq *preq = NULL;
    CBuffer buffer;
    kvlist kv = ParseString(src_info);
    kvlist kv0;   // 需要一个带下标的kvlist

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    if (!isOnline(ap))
    {
        dlog(LOG_ERR, "%s.%d AP %s Not Online!", __FUNC__, __LINE__,
             GetValue(kv, STRING_AP_MAC).c_str());
        return BUSINESS_FAIL;
    }

    preq = (CCapwapConfigUpdateReq *)capwap_get_packet(CAPWAP_PACKET_TYPE_CONFIG_UPDATE_REQ);
    if (NULL == preq)
        return BUSINESS_FAIL;

    preq->pay_loads.resize(1);
    preq->pay_loads[0].setValid(true);
    preq->pay_loads[0].actl_user_info.setValid(true);

    // 给元素值添加下标值
    for (kvlist::iterator it = kv.begin(); it != kv.end(); it++)
    {
        SetValue(kv0, it->first + toString(0), it->second);
    }

    DumpKv(kv0);

    preq->LoadFrom(kv0);
    buffer.extend();
    preq->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(preq);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_multi_process_station()
{
    kvlist kv = ParseString(src_info);
    uint32_t pkt_type;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    pkt_type = toInt(GetValue(kv, STRING_PKT_TYPE"0"));
    if (1 == pkt_type) // TODO 流量上报
    {

    }
    else               // 上线回应 未认证
    {
        kvlist kv_ack;
        SetValue(kv_ack, STRING_PKT_ID, GetValue(kv, STRING_PKT_ID"0"));
        SetValue(kv_ack, STRING_STATE, toString(1));
        SetValue(kv_ack, STRING_STA_MAC, GetValue(kv, STRING_STA_MAC"0"));

        DumpKv(kv_ack);

        ubus_ack(kv_ack);
    }

    return BUSINESS_SUCCESS;
}

int CBusiness::business_config_update_rsp()
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    return BUSINESS_SUCCESS;
}

int CBusiness::business_user_info()
{
    kvlist kv = ParseString(src_info);
    kvlist kv_ack;
    uint32_t pkt_type;
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    pkt_type = toInt(GetValue(kv, STRING_PKT_TYPE));
    if (1 == pkt_type)   // 未知消息，AP不会主动发送PKT TYPE为1的user_info
    {
        dlog(LOG_ERR, "%s.%d Not Support This Pkt_Type %d", __FUNC__, __LINE__, pkt_type);
    }
    else
    {
        SetValue(kv_ack, STRING_PKT_ID, GetValue(kv, STRING_PKT_ID));
        SetValue(kv_ack, STRING_STATE, GetValue(kv, STRING_SUB_TYPE));  // SUB_TYPE就是终端的认证状态
        SetValue(kv_ack, STRING_STA_MAC, GetValue(kv, STRING_STA_MAC));

        DumpKv(kv_ack);

        ubus_ack(kv_ack);
    }

    return BUSINESS_SUCCESS;
}
int CBusiness::business_modify()
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    if (!isOnline(ap))
    {
        dlog(LOG_ERR, "%s.%d AP %s Not Online!", __FUNC__, __LINE__,
             ap->hw_addr);
        return BUSINESS_FAIL;
    }

    return run_modify_process(ap, src_info, ci);
}
int CBusiness::business_add_wlan()
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    kvlist kv;
    kvlist kv_tmp;
    kvlist kv_temp;
    string cond = src_info; // 使用页面下发的key value对来确定添加的是哪一个wlan
    CCapwapWlanConfigReq *preq = NULL;
    CBuffer buffer;
    size_t wlans = 0;

    if (!isOnline(ap))
    {
        dlog(LOG_ERR, "%s.%d AP %s Not Online!", __FUNC__, __LINE__,
             ap->hw_addr);
        return BUSINESS_FAIL;
    }

    kv_temp = data_line_from_db(TO_STR(WLAN_LIST), src_info, "", "");
    DumpKv(kv_temp);

    cond = STRING_NAME"=(select "TO_STR(STRING_GROUP_NAME)" from "TO_STR(AP_LIST)
        " where "STRING_AP_MAC"=\"" + string(ap->hw_addr) + "\")";

    kv_tmp = data_line_from_db(TO_STR(GROUP_LIST), cond.c_str(),
                               TO_STR(STRING_WLAN_2G_STRATEGY_NAME)","
                               TO_STR(STRING_WLAN_5G_STRATEGY_NAME), "");
    DumpKv(kv_tmp);

    if (GetValue(kv_tmp, TO_STR(STRING_WLAN_2G_STRATEGY_NAME)) == GetValue(kv_temp, STRING_NAME))
    {
        Add(kv, data_line_from_db(TO_STR(WLAN_LIST), src_info, "", toString(wlans)));

        cond = STRING_NAME"='" +
            GetValue(kv, TO_STR(STRING_WLAN_SECURE_STRATEGY) + toString(wlans)) + "'";
        Add(kv, data_line_from_db(TO_STR(WLAN_SECURE_LIST), cond, "", toString(wlans)));
        SetValue(kv, STRING_RADIO_ID + toString(wlans), "1");
        wlans++;
    }

    if (GetValue(kv_tmp, TO_STR(STRING_WLAN_5G_STRATEGY_NAME)) == GetValue(kv_temp, STRING_NAME))
    {
        Add(kv, data_line_from_db(TO_STR(WLAN_LIST), src_info, "", toString(wlans)));

        cond = STRING_NAME"='" +
            GetValue(kv, TO_STR(STRING_WLAN_SECURE_STRATEGY) + toString(wlans)) + "'";
        Add(kv, data_line_from_db(TO_STR(WLAN_SECURE_LIST), cond, "", toString(wlans)));
        SetValue(kv, STRING_RADIO_ID + toString(wlans), "2");
        wlans++;
    }
    SetValue(kv, STRING_COUNT, toString(wlans));
    DumpKv(kv);

    preq = (CCapwapWlanConfigReq *)capwap_get_packet(CAPWAP_PACKET_TYPE_WLAN_CONFIG_REQ);
    if (preq == NULL)
        return BUSINESS_FAIL;

    preq->add_wlans.resize(wlans);
    preq->pay_loads.resize(wlans);
    for (size_t i=0; i<wlans; i++)
    {
        preq->add_wlans[i].setValid(true);
        preq->pay_loads[i].setValid(true);
        preq->pay_loads[i].vlan_conf.setValid(true);
        preq->pay_loads[i].mcast_enhance.setValid(true);
        preq->pay_loads[i].wx_auth.setValid(true);
        preq->pay_loads[i].wlan_info.setValid(true);
    }

    preq->LoadFrom(kv);
    buffer.extend();
    preq->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(preq);
    return BUSINESS_SUCCESS;
}
int CBusiness::business_del_wlan()
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    kvlist kv = ParseString(src_info);
    CCapwapWlanConfigReq *preq = NULL;
    CBuffer buffer;
    size_t wlans = toInt(GetValue(kv, STRING_COUNT));

    preq = (CCapwapWlanConfigReq *)capwap_get_packet(CAPWAP_PACKET_TYPE_WLAN_CONFIG_REQ);
    if (NULL == preq)
        return BUSINESS_FAIL;

    preq->del_wlans.resize(wlans);
    for (size_t i=0; i<wlans; i++)
        preq->del_wlans[i].setValid(true);

    preq->LoadFrom(kv);
    buffer.extend();
    preq->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(preq);
    return BUSINESS_SUCCESS;
}




int add_business(CBusiness *pbusiness)
{
    dlog(LOG_DEBUG, "%s.%d {}", __FUNC__, __LINE__);
    businesses.push_back(pbusiness);
    return 0;
}
int flush_business()
{
    dlog(LOG_DEBUG, "%s.%d {}", __FUNC__, __LINE__);
    while (businesses.size())
    {
        CBusiness *pbusiness = businesses.front();
        SAFE_DELETE(pbusiness);
        businesses.pop_front();
    }
    return 0;
}
int flush_business(struct ap_dev *ap)
{
    dlog(LOG_DEBUG, "%s.%d {AP %s %s %s:%d}", __FUNC__, __LINE__,
         ap->hw_addr, ap->lan_ip, ap->wan_ip, ntohs(ap->cl->peer_addr.sin_port));
    list<CBusiness*>::iterator it = businesses.begin();
    for (; it != businesses.end(); it++)
    {
        if ((*it)->ap_dev() == ap)
        {
            list<CBusiness*>::iterator ite = it++;
            SAFE_DELETE(*ite);
            businesses.erase(ite);
        }
    }

    return 0;
}
void business_timeout_cb(struct uloop_timeout *timeout)
{
    if (businesses.size())
    {
        dlog(LOG_DEBUG, "%s.%d {}", __FUNC__, __LINE__);
        CBusiness *pbusiness = businesses.front();
        pbusiness->Process();
        SAFE_DELETE(pbusiness);
        businesses.pop_front();
    }

    uloop_timeout_set(&business_timeout, BUSINESS_TIMEOUT_INTERVAL);
}
