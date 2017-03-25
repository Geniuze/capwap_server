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

    VS_ELEMENT_ECHO_INFO_TLV = 212,
    VS_ELEMENT_TRAFFIC_STATICS = 213,
    VS_ELEMENT_WX_AUTH_INFO = 214,
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

enum {
    IMAGE_ID_DATA_DEV_MODEL = 0,
    IMAGE_ID_DATA_SOFTWARE_VERSION,
    IMAGE_ID_DATA_FILE_NAME,
    IMAGE_ID_DATA_FILE_SERVER,
    IMAGE_ID_DATA_DOWNLOAD_TYPE,
    IMAGE_ID_DATA_FTP_USER_NAME,
    IMAGE_ID_DATA_FTP_PASSWORD,
    IMAGE_ID_DATA_FTP_PATH,
};

enum
{
	IEEE80211_RSN = 48,
	IEEE80211_WPA = 221,	//0xdd
};

enum
{
    IP_TYPE_IPV4 = 4,
    IP_TYPE_IPV6 = 6,
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

class CMiniElement : public CElementUtil
{
public:
    uint8_t _etype;
    uint8_t _elength;
public:
    CMiniElement() { _etype = 0; _elength = 0; }
    ~CMiniElement(){}

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
    void set_element_type(uint8_t type)
    {
        _etype = type;
    }
    int skipEH(CBuffer &buffer);
    int ReAssembleEH(CBuffer &buffer);
};

class CDstAddr
{
public:
    uint32_t addr_len;
    string addr;
    CDstAddr()
    {
        addr_len = 0;
    }
    ~CDstAddr(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv, string ex="");
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

class CImageIDDataDevModelTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataDevModelTlv()
    {
        set_element_type(IMAGE_ID_DATA_DEV_MODEL);
    }
    ~CImageIDDataDevModelTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CImageIDDataSoftwareVersionTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataSoftwareVersionTlv()
    {
        set_element_type(IMAGE_ID_DATA_SOFTWARE_VERSION);
    }
    ~CImageIDDataSoftwareVersionTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};
class CImageIDDataFileNameTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataFileNameTlv()
    {
        set_element_type(IMAGE_ID_DATA_FILE_NAME);
    }
    ~CImageIDDataFileNameTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CImageIDDataFileServerTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataFileServerTlv()
    {
        set_element_type(IMAGE_ID_DATA_FILE_SERVER);
    }
    ~CImageIDDataFileServerTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CImageIDDataDownloadTypeTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataDownloadTypeTlv()
    {
        set_element_type(IMAGE_ID_DATA_DOWNLOAD_TYPE);
    }
    ~CImageIDDataDownloadTypeTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CImageIDDataFtpUserNameTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataFtpUserNameTlv()
    {
        set_element_type(IMAGE_ID_DATA_FTP_USER_NAME);
    }
    ~CImageIDDataFtpUserNameTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CImageIDDataFtpPasswordTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataFtpPasswordTlv()
    {
        set_element_type(IMAGE_ID_DATA_FTP_PASSWORD);
    }
    ~CImageIDDataFtpPasswordTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CImageIDDataFtpPathTlv : public CElement
{
private:
    string data;
public:
    CImageIDDataFtpPathTlv()
    {
        set_element_type(IMAGE_ID_DATA_FTP_PATH);
    }
    ~CImageIDDataFtpPathTlv(){}

    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CWpaMiniTlv : public CMiniElement
{
private:
    uint32_t wpa_oui;
    uint16_t wpa_version;
    uint32_t wpa_multicast_oui;
    uint16_t wpa_unicast_num;
    uint32_t wpa_unicast_oui[5];
    uint16_t wpa_auth_num;
    uint32_t wpa_auth_oui[5];
public:
    CWpaMiniTlv()
    {
        set_element_type(IEEE80211_WPA);
        wpa_oui = 0;
        wpa_version = 0;
        wpa_multicast_oui = 0;
        wpa_unicast_num = 0;
        bzero(wpa_unicast_oui, sizeof(wpa_unicast_oui));
        wpa_auth_num = 0;
        bzero(wpa_auth_oui, sizeof(wpa_auth_oui));
    }
    ~CWpaMiniTlv(){}
};

class CRsnMiniTlv : public CMiniElement
{
private:
    uint16_t rsn_version;
	uint16_t rsn_group_cipher_oui16;
	uint8_t	 rsn_group_cipher_oui8;
	uint8_t	 rsn_group_cipher_type;
	uint16_t rsn_pairwise_cipher_cnt;
	uint32_t rsn_pairwise_cipher_oui[5];
	uint16_t rsn_authkey_mngmnt_cnt;
	uint32_t rsn_akmp_suite_oui[5];
	uint16_t rsn_capabilities;
public:
    CRsnMiniTlv()
    {
        set_element_type(IEEE80211_RSN);
        rsn_version = 0;
        rsn_group_cipher_oui16 = 0;
        rsn_group_cipher_oui8 = 0;
        rsn_group_cipher_type = 0;
        rsn_pairwise_cipher_cnt = 0;
        bzero(rsn_pairwise_cipher_oui, sizeof(rsn_pairwise_cipher_oui));
        rsn_authkey_mngmnt_cnt = 0;
        bzero(rsn_akmp_suite_oui, sizeof(rsn_akmp_suite_oui));
        rsn_capabilities = 0;
    }
    ~CRsnMiniTlv(){}
};

class CVSStationInfoReqTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t wlan_id;
    uint8_t sta_mac_len;
    uint8_t sta_mac[8];
public:
    CVSStationInfoReqTlv()
    {
        set_element_type(VS_ELEMENT_LSSUED_STAINFO_TLV);
        radio_id = 0;
        wlan_id = 0;
        sta_mac_len = 0;
        bzero(sta_mac, sizeof(sta_mac));
    }
    ~CVSStationInfoReqTlv(){}
};

class CVSActlUserInfoPktTlv : public CElement
{
private:
    uint32_t pkt_id;
    uint8_t  pkt_type;
    uint8_t  msg_type;
    uint8_t  sub_msg_type;
    uint8_t  sta_ip_type;
    uint8_t  sta_ip[16];
    uint8_t  sta_mac_len;
    uint8_t  sta_mac[8];
    uint8_t  ap_mac_len;
    uint8_t  ap_mac[8];
    uint32_t user_role_len;
    string   user_role;
    uint32_t session_time;
    uint8_t  radio_id;
    uint8_t  wlan_id;
public:
    CVSActlUserInfoPktTlv()
    {
        set_element_type(VS_ELEMENT_ACTL_USER_INFO_TLV);
        pkt_id = 0;
        pkt_type = 0;
        msg_type = 0;
        sub_msg_type = 0;
        sta_ip_type = 0;
        bzero(sta_ip, sizeof(sta_ip));
        sta_mac_len = 0;
        bzero(sta_mac, sizeof(sta_mac));
        ap_mac_len = 0;
        bzero(ap_mac, sizeof(ap_mac));
        user_role_len = 0;
        session_time = 0;
        radio_id = 0;
        wlan_id = 0;
    }
    ~CVSActlUserInfoPktTlv(){}
};

class CVSMcastEnhanceTlv : public CElement
{
private:
    uint8_t  radio_id;
    uint8_t  wlan_id;
    uint16_t broadcast_enable;
    uint16_t broadcast_max_sta;
    uint16_t broadcast_timeout;
public:
    CVSMcastEnhanceTlv()
    {
        set_element_type(VS_ELEMENT_CASTENHANCE_TLV);
        radio_id = 0;
        wlan_id = 0;
        broadcast_enable = 0;
        broadcast_max_sta = 0;
        broadcast_timeout = 0;
    }
    ~CVSMcastEnhanceTlv(){}
};

class CVSRadio11ACConfTlv : public CElement
{
private:
    uint8_t  radio_id;
    uint8_t  ldpc_enable;
    uint8_t  tx_enable;
    uint8_t  rx_enable;
    uint32_t work_rate_11ac;
public:
    CVSRadio11ACConfTlv()
    {
        set_element_type(VS_ELEMENT_RADIO_CONFIG_11AC);
        radio_id = 0;
        ldpc_enable = 0;
        tx_enable = 0;
        rx_enable = 0;
        work_rate_11ac = 0;
    }
    ~CVSRadio11ACConfTlv(){}
};

class CVSWxAuthInfoTlv : public CElement
{
private:
    // AP ---> AC
    u_int8_t	mac_len;
    u_int8_t	mac_addr[8];
    u_int8_t    sta_ip_type;
    u_int8_t    sta_ip_addr[16];
    u_int8_t    wlan_id;
    u_int8_t    radio_id;
    u_int8_t    apsn_len;
    string      apsn;
    u_int8_t    openid_len;
    string      openid;
    u_int8_t    publicid_len;
    string      publicid;
    u_int16_t   accesstoken_len;
    string      accesstoken;
    u_int8_t    essid_len;
    string      essid;

