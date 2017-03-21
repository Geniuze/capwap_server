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

    prsp = capwap_get_packet(CAPWAP_PACKET_TYPE_JOIN_RSP);
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

    ap->cl->write_cb(ap->cl, buffer.GetBuffer(), buffer.GetOffset());

    // 添加到ap列表
    add_ap_dev(ap);

    SAFE_DELETE(prsp);
    return BUSINESS_SUCCESS;
}

int CBusiness::business_configure_process(struct ap_dev *ap)
{
    // configure status req 的内容不需要处理，直接下发分组中的配置

    return BUSINESS_SUCCESS;
}
