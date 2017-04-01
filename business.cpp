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

int CBusiness::Process(struct ap_dev *ap)
{
    int ret = BUSINESS_FAIL;

    switch(business_type)
    {
    case CAPWAP_BUSINESS_DISCOVERY:
        ret = business_discovery_process(ap);
        break;
    case CAPWAP_BUSINESS_JOIN:
        ret = business_join_process(ap);
        break;
    case CAPWAP_BUSINESS_CONFIGURE:
        ret = business_configure_process(ap);
        break;
    case CAPWAP_BUSINESS_DATA_CHECK:
        ret = business_data_check_process(ap);
        break;
    case CAPWAP_BUSINESS_DATA_TRANSFER:
        ret = business_data_transfer_process(ap);
        break;
    case CAPWAP_BUSINESS_ECHO:
        ret = business_echo_process(ap);
        break;
    case CAPWAP_BUSINESS_INIT_CONFIG:
        ret = business_init_config_process(ap);
        break;
    case CAPWAP_BUSINESS_AP_CONFIG_RSP:
        ret = business_ap_config_rsp(ap);
        break;
    case CAPWAP_BUSINESS_WLAN_CONFIG_RSP:
        ret = business_wlan_config_rsp(ap);
        break;
    case CAPWAP_BUSINESS_WTP_EVENT_REQ:
        ret = business_wtp_event_req(ap);
        break;
    case CAPWAP_BUSINESS_AP_INFO_REQ:
        ret = business_ap_info_req(ap);
        break;
    case CAPWAP_BUSINESS_AP_LEAVE:
        ret = business_ap_leave(ap);
        break;
    case CAPWAP_BUSINESS_NOTIFY_STATUS:
        ret = business_notify_status(ap);
        break;
    case CAPWAP_BUSINESS_CONFIG_UPDATE_RSP:
        ret = business_config_update_rsp(ap);
        break;
    default:
        dlog(LOG_ERR, "%s.%d unknown this business-type %d", __FUNC__, __LINE__, business_type);
        break;
    }

    return ret;
}

int CBusiness::business_discovery_process(struct ap_dev *ap)
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
    SetValue(kv, STRING_AP_TRANS_TYPE, toString(0));

    prsp->LoadFrom(kv);

    buffer.extend();
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char *)buffer.GetBuffer(), buffer.GetOffset());
    SAFE_DELETE(prsp);

    return BUSINESS_SUCCESS;
}