    // AC ---> AP
    u_int32_t wlanid;
    u_int32_t radioid;
    u_int32_t wxenable;
    u_int32_t urllen;
    string    wxurl;
public:
    CVSWxAuthInfoTlv()
    {
        set_element_type(VS_ELEMENT_WX_AUTH_INFO);
        mac_len = 0;
        bzero(mac_addr, sizeof(mac_addr));
        sta_ip_type = 0;
        bzero(sta_ip_addr, sizeof(sta_ip_addr));
        wlan_id = 0;
        radio_id = 0;
        apsn_len = 0;
        openid_len = 0;
        publicid_len = 0;
        accesstoken_len = 0;
        essid_len = 0;

        wlanid = 0;
        radioid = 0;
        wxenable = 0;
        urllen = 0;
    }
    ~CVSWxAuthInfoTlv(){}
};

class CVSWlanInfoTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t wlan_id;
    uint8_t wds_enable;
public:
    CVSWlanInfoTlv()
    {
        set_element_type(VS_ELEMENT_WLAN_INFO_TLV);
        radio_id = 0;
        wlan_id = 0;
        wds_enable = 0;
    }
    ~CVSWlanInfoTlv(){}
};

class CVSReverseSSHTlv : public CElement
{
private:
    uint8_t  ssh_enable;
    uint32_t listen_port;
    uint32_t data_port;
    uint32_t server_port;
    uint16_t addr_len;
    string   addr;
public:
    CVSReverseSSHTlv()
    {
        set_element_type(VS_ELEMENT_SSH_TLV);
        ssh_enable = 0;
        listen_port = 0;
        data_port = 0;
        server_port = 0;
        addr_len = 0;
    }
    ~CVSReverseSSHTlv(){}
};

