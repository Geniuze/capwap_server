#ifndef _CAPWAP_VSELEMENT_H_
#define _CAPWAP_VSELEMENT_H_

#include "kvlist.h"

enum {
    VS_ELEMENT_RADIO_CONF = 0,
    VS_ELEMENT_VLAN_CONF = 1,
    VS_ELEMENT_RESULT_TLV = 2,
    VS_ELEMENT_IPCONF_TLV = 3,
    VS_ELEMENT_LSSUED_STAINFO_TLV = 4,	//下发获取sta信息请求
    VS_ELEMENT_REPORT_STAINFO_TLV = 5,	//上报sta信息
    VS_ELEMENT_APTRANS_CONF_TLV = 6,   //数据转发类型
    VS_ELEMENT_REPORT_APINFO_TLV = 7,   //ap的流量信息
    VS_ELEMENT_APLANIP_TLV = 8,               //跨网IP信息
    VS_ELEMENT_RADIUS_CONF_TLV = 9,   //radius信息
    VS_ELEMENT_AC_PRIORITY = 128,
    VS_ELEMENT_RADIO_CAP_TLV = 129,
    VS_ELEMENT_STATION_INFO = 130,
    VS_ELEMENT_EXT_SUPP_RATES = 142,
    VS_ELEMENT_RADIO_CONFIG_TLV = 147,
    VS_ELEMENT_PORTAL=148,
    VS_ELEMENT_STAROME_INFO_TLV = 149,

    VS_ELEMENT_ACTL_REPORT_STAINFO_TLV = 201,	//权限控制模块上报给RADIUS CLIENT 的STA信息
    VS_ELEMENT_ACTL_USER_INFO_TLV = 202,
    VS_ELEMENT_AP_SPACE_INFO_TLV = 203,
    VS_ELEMENT_ACTL_STA_STATE_TLV = 204,

    VS_ELEMENT_OUTSIDE_RADIUS_INFO = 211,
    VS_ELEMENT_ECHO_INFO_TLV = 212,
    VS_ELEMENT_TRAFFIC_STATICS = 213,
    VS_ELEMENT_WX_AUTH_INFO = 214,
    VS_ELEMENT_RFG_INFO_TLV = 215,
    VS_ELEMENT_PACKET_POWER_TLV = 216,

    VS_ELEMENT_WLAN_INFO_TLV = 217,
    VS_ELEMENT_TERMINAL_AUTH_TLV = 218,
    VS_ELEMENT_NTP_SERVER = 219,

    VS_ELEMENT_TIMESTAMP_TLV = 222,
    VS_ELEMENT_BY_PASS_TLV = 223,

    VS_ELEMENT_ABP_RADIO_CONFIG = 1108,
    VS_ELEMENT_RADIO_PROBE_CONFIG = 1109,
    VS_ELEMENT_RADIO_CONFIG_11AC = 1110,
    VS_ELEMENT_IMAGE_ACS_INFO = 1111,

    VS_ELEMENT_SSH_TLV  = 1117,

    VS_ELEMENT_8021X_TLV = 1120,


    VS_ELEMENT_PORTAL_PARAM = 20101,

    VS_ADD_ELEMENT_AUTH_CONTROL = 20030,
    VS_ADD_ELEMENT_ACCESS_CONTROL = 20031,
    VS_ADD_ELEMENT_ACCESS_RULE = 20032,
    VS_ADD_ELEMENT_ACCESS_STRTEGY = 20033,
    VS_ADD_ELEMENT_SPACE_CONFIG = 20034,
    VS_ADD_ELEMENT_TIME_CONFIG = 20035,
    VS_ADD_ELEMENT_USER_INFO = 20036,
    VS_ADD_ELEMENT_USER_ROLE = 20037,
    VS_ADD_ELEMENT_BLACK_LIST = 20038,
    VS_ADD_ELEMENT_WHITE_LIST = 20039,
    VS_DEL_ELEMENT_AUTH_CONTROL = 20040,
    VS_DEL_ELEMENT_ACCESS_CONTROL = 20041,
    VS_DEL_ELEMENT_ACCESS_RULE = 20042,
    VS_DEL_ELEMENT_ACCESS_STRTEGY = 20043,
    VS_DEL_ELEMENT_SPACE_CONFIG = 20044,
    VS_DEL_ELEMENT_TIME_CONFIG = 20045,
    VS_DEL_ELEMENT_USER_INFO = 20046,
    VS_DEL_ELEMENT_USER_ROLE = 20047,
    VS_DEL_ELEMENT_BLACK_LIST = 20048,
    VS_DEL_ELEMENT_WHITE_LIST = 20049,
    VS_UPDATE_ELEMENT_AUTH_CONTROL = 20050,
    VS_UPDATE_ELEMENT_ACCESS_CONTROL = 20051,
    VS_UPDATE_ELEMENT_ACCESS_RULE = 20052,
    VS_UPDATE_ELEMENT_ACCESS_STRTEGY = 20053,
    VS_UPDATE_ELEMENT_SPACE_CONFIG = 20054,
    VS_UPDATE_ELEMENT_TIME_CONFIG = 20055,
    VS_UPDATE_ELEMENT_USER_INFO = 20056,
    VS_UPDATE_ELEMENT_USER_ROLE = 20057,
    VS_UPDATE_ELEMENT_BLACK_LIST = 20058,
    VS_UPDATE_ELEMENT_WHITE_LIST = 20059,
    VS_ELEMENT_ACTL_REPORT_STASTATE_TLV = 20060,
    VS_ELEMENT_CHANNEL_REUSE_TLV = 20061,

