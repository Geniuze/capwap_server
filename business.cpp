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

int CBusiness::Process(struct ap_dev *ap)
{
    int ret = BUSINESS_FAIL;
    dlog(LOG_DEBUG, "%s.%d", __func__, __LINE__);
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
    default:
        dlog(LOG_ERR, "%s.%d unknown this business-type %d", __func__, __LINE__, business_type);
        break;
    }

    return ret;
}

int CBusiness::business_discovery_process(struct ap_dev *ap)
{
    if ("" == src_info)
    {
        dlog(LOG_ERR, "%s.%d Business need source information", __func__, __LINE__);
        return BUSINESS_FAIL;
    }

    CCapwapDiscoveryRsp *prsp = NULL;
    CBuffer buffer;
    string inter_addr = "";
    kvlist kv;

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
        ap_dev_load_from_db(ap, result[0]);
        break;
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

    for (uint32_t i=0; i<radio_config.size(); i++)
    {
        int radio_id = i+1;
        SetValue(kv, STRING_RADIO_ID + toString(radio_id), toString(radio_id));
        SetValue(kv, STRING_RADIO_STATE + toString(radio_id), radio_config[i][1]);
        SetValue(kv, STRING_RADIO_TYPE_11B + toString(radio_id),
                 toString((toInt(radio_config[i][2]) & RADIO_TYPE_11B)?1:0));
        SetValue(kv, STRING_RADIO_TYPE_11A + toString(radio_id),
                 toString((toInt(radio_config[i][2]) & RADIO_TYPE_11A)?1:0));
        SetValue(kv, STRING_RADIO_TYPE_11G + toString(radio_id),
                 toString((toInt(radio_config[i][2]) & RADIO_TYPE_11G)?1:0));
        SetValue(kv, STRING_RADIO_TYPE_11N + toString(radio_id),
                 toString((toInt(radio_config[i][2]) & RADIO_TYPE_11N)?1:0));
        SetValue(kv, STRING_CURRENT_CHANNEL + toString(radio_id), radio_config[i][3]);
        SetValue(kv, STRING_CURRENT_TXPOWER + toString(radio_id), radio_config[i][4]);
        SetValue(kv, STRING_TX_ANTENNA + toString(radio_id), radio_config[i][5]);
        SetValue(kv, STRING_RX_ANTENNA + toString(radio_id), radio_config[i][5]);
        SetValue(kv, STRING_BAND_WIDTH + toString(radio_id), radio_config[i][6]);
        SetValue(kv, STRING_RTS_THRESHOLD + toString(radio_id), radio_config[i][7]);
        SetValue(kv, STRING_FRAG_THR + toString(radio_id), radio_config[i][8]);
        SetValue(kv, STRING_BEACON_PERIOD + toString(radio_id), radio_config[i][9]);
        SetValue(kv, STRING_ACK_TIMEOUT + toString(radio_id), radio_config[i][10]);
        SetValue(kv, STRING_MIN_CONTRACT_RATE + toString(radio_id), radio_config[i][11]);
        SetValue(kv, STRING_BEACON_RATE + toString(radio_id), radio_config[i][12]);
        SetValue(kv, STRING_PROTECT_MODE + toString(radio_id), radio_config[i][13]);
        SetValue(kv, STRING_PROBE_RSSI + toString(radio_id), radio_config[i][14]);
        SetValue(kv, STRING_DTIM_PERIOD + toString(radio_id), radio_config[i][15]);
        SetValue(kv, STRING_CCA_ADJ + toString(radio_id), radio_config[i][16]);
        SetValue(kv, STRING_CCA_THR + toString(radio_id), radio_config[i][17]);
        SetValue(kv, STRING_A_MPDU + toString(radio_id), radio_config[i][18]);
        SetValue(kv, STRING_A_MSDU + toString(radio_id), radio_config[i][19]);
        SetValue(kv, STRING_SHORT_GI + toString(radio_id), radio_config[i][20]);
        SetValue(kv, STRING_COUNTRY_STRING + toString(radio_id), radio_config[i][21]);
        SetValue(kv, STRING_11N_ONLY + toString(radio_id), radio_config[i][22]);
        SetValue(kv, STRING_SHORT_PREAMBLE + toString(radio_id), radio_config[i][23]);
        SetValue(kv, STRING_AUTO_CHANNEL_SWITCH + toString(radio_id), radio_config[i][24]);
        SetValue(kv, STRING_AUTO_POWER_SWITCH + toString(radio_id), radio_config[i][25]);
        SetValue(kv, STRING_PPC_ENABLE + toString(radio_id), radio_config[i][26]);
        SetValue(kv, STRING_MIN_POWER + toString(radio_id), radio_config[i][27]);
        SetValue(kv, STRING_MIN_THROUGHPUT_THRESHOLD + toString(radio_id), radio_config[i][28]);
        SetValue(kv, STRING_MIN_RATE_THRESHOLD + toString(radio_id), radio_config[i][29]);
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

    dlog(LOG_DEBUG, "%s.%d", __func__, __LINE__);
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

    dlog(LOG_DEBUG, "%s.%d", __func__, __LINE__);
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

    dlog(LOG_DEBUG, "%s.%d ", __func__, __LINE__);
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
    dlog(LOG_DEBUG, "%s.%d", __func__, __LINE__);

    if (BUSINESS_SUCCESS != business_init_ap_config(ap))
    {
        dlog(LOG_ERR, "%s.%d init ap config error", __func__, __LINE__);
    }

    return BUSINESS_SUCCESS;
}

int CBusiness::business_init_ap_config(struct ap_dev* ap)
{
    CCapwapAPConfReq *preq = NULL;
    CBuffer buffer;

    preq = (CCapwapAPConfReq *)capwap_get_packet(CAPWAP_PACKET_TYPE_AP_CONFIG_REQ);
    if (NULL == preq)
        return BUSINESS_FAIL;

    // preq->outside_auth_conf.setValid(true); //小AC不支持外部radius配置
    // preq->nat_dhcp_conf.setValid(true); //小AC不支持配置DHCP及NAT
    preq->report_station_enable.setValid(true);
    preq->roming_conf.setValid(true);
    preq->wp_conf.setValid(true);
    preq->rfg_conf.setValid(true);
    preq->ap_lb_conf.setValid(true);
    preq->rate_set_conf.setValid(true);
    preq->low_rssi_conf.setValid(true);
    preq->connection_mode_conf.setValid(true);
    preq->sta_data_collection_conf.setValid(true);
    preq->lan_vlan_conf.setValid(true);
    preq->report_station_interval.setValid(true);
    preq->audit_appri_conf.setValid(true);
    preq->lan_portal_conf.setValid(true);
    preq->pay_load.setValid(true);
    preq->pay_load.portal_custom.setValid(true);
    preq->pay_load.time_stamp.setValid(true);

    return BUSINESS_SUCCESS;
}