class CVS8021xConfTlv : public CElement
{
private:
    uint32_t auth_server_ip;
    uint32_t auth_server_port;
    uint8_t  auth_key_len;
    string   auth_shared_key;
    uint32_t acc_server_ip;
    uint32_t acc_server_port;
    uint8_t  acc_key_len;
    string   acc_key;
    uint8_t  uas_id[16];
    uint32_t nas_ip;
    uint8_t  opaque[32];
public:
    CVS8021xConfTlv()
    {
        set_element_type(VS_ELEMENT_8021X_TLV);
        auth_server_ip = 0;
        auth_server_port = 0;
        auth_key_len = 0;
        acc_server_ip = 0;
        acc_server_port = 0;
        acc_key_len = 0;
        bzero(uas_id, sizeof(uas_id));
        nas_ip = 0;
        bzero(opaque, sizeof(opaque));
    }
    ~CVS8021xConfTlv(){}
};

class CVSSrcIPConfTlv : public CElement
{
private:
    uint32_t count;
    vector<CDstAddr> src_ips;
public:
    CVSSrcIPConfTlv()
    {
        set_element_type(VS_ELEMENT_SRC_IP_TLV);
        count = 0;
    }
    ~CVSSrcIPConfTlv(){}
};

class CVSNtpServerConfTlv : public CElement
{
private:
    uint16_t interval;
    uint16_t ntp_server_len;
    string   server;
public:
    CVSNtpServerConfTlv()
    {
        set_element_type(VS_ELEMENT_NTP_SERVER);
        interval = 0;
        ntp_server_len = 0;
    }
    ~CVSNtpServerConfTlv(){}
    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CPortalCustom : public CElement
{
private:
    uint16_t key_len;
    string   key;
    uint16_t alias_len;
    string   alias;
    uint16_t value_len;
    string   value;
public:
    CPortalCustom()
    {
        key_len = 0;
        alias_len = 0;
        value_len = 0;
    }
    ~CPortalCustom(){}
    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSPortalCustomConfTlv : public CElement
{
private:
    uint32_t count;
public:
    vector<CPortalCustom> portal_customs;
public:
    CVSPortalCustomConfTlv()
    {
        set_element_type(VS_ELEMENT_PORTAL_PARAM);
        count = 0;
    }
    ~CVSPortalCustomConfTlv(){}
    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class  CVSTimeStampConfTlv : public CElement
{
private:
    uint32_t time_stamp;
public:
    CVSTimeStampConfTlv()
    {
        set_element_type(VS_ELEMENT_TIMESTAMP_TLV);
        time_stamp = 0;
    }
    ~CVSTimeStampConfTlv(){}
    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CVSByPassConfTlv : public CElement
{
private:
    uint8_t by_pass;
public:
    CVSByPassConfTlv()
    {
        set_element_type(VS_ELEMENT_BY_PASS_TLV);
        by_pass = 0;
    }
    ~CVSByPassConfTlv(){}
    int Assemble(CBuffer &buffer);
    int LoadFrom(kvlist &kv, string ex);
};

class CAccessControl
{
private:
    uint32_t name_len;
    string   name;
    uint32_t access_strategy_len;
    string   access_strategy;
    uint32_t space_len;
    string   space;
    uint32_t time_len;
    string   time;
public:
    CAccessControl()
    {
        name_len = 0;
        access_strategy_len = 0;
        space_len = 0;
        time_len = 0;
    }
    ~CAccessControl(){}
};

class CVSAddAccessControlTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessControl> access_controls;
public:
    CVSAddAccessControlTlv()
    {
        set_element_type(VS_ADD_ELEMENT_ACCESS_CONTROL);
        count = 0;
    }
    ~CVSAddAccessControlTlv(){}
};

class CVSModifyAccessControlTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessControl> access_controls;
public:
    CVSModifyAccessControlTlv()
    {
        set_element_type(VS_UPDATE_ELEMENT_ACCESS_CONTROL);
        count = 0;
    }
    ~CVSModifyAccessControlTlv(){}
};

class CVSDelAccessControlTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessControl> access_controls;
public:
    CVSDelAccessControlTlv()
    {
        set_element_type(VS_DEL_ELEMENT_ACCESS_CONTROL);
        count = 0;
    }
    ~CVSDelAccessControlTlv(){}
};

class CAccessRule
{
private:
    uint32_t    dst_addr_len;
    string 	    dst_addr;
    uint32_t    src_addr_len;
    string      src_addr;
    uint32_t    dst_port_len;
    string      dst_port;
    uint32_t    src_port_len;
    string      src_port;
    uint32_t    name_len;
    string      name;
    uint32_t    protocol_len;
    string      protocol;
    uint32_t    action_len;
    string      action;
public:
    CAccessRule()
    {
        dst_addr_len = 0;
        src_addr_len = 0;
        dst_port_len = 0;
        src_port_len = 0;
        name_len = 0;
        protocol_len = 0;
        action_len = 0;
    }
    ~CAccessRule(){}
};

class CVSAddAccessRuleTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessRule> access_rules;
public:
    CVSAddAccessRuleTlv()
    {
        set_element_type(VS_ADD_ELEMENT_ACCESS_RULE);
        count = 0;
    }
    ~CVSAddAccessRuleTlv(){}
};

class CVSModifyAccessRuleTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessRule> access_rules;
public:
    CVSModifyAccessRuleTlv()
    {
        set_element_type(VS_UPDATE_ELEMENT_ACCESS_RULE);
        count = 0;
    }
    ~CVSModifyAccessRuleTlv(){}
};

class CVSDelAccessRuleTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessRule> access_rules;
public:
    CVSDelAccessRuleTlv()
    {
        set_element_type(VS_DEL_ELEMENT_ACCESS_RULE);
        count = 0;
    }
    ~CVSDelAccessRuleTlv(){}
};

class CAccessStrategy
{
public:

    uint32_t  down;
    uint32_t  up;
    uint32_t  name_len;
    string    name;
    uint32_t  rule_name_len;
    string    rule_name;
	CAccessStrategy()
	{
        down = 0;
        up = 0;
        rule_name = "";
        name_len = 0;
        rule_name_len = 0;
        name = "";
	}
	~CAccessStrategy(){}
};

class CVSAddAccessStrategyTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessStrategy> access_strategies;
public:
    CVSAddAccessStrategyTlv()
    {
        set_element_type(VS_ADD_ELEMENT_ACCESS_STRTEGY);
        count = 0;
    }
    ~CVSAddAccessStrategyTlv(){}
};

class CVSModifyAccessStrategyTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessStrategy> access_strategies;
public:
    CVSModifyAccessStrategyTlv()
    {
        set_element_type(VS_UPDATE_ELEMENT_ACCESS_STRTEGY);
        count = 0;
    }
    ~CVSModifyAccessStrategyTlv(){}
};