    VS_ELEMENT_BLACKDOMAIN_RESET = 20085,

    VS_ELEMENT_SRC_IP_TLV = 20095,

    VS_ELEMENT_CASTENHANCE_TLV = 21001,
};

enum {
    WTP_BOARD_DATA_MODEL_NUMBER = 0,
    WTP_BOARD_DATA_SERIAL_NUMBER,
    WTP_BOARD_DATA_BOARD_ID,
    WTP_BOARD_DATA_BOARD_REVISION,
    WTP_BOARD_DATA_MAC_ADDRESS,
};

enum {
    WTP_DESCRIPTOR_HARDWARE_VERSION = 0,
    WTP_DESCRIPTOR_SOFTWARE_VERSION = 1,
};

class CBuffer;

class CElementUtil {
private:
    bool _valid;
public:
    CElementUtil(){ _valid = false; }
    ~CElementUtil(){}
    void setValid(bool valid)
    {
        _valid = valid;
    }
    bool isValid()
    {
        return _valid;
    }
};

class CElement : public CElementUtil
{
public:
    uint16_t _etype;
    uint16_t _elength;
public:
    CElement() { _etype = 0; _elength = 0; }
    ~CElement(){}
    int Parse(CBuffer &buffer) { return 0; }
    int Assemble(CBuffer &buffer) { return 0; }
    int SaveTo(string &str) { return 0; }
    int LoadFrom(kvlist &kv,string ex="") { return 0; }
    int ParseEH(CBuffer &buffer);
    int AssembleEH(CBuffer &buffer);

    int length()
    {
        return _elength;
    }
    void set_element_type(uint16_t type)
    {
        _etype = type;
    }

    int skipEH(CBuffer &buffer);
    int ReAssembleEH(CBuffer &buffer);
};

class CWTPModelNumber : public CElement
{
private:
    string value;
public:
    CWTPModelNumber() { set_element_type(WTP_BOARD_DATA_MODEL_NUMBER); }
    ~CWTPModelNumber(){}
    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
};

class CWTPSerialNumber : public CElement
{
private:
    string value;
public:
    CWTPSerialNumber(){ set_element_type(WTP_BOARD_DATA_SERIAL_NUMBER); }
    ~CWTPSerialNumber(){}
    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
};

class CWTPBoardId : public CElement
{
private:
    string value;
public:
    CWTPBoardId(){ set_element_type(WTP_BOARD_DATA_BOARD_ID); }
    ~CWTPBoardId(){}
    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
};

class CWTPBoardRevision :  public CElement
{
private:
    string value;
public:
    CWTPBoardRevision(){ set_element_type(WTP_BOARD_DATA_BOARD_REVISION); }
    ~CWTPBoardRevision(){}
    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
};

class CWTPBoardMacAddress :  public CElement
{
private:
    uint8_t macaddr[6];
public:
    CWTPBoardMacAddress(){ set_element_type(WTP_BOARD_DATA_MAC_ADDRESS); }
    ~CWTPBoardMacAddress(){}
    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
};

class CAPLanIPTlv : public CElement
{
public:
    CAPLanIPTlv(){ set_element_type(VS_ELEMENT_APLANIP_TLV); }
    ~CAPLanIPTlv(){}
    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
private:
    uint8_t ip_type;
    uint32_t ap_lanip;
    uint8_t ap_mac_len;
    uint8_t ap_mac[6];
};

class CAPTransConfTlv : public CElement
{
private:
    uint8_t trans_type;
public:
    CAPTransConfTlv()
    {
        set_element_type(VS_ELEMENT_APTRANS_CONF_TLV);
        trans_type = 0;
    }
    ~CAPTransConfTlv(){}
    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv,string ex="");
};

class CWTPDescriptorHardwareVersionTlv : public CElement
{
private:
    uint32_t wtp_descriptor_vendor;
    string value;
public:
    CWTPDescriptorHardwareVersionTlv()
    {
        set_element_type(WTP_DESCRIPTOR_HARDWARE_VERSION);
    }
    ~CWTPDescriptorHardwareVersionTlv(){}

    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
};