int CBusiness::business_join_process(struct ap_dev *ap)
{

    kvlist kv = ParseString(src_info);
    DBResult result;
    string cond = "";
    char wan_ip[32] = {0};
    CCapwapJoinRsp *prsp = NULL;
    CBuffer buffer;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    // 更新AP结构中的内容
    inet_ntop(AF_INET, &ap->cl->peer_addr.sin_addr.s_addr, wan_ip, sizeof(wan_ip));

    ap_dev_set_hw_addr(ap, GetValue(kv, STRING_AP_MAC).c_str());
    ap_dev_set_dev_model(ap, GetValue(kv, STRING_DEV_MODEL).c_str());
    ap_dev_set_serial_number(ap, GetValue(kv, STRING_SERIAL_NUMBER).c_str());
    ap_dev_set_company_id(ap, GetValue(kv, STRING_COMPANY_ID).c_str());
    ap_dev_set_hardware_version(ap, GetValue(kv, STRING_HARDWARE_VERSION).c_str());
    ap_dev_set_lan_ip(ap, GetValue(kv, STRING_LAN_IPADDR).c_str());
    ap_dev_set_wan_ip(ap, wan_ip);

    // 确定AP有几个射频模块，下发配置时对应射频下发对应的射频配置
    ap_dev_set_radios(ap, kv);

    cond.append(DB_STRING_AP_MAC"='" + string(ap->hw_addr) + "'");
    DBI::Query(AP_LIST, "*", result, cond.c_str());

    switch(result.size())
    {
    case 0:
    {
        string field = "", value = "";

        field.append(DB_STRING_AP_MAC",");
        field.append(DB_STRING_SERIAL_NUMBER",");
        field.append(DB_STRING_WAN_IP",");
        field.append(DB_STRING_LAN_IP",");
        field.append(DB_STRING_DEV_MODEL",");
        field.append(DB_STRING_COMPANY_ID",");
        field.append(DB_STRING_HARDWARE_VERSION",");
        field.append(DB_STRING_STATE",");
        field.append(DB_STRING_CREATE_TIME);

        value.append("'" + string(ap->hw_addr) + "',");
        value.append("'" + string(ap->serial_number) + "',");
        value.append("'" + string(ap->wan_ip) + "',");
        value.append("'" + string(ap->lan_ip) + "',");
        value.append("'" + string(ap->dev_model) + "',");
        value.append("'" + string(ap->company_id) + "',");
        value.append("'" + string(ap->hardware_version) + "',");
        value.append(toString(ap->state) + ",");
        value.append(toString(time(NULL)));

        DBI::Insert(AP_LIST, field.c_str(), value.c_str());
        break;
    }
    case 1:
    {
        string key_value;
        ap_dev_load_from_db(ap, result[0]);
        key_value.append(DB_STRING_LEAVE_TIME"=NULL,");
        key_value.append(DB_STRING_ONLINE_TIME"=" + toString(time(NULL)));

        DBI::Update(AP_LIST, key_value.c_str(), cond.c_str());
        break;
    }
    default:
        dlog(LOG_ERR, "MULTI ROW DATA in DATABASES THIS MAC %s", ap->hw_addr);
        break;
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

int CBusiness::business_configure_process(struct ap_dev *ap)
{
    // configure status req 的内容不需要处理，直接下发分组中的配置
    CCapwapConfigureStatusRsp *prsp = NULL;
    int i = 0;
    string cond = "";
    DBResult result;
    vector<vector<string> > radio_config;
    CBuffer buffer;
    kvlist kv;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    radio_config.resize(ap->max_radios);

    // 查找2G射频信息
    cond.append(DB_STRING_RADIO_2G_STRATEGY_NAME"=(");
    cond.append("select "DB_STRING_RADIO_2G_STRATEGY_NAME" from "GROUP_LIST
                " where "DB_STRING_GROUP_NAME"=(");
    cond.append("select "DB_STRING_GROUP_NAME" from "AP_LIST
                " where "DB_STRING_AP_MAC"='" + string(ap->hw_addr) + "'))");
    DBI::Query(RADIO_2G_LIST, "*", result, cond.c_str());
    if (result.size() != 1)
    {
        return BUSINESS_FAIL;
    }
    radio_config[0] = result[0];

    // 查找5G射频信息
    cond.clear();
    cond.append(DB_STRING_RADIO_5G_STRATEGY_NAME"=(");
    cond.append("select "DB_STRING_RADIO_5G_STRATEGY_NAME" from "GROUP_LIST
                " where "DB_STRING_GROUP_NAME"=(");
    cond.append("select "DB_STRING_GROUP_NAME" from "AP_LIST
                " where "DB_STRING_AP_MAC"='" + string(ap->hw_addr) + "'))");
    DBI::Query(RADIO_5G_LIST, "*", result, cond.c_str());
    if (result.size() != 1)
    {
        return BUSINESS_FAIL;
    }
    radio_config[1] = result[0];

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

    // TODO 数据库结构不应该在这里体现，需要单独的文件来存放，使用宏控制
    for (uint32_t i=0; i<radio_config.size(); i++)
    {
        int radio_id = i+1;
        SetValue(kv, STRING_RADIO_ID + toString(i), toString(radio_id));
        SetValue(kv, STRING_RADIO_STATE + toString(i), radio_config[i][1]);
        SetValue(kv, STRING_RADIO_TYPE_11B + toString(i),!!(toInt(radio_config[i][2]) & RADIO_TYPE_11B));
        SetValue(kv, STRING_RADIO_TYPE_11A + toString(i),!!(toInt(radio_config[i][2]) & RADIO_TYPE_11A));
        SetValue(kv, STRING_RADIO_TYPE_11G + toString(i),!!(toInt(radio_config[i][2]) & RADIO_TYPE_11G));
        SetValue(kv, STRING_RADIO_TYPE_11N + toString(i),!!(toInt(radio_config[i][2]) & RADIO_TYPE_11N));
        SetValue(kv, STRING_CURRENT_CHANNEL + toString(i), radio_config[i][3]);
        SetValue(kv, STRING_CURRENT_TXPOWER + toString(i), radio_config[i][4]);
        SetValue(kv, STRING_TX_ANTENNA + toString(i), radio_config[i][5]);
        SetValue(kv, STRING_RX_ANTENNA + toString(i), radio_config[i][5]);
        SetValue(kv, STRING_BAND_WIDTH + toString(i), radio_config[i][6]);
        SetValue(kv, STRING_RTS_THRESHOLD + toString(i), radio_config[i][7]);
        SetValue(kv, STRING_FRAG_THR + toString(i), radio_config[i][8]);
        SetValue(kv, STRING_BEACON_PERIOD + toString(i), radio_config[i][9]);
        SetValue(kv, STRING_ACK_TIMEOUT + toString(i), radio_config[i][10]);
        SetValue(kv, STRING_MIN_CONTRACT_RATE + toString(i), radio_config[i][11]);
        SetValue(kv, STRING_BEACON_RATE + toString(i), radio_config[i][12]);
        SetValue(kv, STRING_PROTECT_MODE + toString(i), radio_config[i][13]);
        SetValue(kv, STRING_PROBE_RSSI + toString(i), radio_config[i][14]);
        SetValue(kv, STRING_DTIM_PERIOD + toString(i), radio_config[i][15]);
        SetValue(kv, STRING_CCA_ADJ + toString(i), radio_config[i][16]);
        SetValue(kv, STRING_CCA_THR + toString(i), radio_config[i][17]);
        SetValue(kv, STRING_A_MPDU + toString(i), radio_config[i][18]);
        SetValue(kv, STRING_A_MSDU + toString(i), radio_config[i][19]);
        SetValue(kv, STRING_SHORT_GI + toString(i), radio_config[i][20]);
        SetValue(kv, STRING_COUNTRY_STRING + toString(i), radio_config[i][21]);
        SetValue(kv, STRING_11N_ONLY + toString(i), radio_config[i][22]);
        SetValue(kv, STRING_SHORT_PREAMBLE + toString(i), radio_config[i][23]);
        SetValue(kv, STRING_AUTO_CHANNEL_SWITCH + toString(i), radio_config[i][24]);
        SetValue(kv, STRING_AUTO_POWER_SWITCH + toString(i), radio_config[i][25]);
        SetValue(kv, STRING_PPC_ENABLE + toString(i), radio_config[i][26]);
        SetValue(kv, STRING_MIN_POWER + toString(i), radio_config[i][27]);
        SetValue(kv, STRING_MIN_THROUGHPUT_THRESHOLD + toString(i), radio_config[i][28]);
        SetValue(kv, STRING_MIN_RATE_THRESHOLD + toString(i), radio_config[i][29]);
    }
    // TODO 设置echo超时的时间和间隔
    SetValue(kv, STRING_ECHO_INTERVAL, toString(10));
    SetValue(kv, STRING_ECHO_TIMEOUT_COUNT, toString(3));

    DumpKv(kv);

    prsp->LoadFrom(kv);

    buffer.extend(10240);
    prsp->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_data_check_process(struct ap_dev *ap)
{
    CCapwapChangeStateRsp *prsp = NULL;
    CBuffer buffer;

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

int CBusiness::business_data_transfer_process(struct ap_dev *ap)
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

int CBusiness::business_echo_process(struct ap_dev *ap)
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

int CBusiness::business_init_config_process(struct ap_dev* ap)
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    if (BUSINESS_SUCCESS != business_init_ap_config(ap))
    {
        dlog(LOG_ERR, "%s.%d init ap config error", __FUNC__, __LINE__);
    }
    if (BUSINESS_SUCCESS != business_init_wlan_config(ap))
    {
        dlog(LOG_ERR, "%s.%d init wlan config error", __FUNC__, __LINE__);
    }


    return BUSINESS_SUCCESS;
}

int CBusiness::business_init_ap_config(struct ap_dev* ap)
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

    cond.append(DB_STRING_GROUP_NAME"=(select "DB_STRING_GROUP_NAME \
                " from "AP_LIST" where "DB_STRING_AP_MAC"='" + toString(ap->hw_addr)
                + "')");
    DBI::Query(GROUP_LIST, "*", result, cond.c_str());
    if (result.size() != 1)
    {
        dlog(LOG_ERR, "%s.%d AP %s GET GROUP ERROR.", __FUNC__, __LINE__, ap->hw_addr);
        return BUSINESS_FAIL;
    }
    group_config = result[0];

    SetValue(kv, STRING_REPORT_STATION_INFO_ENABLE,group_config[DB_INDEX_STATION_TRAFFIC_ENABLE]);
    SetValue(kv, STRING_REPORT_STATION_INFO_INTERVAL,group_config[DB_INDEX_STATION_TRAFFIC_INTERVAL]);
    SetValue(kv, STRING_ROMING_CONFIG_ENABLE, group_config[DB_INDEX_STATION_ROMING_ENABLE]);
    SetValue(kv, STRING_ROMING_CONFIG_SIGNAL, group_config[DB_INDEX_STATION_ROMING_SIGNAL]);
    SetValue(kv, STRING_LOW_RSSI_REFUSE_ENABLE, group_config[DB_INDEX_LOW_SIGNAL_ENABLE]);
    SetValue(kv, STRING_LOW_RSSI_THRESHOLD, group_config[DB_INDEX_LOW_SIGNAL_THRESHOLD]);
    SetValue(kv, STRING_BY_PASS_ENABLE, group_config[DB_INDEX_BY_PASS_ENABLE]);
    SetValue(kv, STRING_AP_LOADBALANCE_ENABLE, group_config[DB_INDEX_LOAD_BALANCE_ENABLE]);
    SetValue(kv, STRING_AP_LOADBALANCE_INTERVAL, group_config[DB_INDEX_SCAN_INTERVAL]);
    SetValue(kv, STRING_AP_LOADBALANCE_THRESHOLD, group_config[DB_INDEX_LOAD_BALANCE_THRESHOLD]);
    SetValue(kv, STRING_LAN_VLAN_ID, group_config[DB_INDEX_VLAN_INTER_ID]);

    cond = "";
    cond.append(DB_STRING_WIRELESS_POSITION_NAME"='" +
                group_config[DB_INDEX_WIRELESS_POSITION_NAME] + "'");
    DBI::Query(WP_LIST, "*", result, cond.c_str());
    if (result.size() != 1)
    {
        dlog(LOG_ERR, "%s.%d AP %s HAVE NO WP STRATEGY.", __FUNC__, __LINE__, ap->hw_addr);
    }
    else
    {
        wp_config = result[0];
        SetValue(kv, STRING_WP_ENABLE, wp_config[DB_INDEX_WP_ENABLE]);
        SetValue(kv, STRING_WP_INTERVAL, wp_config[DB_INDEX_WP_INTERVAL]);
        SetValue(kv, STRING_WP_SERVER_IP_TYPE, toString(IP_TYPE_IPV4));
        SetValue(kv, STRING_WP_SERVER_IP_ADDR, wp_config[DB_INDEX_WP_SERVER_IP]);
        SetValue(kv, STRING_WP_SERVER_PORT, wp_config[DB_INDEX_WP_SERVER_PORT]);
        SetValue(kv, STRING_WP_SCAN_TYPE, wp_config[DB_INDEX_WP_SCAN_TYPE]);
        SetValue(kv, STRING_WP_CODE, wp_config[DB_INDEX_WP_CODE]);
        SetValue(kv, STRING_WP_PROTO, wp_config[DB_INDEX_WP_PROTO]);
        SetValue(kv, STRING_EF_ENABLE, wp_config[DB_INDEX_EF_ENABLE]);
        SetValue(kv, STRING_EF_INTERVAL, wp_config[DB_INDEX_EF_INTERVAL]);
        SetValue(kv, STRING_EF_SERVER_IP_TYPE, toString(IP_TYPE_IPV4));
        SetValue(kv, STRING_EF_SERVER_IP_ADDR, wp_config[DB_INDEX_EF_SERVER_IP]);
        SetValue(kv, STRING_EF_SCAN_TYPE, wp_config[DB_INDEX_EF_SCAN_TYPE]);
        SetValue(kv, STRING_EF_CODE, wp_config[DB_INDEX_EF_CODE]);
        SetValue(kv, STRING_EF_PROTO, wp_config[DB_INDEX_EF_PROTO]);
        SetValue(kv, STRING_WE_AD_INTERVAL, wp_config[DB_INDEX_WE_AD_INTERVAL]);
        SetValue(kv, STRING_WE_CHANNEL_2G, wp_config[DB_INDEX_WE_CHANNEL_2G]);
        SetValue(kv, STRING_WE_CHANNEL_5G, wp_config[DB_INDEX_WE_CHANNEL_5G]);
        SetValue(kv, STRING_WE_AD_RSSI, wp_config[DB_INDEX_WE_AD_RSSI]);
    }

    // TODO 频谱分析初始化
    // 终端数据收集不再下发，废弃

    cond = "";
    cond.append(DB_STRING_LAN_STRATEGY_NAME"='" +
                group_config[DB_INDEX_LAN_STRATEGY_NAME] + "'");
    DBI::Query(LAN_PORTAL_LIST, "*", result, cond.c_str());
    if (result.size() != 1)
    {
        dlog(LOG_ERR, "%s.%d AP %s HAVE NO LAN PORTAL STRATEGY.",
             __FUNC__, __LINE__, ap->hw_addr);
    }
    else
    {
        lan_portal_config = result[0];
        SetValue(kv, STRING_LAN_PORTAL_ENABLE, lan_portal_config[DB_INDEX_LAN_PORTAL_ENABLE]);
        SetValue(kv, STRING_LAN_PORTAL_URL, lan_portal_config[DB_INDEX_LAN_PORTAL_URL]);
    }

    cond = "";
    cond.append(DB_STRING_RFG_STRATEGY_NAME"='" +
                group_config[DB_INDEX_RFG_STRATEGY_NAME] + "'");
    DBI::Query(RFG_LIST, "*", result, cond.c_str());
    if (result.size() != 1)
    {
        dlog(LOG_ERR, "%s.%d AP %s HAVE NO RFG STRATEGY.",
             __FUNC__, __LINE__, ap->hw_addr);
    }
    else
    {
        rfg_config = result[0];
        SetValue(kv, STRING_RFG_ENABLE, rfg_config[DB_INDEX_RFG_ENABLE]);
        SetValue(kv, STRING_RFG_ASSOCMAX, rfg_config[DB_INDEX_RFG_ASSOC_MAX]);
        SetValue(kv, STRING_RFG_TIMEOUT, rfg_config[DB_INDEX_RFG_TIMEOUT]);
        SetValue(kv, STRING_RFG_MAXSTA, rfg_config[DB_INDEX_RFG_MAX_STA]);
        SetValue(kv, STRING_RFG_METHOD, rfg_config[DB_INDEX_RFG_METHOD]);
    }

    cond = "";
    cond.append(DB_STRING_RATE_SET_STRATEGY_NAME"='" +
                group_config[DB_INDEX_RATE_SET_STRATEGY_NAME] + "'");
    DBI::Query(RATE_SET_LIST, "*", result, cond.c_str());
    if (result.size() != 1)
    {
        dlog(LOG_ERR, "%s.%d AP %s HAVE NO RATE SET STRATEGY.",
            __FUNC__, __LINE__, ap->hw_addr);
    }
    else
    {
        rate_set_config = result[0];
        SetValue(kv, STRING_RATE_SET_11A_LEN, toString(1));
        SetValue(kv, STRING_RATE_SET_11BG_LEN, toString(2));
        SetValue(kv, STRING_RATE_SET_11N_LEN, toString(3));
        SetValue(kv, STRING_RATE_SET_11AC_LEN, toString(0));
        SetValue(kv, STRING_RATE_SET_11A_RATE, rate_set_config[DB_INDEX_RATE_SET_11A]);
        SetValue(kv, STRING_RATE_SET_11BG_RATE, rate_set_config[DB_INDEX_RATE_SET_11BG]);
        SetValue(kv, STRING_RATE_SET_11N_RATE, rate_set_config[DB_INDEX_RATE_SET_11N]);
        SetValue(kv, STRING_RATE_SET_11AC_RATE, rate_set_config[DB_INDEX_RATE_SET_11AC]);
    }

    SetValue(kv, STRING_CONNECTION_MODE, toString(1));
    SetValue(kv, STRING_TIME_STAMP, toString(time(NULL)));

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
    preq->pay_load.portal_custom.setValid(true);
    // TODO 此处需知道定制的个数
    // preq->pay_load.portal_custom.portal_customs.resize();
    // 每个CPortalCustom置为true
    preq->pay_load.time_stamp.setValid(true);   // Y
    preq->pay_load.by_pass.setValid(true);      // Y

    preq->LoadFrom(kv);
    buffer.extend();
    preq->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(preq);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_init_wlan_config(struct ap_dev* ap)
{
    CCapwapWlanConfigReq *preq = NULL;
    string cond;
    vector<vector<string> > wlan_2g_config;
    vector<vector<string> > wlan_5g_config;
    size_t wlan_count;
    DBResult result;
    kvlist kv;
    CBuffer buffer;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    cond.append(DB_STRING_WLAN_STRATE_NAME"=(");
    cond.append("select "DB_STRING_WLAN_2G_STRATEGY_NAME" from "GROUP_LIST
                " where "DB_STRING_GROUP_NAME"=(");
    cond.append("select "DB_STRING_GROUP_NAME" from "AP_LIST
                " where "DB_STRING_AP_MAC"='" + toString(ap->hw_addr) + "'))");
    DBI::Query(WLAN_LIST, "*", result, cond.c_str());
    if (0 == result.size())
    {
        dlog(LOG_ERR, "%s.%d AP %s GET WLAN 2G ERROR.", __FUNC__, __LINE__, ap->hw_addr);
        return BUSINESS_FAIL;
    }
    wlan_2g_config = result;

    cond = "";
    cond.append(DB_STRING_WLAN_STRATE_NAME"=(");
    cond.append("select "DB_STRING_WLAN_5G_STRATEGY_NAME" from "GROUP_LIST
                " where "DB_STRING_GROUP_NAME"=(");
    cond.append("select "DB_STRING_GROUP_NAME" from "AP_LIST
                " where "DB_STRING_AP_MAC"='" + toString(ap->hw_addr) + "'))");
    DBI::Query(WLAN_LIST, "*", result, cond.c_str());
    if (0 == result.size())
    {
        dlog(LOG_ERR, "%s.%d AP %s GET WLAN 5G ERROR.", __FUNC__, __LINE__, ap->hw_addr);
        return BUSINESS_FAIL;
    }
    wlan_5g_config = result;

    wlan_count = wlan_2g_config.size() + wlan_5g_config.size();

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

    // TODO 设置wlan所需要的参数 后缀以wlan序号为准 如：radio_id1=1,表示第二个wlan的radio为2G
    for (size_t i=0; i<wlan_count; i++)
    {
        vector<string> wlan;
        int radio_id;
        string wlan_secure_strategy;
        vector<string> wlan_secure;

        if (wlan_2g_config.size() <= i)
        {
            wlan = wlan_5g_config[i-wlan_2g_config.size()];
            radio_id = 2;
        }
        else
        {
            wlan = wlan_2g_config[i];
            radio_id = 1;
        }
        wlan_secure_strategy = wlan[DB_INDEX_WLAN_SECURE_STRATEGY];

        SetValue(kv, STRING_RADIO_ID + toString(i), toString(radio_id));
        SetValue(kv, STRING_ESSID + toString(i), wlan[DB_INDEX_WLAN_ESSID]);
        SetValue(kv, STRING_ESSID_ENCODE + toString(i),wlan[DB_INDEX_WLAN_ESSID_ENCODE]);
        // TODO 获取加密信息下发
        cond = DB_STRING_WLAN_SECURE_STRATEGY"='" + wlan_secure_strategy + "'";
        DBI::Query(WLAN_SECURE_LIST, "*", result, cond.c_str());
        if (result.size() != 1)
        {
            dlog(LOG_ERR, "%s.%d wlan %s GET WLAN SECURE ERROR.",
                 __FUNC__, __LINE__, wlan[DB_INDEX_WLAN_ESSID].c_str());
        }
        else
        {
            wlan_secure = result[0];
            SetValue(kv, STRING_SECURE_TYPE + toString(i),
                     wlan_secure[DB_INDEX_WLAN_SECURE_TYPE]);
            SetValue(kv, STRING_KEY_INDEX + toString(i),
                     wlan_secure[DB_INDEX_WLAN_SECURE_KEY_INDEX]);
            // SetValue(kv, STRING_WLAN_KEY_TYPE  + toString(i), wlan_secure[DB_INDEX_WLAN_SECURE_KEY_TYPE]);
            SetValue(kv, STRING_KEY_LENGTH + toString(i), wlan_secure[DB_INDEX_WLAN_SECURE_KEY_LENGTH]);
            SetValue(kv, STRING_KEY + toString(i), wlan_secure[DB_INDEX_WLAN_SECURE_KEY]);
        }

        SetValue(kv, STRING_PORTAL_ENABLE + toString(i), !!(toInt(wlan[DB_INDEX_WLAN_AUTH_TYPE]) & AUTH_TYPE_PORTALAUTH));
        SetValue(kv, STRING_WX_AUTH_ENABLE + toString(i), !!(toInt(wlan[DB_INDEX_WLAN_AUTH_TYPE]) & AUTH_TYPE_WXAUTH));
        SetValue(kv, STRING_PORTAL_URL + toString(i), wlan[DB_INDEX_WLAN_PORTAL_URL]);
        // TODO 获取微信号中的图文链接下发为微信url
        SetValue(kv, STRING_HIDE_SSID + toString(i), wlan[DB_INDEX_WLAN_HIDE_SSID]);
        SetValue(kv, STRING_WDS_ENABLE + toString(i), wlan[DB_INDEX_WLAN_WDS_ENABLE]);
        SetValue(kv, STRING_VLAN_ID + toString(i), wlan[DB_INDEX_WLAN_VLAN_ID]);
        SetValue(kv, STRING_WLAN_ID + toString(i), wlan[DB_INDEX_WLAN_WLAN_ID]);
        SetValue(kv, STRING_MAX_STATIONS + toString(i), wlan[DB_INDEX_WLAN_MAX_USER]);
        SetValue(kv, STRING_SSID_DOWN + toString(i), wlan[DB_INDEX_WLAN_SSID_DOWN]);
        SetValue(kv, STRING_SSID_UP + toString(i), wlan[DB_INDEX_WLAN_SSID_UP]);
        SetValue(kv, STRING_USER_DOWN + toString(i), wlan[DB_INDEX_WLAN_USER_DOWN]);
        SetValue(kv, STRING_USER_UP + toString(i), wlan[DB_INDEX_WLAN_USER_UP]);
        SetValue(kv, STRING_QOS_ENABLE + toString(i), wlan[DB_INDEX_WLAN_QOS_ENABLE]);
        SetValue(kv, STRING_TUNNEL_ENABLE + toString(i), wlan[DB_INDEX_WLAN_TUNNEL_ENABLE]);
        SetValue(kv, STRING_STA_ISOLATE + toString(i), wlan[DB_INDEX_WLAN_USER_ISOLATE]);
        SetValue(kv, STRING_BROADCAST_TO_UNICAST_ENABLE + toString(i), wlan[DB_INDEX_WLAN_BROADCAST_TO_UNICAST_ENABLE]);
        SetValue(kv, STRING_BROADCAST_TO_UNICAST_MAX_STA + toString(i), wlan[DB_INDEX_WLAN_BROADCAST_TO_UNICAST_MAX_STA]);
        SetValue(kv, STRING_BROADCAST_TO_UNICAST_TIMEOUT + toString(i), wlan[DB_INDEX_WLAN_BROADCAST_TO_UNICAST_TIMEOUT]);
    }

    DumpKv(kv);

    preq->LoadFrom(kv);
    buffer.extend(102400);
    preq->Assemble(buffer);

    ap->cl->write_cb(ap->cl, (char*)buffer.GetBuffer(), buffer.GetOffset());

    SAFE_DELETE(preq);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_ap_config_rsp(struct ap_dev* ap)
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    return BUSINESS_SUCCESS;
}
int CBusiness::business_wlan_config_rsp(struct ap_dev* ap)
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    return BUSINESS_SUCCESS;
}

int CBusiness::business_add_station(struct ap_dev *ap)
{
    kvlist kv = ParseString(src_info);

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    ubus_add_station(kv);

    // TODO 发送ubus消息到auth
    return BUSINESS_SUCCESS;
}
int CBusiness::business_multi_add_station(struct ap_dev *ap)
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    // TODO 发送ubus消息到auth


    return BUSINESS_SUCCESS;
}
int CBusiness::business_del_station(struct ap_dev *ap)
{
    kvlist kv = ParseString(src_info);
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    ubus_del_station(kv);

    return BUSINESS_SUCCESS;
}

int CBusiness::business_wtp_event_req(struct ap_dev* ap)
{
    CCapwapWTPEventReq *preq = (CCapwapWTPEventReq *)packet();
    CCapwapWTPEventRsp *prsp = NULL;
    CBuffer buffer;

    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());

    if (preq == NULL)
        return BUSINESS_FAIL;

    if (preq->add_station.isValid())
        business_add_station(ap);
    else if (preq->del_station.isValid())
        business_del_station(ap);
    else if (preq->pay_load.sta_state.isValid())
        business_multi_add_station(ap);
    else if (preq->pay_load.actl_sta_info.isValid())
        business_multi_process_station(ap);
    else if (preq->pay_load.actl_user_info.isValid())
        business_user_info(ap);
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

int CBusiness::business_ap_info_req(struct ap_dev* ap)
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

int CBusiness::business_ap_leave(struct ap_dev *ap)
{
    string cond;
    string key_value;

    dlog(LOG_DEBUG, "%s.%d {AP %s %s %s:%d}", __FUNC__, __LINE__,
         ap->hw_addr, ap->lan_ip, ap->wan_ip, ntohs(ap->cl->peer_addr.sin_port));

    key_value.append(DB_STRING_LEAVE_TIME"=" + toString(time(NULL)));
    cond.append(DB_STRING_AP_MAC"='" + toString(ap->hw_addr) + "'");

    DBI::Update(AP_LIST, key_value.c_str(), cond.c_str());

    del_ap_dev(ap);

    return BUSINESS_SUCCESS;
}

int CBusiness::business_notify_status(struct ap_dev *ap)
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

int CBusiness::business_multi_process_station(struct ap_dev *ap)
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

    // TODO 发送ubus消息到auth
    return BUSINESS_SUCCESS;
}

int CBusiness::business_config_update_rsp(struct ap_dev *ap)
{
    dlog(LOG_DEBUG, "%s.%d {%s}", __FUNC__, __LINE__, src_info.c_str());
    return BUSINESS_SUCCESS;
}

int CBusiness::business_user_info(struct ap_dev *ap)
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