class CVSDelAccessStrategyTlv : public CElement
{
private:
    uint32_t count;
    vector<CAccessStrategy> access_strategies;
public:
    CVSDelAccessStrategyTlv()
    {
        set_element_type(VS_DEL_ELEMENT_ACCESS_STRTEGY);
        count = 0;
    }
    ~CVSDelAccessStrategyTlv(){}
};

class CSpaceConfig
{
public:
    uint32_t     name_len;
    string       name;
    uint32_t     ids_len;
    string       ids;
	CSpaceConfig()
	{
        name_len = 0;
        ids_len = 0;
	}
	~CSpaceConfig(){}
};

class CVSAddSpaceInfoTlv : public CElement
{
private:
    uint32_t count;
    vector<CSpaceConfig> space_confs;
public:
    CVSAddSpaceInfoTlv()
    {
        set_element_type(VS_ADD_ELEMENT_SPACE_CONFIG);
        count = 0;
    }
    ~CVSAddSpaceInfoTlv(){}
};

class CVSModifySpaceInfoTlv : public CElement
{
private:
    uint32_t count;
    vector<CSpaceConfig> space_confs;
public:
    CVSModifySpaceInfoTlv()
    {
        set_element_type(VS_UPDATE_ELEMENT_SPACE_CONFIG);
        count = 0;
    }
    ~CVSModifySpaceInfoTlv(){}
};

class CVSDelSpaceInfoTlv : public CElement
{
private:
    uint32_t count;
    vector<CSpaceConfig> space_confs;
public:
    CVSDelSpaceInfoTlv()
    {
        set_element_type(VS_DEL_ELEMENT_SPACE_CONFIG);
        count = 0;
    }
    ~CVSDelSpaceInfoTlv(){}
};

class CTimeConfig
{
public:
    uint32_t nameLen;
    string name;
    uint8_t anyDate;
    uint8_t anyDay;
    uint8_t anyTime;
    uint16_t yearStart;
    uint16_t yearEnd;
    uint8_t monthStart;
    uint8_t monthEnd;
    uint8_t dayStart;
    uint8_t dayEnd;
    uint8_t hourStart;
    uint8_t hourEnd;
    uint8_t minuteStart;
    uint8_t minuteEnd;
    uint8_t secondStart;
    uint8_t secondEnd;

    uint8_t monday;
    uint8_t tuesday;
    uint8_t wednesday;
    uint8_t thursday;
    uint8_t friday;
    uint8_t saturday;
    uint8_t sunday;
    CTimeConfig()
    {
        nameLen = 0;
        name = "";
        anyDate = 0;
        anyDay = 0;
        anyTime = 0;
        yearStart = 0;
        yearEnd = 0;
        monthStart = 0;
        monthEnd = 0;
        dayStart = 0;
        dayEnd = 0;
        hourStart = 0;
        hourEnd = 0;
        minuteStart = 0;
        minuteEnd = 0;
        secondStart = 0;
        secondEnd = 0;

        monday = 0;
        tuesday = 0;
        wednesday = 0;
        thursday = 0;
        friday = 0;
        saturday = 0;
        sunday = 0;
    }
    ~CTimeConfig(){}
};

class CVSAddTimeConfTlv : public CElement
{
private:
    uint32_t count;
    vector<CTimeConfig> time_confs;
public:
    CVSAddTimeConfTlv()
    {
        set_element_type(VS_ADD_ELEMENT_TIME_CONFIG);
        count = 0;
    }
    ~CVSAddTimeConfTlv(){}
};

class CVSModifyTimeConfTlv : public CElement
{
private:
    uint32_t  count;
    vector<CTimeConfig> time_confs;
public:
    CVSModifyTimeConfTlv()
    {
        set_element_type(VS_UPDATE_ELEMENT_TIME_CONFIG);
        count = 0;
    }
    ~CVSModifyTimeConfTlv(){}
};

class CVSDelTimeConfTlv : public CElement
{
private:
    uint32_t count;
    vector<CTimeConfig> time_confs;
public:
    CVSDelTimeConfTlv()
    {
        set_element_type(VS_DEL_ELEMENT_TIME_CONFIG);
        count = 0;
    }
    ~CVSDelTimeConfTlv(){}
};