class CWTPDescriptorSoftwareVersionTlv : public CElement
{
private:
    uint32_t wtp_descriptor_vendor;
    string value;
public:
    CWTPDescriptorSoftwareVersionTlv()
    {
        set_element_type(WTP_DESCRIPTOR_SOFTWARE_VERSION);
    }
    ~CWTPDescriptorSoftwareVersionTlv(){}

    int Parse(CBuffer &buffer);
    int SaveTo(string &str);
};

class CVSRadioConfTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t amsdu;
    uint8_t ampdu;
    uint8_t radio_type_11nonly;
    uint8_t short_gi;
    uint8_t band_width;
    uint8_t max_support_mcs;
    uint8_t max_mandantory_mcs;
    uint8_t tx_antenna;
    uint8_t rx_antenna;

    uint8_t auto_channel_switch;
    uint16_t auto_channel_period;
    uint8_t auto_power_switch;
    uint8_t reserved[2];

public:
    CVSRadioConfTlv()
    {
        set_element_type(VS_ELEMENT_RADIO_CONFIG_TLV);
        radio_id = 0;
        amsdu = 0;
        ampdu = 0;
        radio_type_11nonly = 0;
        short_gi = 0;
        band_width = 0;
        max_support_mcs = 0;
        max_mandantory_mcs = 0;
        tx_antenna = 0;
        rx_antenna = 0;
        auto_channel_switch = 0;
        auto_channel_period = 0;
        auto_power_switch = 0;
        bzero(reserved, sizeof(reserved));
    }
    ~CVSRadioConfTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSAPSpaceInfoTlv : public CElement
{
private:
    uint32_t space_len;
    string space;
public:
    CVSAPSpaceInfoTlv()
    {
        set_element_type(VS_ELEMENT_AP_SPACE_INFO_TLV);
        space_len = 0;
    }
    ~CVSAPSpaceInfoTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSABPRadioConfTlv : public CElement
{
private:
    uint8_t radio_id;
    uint32_t ack_timeout;
    uint32_t beacon_rate;
    uint8_t protect_mode;
public:
    CVSABPRadioConfTlv()
    {
        set_element_type(VS_ELEMENT_ABP_RADIO_CONFIG);
        radio_id = 0;
        ack_timeout = 0;
        beacon_rate = 0;
        protect_mode = 0;
    }
    ~CVSABPRadioConfTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSRadioProbeConfTlv : public CElement
{
private:
    uint8_t radio_id;
    uint32_t probe_rssi;
    uint32_t min_contract_rate;
    uint32_t reserved[3];
public:
    CVSRadioProbeConfTlv()
    {
        set_element_type(VS_ELEMENT_RADIO_PROBE_CONFIG);
        radio_id = 0;
        probe_rssi = 0;
        min_contract_rate = 0;
        bzero(reserved, sizeof(reserved));
    }
    ~CVSRadioProbeConfTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSEchoConfTlv : public CElement
{
private:

    uint32_t echo_interval;
    uint32_t echo_timeout_cnt;

public:
    CVSEchoConfTlv()
    {
        set_element_type(VS_ELEMENT_ECHO_INFO_TLV);
        echo_interval = 0;
        echo_timeout_cnt = 0;
    }
    ~CVSEchoConfTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSTrafficStaticsTlv : public CElement
{
private:
    // AC ---> AP
    uint32_t ts_interval;
    uint32_t ts_switch;

    // AP ---> AC
    uint64_t tx_speed;
    uint64_t rx_speed;

public:
    CVSTrafficStaticsTlv()
    {
        set_element_type(VS_ELEMENT_TRAFFIC_STATICS);
        ts_interval = 0;
        ts_switch = 0;
        tx_speed = 0;
        rx_speed = 0;
    }
    ~CVSTrafficStaticsTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSNtpServerConfTlv : public CElement
{
private:
    uint16_t interval;
    uint16_t ntp_server_length;
    string ntp_server;
public:
    CVSNtpServerConfTlv()
    {
        set_element_type(VS_ELEMENT_NTP_SERVER);
        interval = 0;
    }
    ~CVSNtpServerConfTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSPacketPowerConfTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t min_power;
    uint32_t min_throughput_threshold;
    uint32_t min_rate_threshold;
    uint8_t ppc_enable;
public:
    CVSPacketPowerConfTlv()
    {
        set_element_type(VS_ELEMENT_PACKET_POWER_TLV);
        radio_id = 0;
        min_power = 0;
        min_throughput_threshold = 0;
        min_rate_threshold = 0;
        ppc_enable = 0;
    }
    ~CVSPacketPowerConfTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSChannelReuseConfTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t cca_adj;
    uint8_t cca_thr;
public:
    CVSChannelReuseConfTlv()
    {
        set_element_type(VS_ELEMENT_CHANNEL_REUSE_TLV);
        radio_id = 0;
        cca_adj = 0;
        cca_thr = 0;
    }
    ~CVSChannelReuseConfTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

#endif