class CUserRole
{
public:
    uint32_t   nameLen;
    string        name;
    uint32_t   accessControlLen;
    string        accessControl;
	CUserRole()
	{
        nameLen = 0;
        accessControlLen = 0;
        name = "";
        accessControl = "";
	}
    ~CUserRole(){}
};

class CVSAddUserRoleTlv : public CElement
{
private:
    uint32_t count;
    vector<CUserRole> user_roles;
public:
    CVSAddUserRoleTlv()
    {
        set_element_type(VS_ADD_ELEMENT_USER_ROLE);
        count = 0;
    }
    ~CVSAddUserRoleTlv(){}
};

class CVSModifyUserRoleTlv : public CElement
{
private:
    uint32_t count;
    vector<CUserRole> user_roles;
public:
    CVSModifyUserRoleTlv()
    {
        set_element_type(VS_UPDATE_ELEMENT_USER_ROLE);
        count = 0;
    }
    ~CVSModifyUserRoleTlv(){}
};

class CVSDelUserRoleTlv : public CElement
{
private:
    uint32_t count;
    vector<CUserRole> user_roles;
public:
    CVSDelUserRoleTlv()
    {
        set_element_type(VS_DEL_ELEMENT_USER_ROLE);
        count = 0;
    }
    ~CVSDelUserRoleTlv(){}
};

class CVSResultStrTlv : public CElement
{
private:
    string result;
public:
    CVSResultStrTlv()
    {
        set_element_type(VS_ELEMENT_RESULT_TLV);
    }
    ~CVSResultStrTlv(){}
};

class CVSStationIPAddrTlv : public CElement
{
private:
    uint8_t  ip_type;
    uint32_t ip_addr[4];
public:
    CVSStationIPAddrTlv()
    {
        set_element_type(VS_ELEMENT_IPCONF_TLV);
        ip_type = 0;
        bzero(ip_addr, sizeof(ip_addr));
    }
    ~CVSStationIPAddrTlv(){}
};

class CVSReportStaInfoByMacTlv : public CElement
{
private:
    uint8_t  mac_len;
    uint8_t  mac[8];
    uint64_t tx_bytes;
    uint64_t rx_bytes;
    uint64_t tx_pkts;
    uint64_t rx_pkts;
    uint32_t sta_rate;
public:
    CVSReportStaInfoByMacTlv()
    {
        set_element_type(VS_ELEMENT_REPORT_STAINFO_TLV);
        mac_len = 0;
        bzero(mac, sizeof(mac));
        tx_bytes = 0;
        rx_bytes = 0;
        tx_pkts = 0;
        rx_pkts = 0;
        sta_rate = 0;
    }
    ~CVSReportStaInfoByMacTlv(){}
};

class CActlStaInfo
{
private:
    uint32_t pkt_id;
    uint8_t  pkt_type;
    uint8_t  msg_type;
    uint32_t user_name_len;
    string   user_name;
    uint8_t  user_ip_type;
    uint8_t  user_ip[16];
    uint8_t  mac_len;
    uint8_t  mac[8];
    uint32_t acc_session_time;
    uint32_t tx_bytes_high;
    uint32_t tx_bytes;
    uint32_t rx_bytes_high;
    uint32_t rx_bytes;
    uint32_t tx_pkts;
    uint32_t rx_pkts;
    uint8_t  acc_down_cause;
    uint8_t  radio_id;
    uint8_t  wlan_id;

public:
    CActlStaInfo()
    {
        pkt_id = 0;
        pkt_type = 0;
        msg_type = 0;
        user_name_len = 0;
        user_ip_type = 0;
        bzero(user_ip, sizeof(user_ip));
        mac_len = 0;
        bzero(mac, sizeof(mac));
        acc_session_time = 0;
        tx_bytes_high = 0;
        tx_bytes = 0;
        rx_bytes_high = 0;
        rx_bytes = 0;
        tx_pkts = 0;
        rx_pkts = 0;
        acc_down_cause = 0;
        radio_id = 0;
        wlan_id = 0;
    }
    ~CActlStaInfo(){}
};

class CVSActlReportStaInfoTlv : public CElement
{
private:
    uint32_t count;
    vector<CActlStaInfo> sta_infos;
public:
    CVSActlReportStaInfoTlv()
    {
        set_element_type(VS_ELEMENT_ACTL_REPORT_STAINFO_TLV);
        count = 0;
    }
    ~CVSActlReportStaInfoTlv(){}
};

class CActlStaStateTlv : public CElement
{
private:
    uint32_t pkt_id;
    uint8_t  pkt_type;
    uint8_t  msg_type;
    uint8_t  sub_msg_type;
    uint8_t  user_ip_type;
    uint8_t  user_ip[16];
    uint8_t  sta_mac_len;
    uint8_t  sta_mac[8];
    uint8_t  ap_mac_len;
    uint8_t  ap_mac[8];
    uint32_t user_role_len;
    string   user_role;
    uint32_t session_time;
    uint8_t  radio_id;
    uint8_t  wlan_id;
    uint8_t  ssid_len;
    string   ssid;
public:
    CActlStaStateTlv()
    {
        set_element_type(VS_ELEMENT_ACTL_STA_STATE_TLV);
        pkt_id = 0;
        pkt_type = 0;
        msg_type = 0;
        sub_msg_type = 0;
        user_ip_type = 0;
        bzero(user_ip, sizeof(user_ip));
        sta_mac_len = 0;
        bzero(sta_mac, sizeof(sta_mac));
        ap_mac_len = 0;
        bzero(ap_mac, sizeof(ap_mac));
        user_role_len = 0;
        session_time = 0;
        radio_id = 0;
        wlan_id = 0;
        ssid_len = 0;
    }
    ~CActlStaStateTlv(){}
};

class CVSActlReportStaStateTlv : public CElement
{
private:
    uint8_t count;
    vector<CActlStaStateTlv> sta_states;
public:
    CVSActlReportStaStateTlv()
    {
        set_element_type(VS_ELEMENT_ACTL_REPORT_STASTATE_TLV);
        count = 0;
    }
    ~CVSActlReportStaStateTlv(){}
};

class CVSReportAPInfoTlv : public CElement
{
private:
    uint64_t tx_bytes;
    uint64_t rx_bytes;
    uint64_t tx_pkts;
    uint64_t rx_pkts;
    uint32_t count;
public:
    CVSReportAPInfoTlv()
    {
        set_element_type(VS_ELEMENT_REPORT_APINFO_TLV);
        tx_bytes = 0;
        rx_bytes = 0;
        tx_pkts = 0;
        rx_pkts = 0;
    }
    ~CVSReportAPInfoTlv(){}
};

class CVSStaAuthReqTlv : public CElement
{
private:
    uint8_t sta_mac[6];
    uint8_t sta_state;
public:
    CVSStaAuthReqTlv()
    {
        set_element_type(VS_ELEMENT_TERMINAL_AUTH_TLV);
        bzero(sta_mac, sizeof(sta_mac));
        sta_state = 0;
    }
    ~CVSStaAuthReqTlv(){}
};

class CVSAutoChannelSelectInfoTlv : public CElement
{
private:
    uint32_t identifier;
    uint8_t  type;
    uint8_t  ap_mac[6];
    uint32_t neighbor_num;
    uint8_t  ap_mac_one[6];
    uint32_t ap_rssi_one;
    uint8_t  ap_mac_two[6];
    uint32_t ap_rssi_two;
    uint32_t ap_rssi_channel_one;
    uint32_t ap_channel_one_num;
    uint32_t ap_rssi_channel_six;
    uint32_t ap_channel_six_num;
    uint32_t ap_rssi_channel_eleven;
    uint32_t ap_channel_eleven_num;
public:
    CVSAutoChannelSelectInfoTlv()
    {
        set_element_type(VS_ELEMENT_IMAGE_ACS_INFO);
        identifier = 0;
        type = 0;
        bzero(ap_mac, sizeof(ap_mac));
        neighbor_num = 0;
        bzero(ap_mac_one, sizeof(ap_mac_one));
        ap_rssi_one = 0;
        bzero(ap_mac_two, sizeof(ap_mac_two));
        ap_rssi_two = 0;
        ap_rssi_channel_one = 0;
        ap_channel_one_num = 0;
        ap_rssi_channel_six = 0;
        ap_channel_six_num = 0;
        ap_rssi_channel_eleven = 0;
        ap_channel_eleven_num = 0;
    }
    ~CVSAutoChannelSelectInfoTlv(){}
};



#endif
