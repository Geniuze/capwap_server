
#ifndef _CAPWAP_ELEMENTS_H_
#define _CAPWAP_ELEMENTS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "capwap_vselement.h"

#define DUNCHONG_IDENTIFY 0x9cbb

typedef enum
{
    ELEMENT_AC_DESCRIPTOR = 1,
    ELEMENT_AC_IPv4_LIST = 2,
    ELEMENT_AC_IPv6_LIST = 3,
    ELEMENT_AC_NAME = 4,
    ELEMENT_AC_NAME_WITH_PRIO = 5,
    ELEMENT_AC_TIMESTAMP = 6,
    ELEMENT_ADD_MAC_ACL = 7,
    ELEMENT_ADD_STATION = 8,
    ELEMENT_IPv4_ADDR = 10,
    ELEMENT_IPv6_ADDR = 11,
    ELEMENT_CAPWAP_TIMERS = 12,
    ELEMENT_DATA_TRANS_DATA = 13,
    ELEMENT_DATA_TRANS_MODE = 14,
    ELEMENT_DECRY_ERR_RPT_PERIOD = 16,
    ELEMENT_DEL_MAC_ACL = 17,
    ELEMENT_DEL_STATION = 18,
    ELEMENT_DISCOVERY_TYPE = 20,
    ELEMENT_IDLE_TIMEOUT = 23,
    ELEMENT_IMAGE_DATA = 24,
    ELEMENT_IMAGE_IDENTIFIER = 25,
    ELEMENT_IMAGE_INFO = 26,
    ELEMENT_INIT_DOWNLOAD = 27,
    ELEMENT_LOCATION_DATA = 28,
    ELEMENT_LOCAL_IPv4_ADDR = 30,
    ELEMENT_RADIO_ADMIN_STATE = 31,
    ELEMENT_RADIO_OPT_STATE = 32,
    ELEMENT_RESULT_CODE = 33,
    ELEMENT_SESSION_ID = 35,
    ELEMENT_STATISTICS_TIMER = 36,
    ELEMENT_VENDOR_SPEC_PAYLOAD = 37,
    ELEMENT_WTP_BOARD_DATA = 38,
    ELEMENT_WTP_DESCRIPTOR = 39,
    ELEMENT_WTP_FRAME_TUNNEL_MODE = 41,
    ELEMENT_WTP_MAC_TYPE = 44,
    ELEMENT_WTP_NAME = 45,
    ELEMENT_WTP_REBOOT_STATISTICS = 48,
    ELEMENT_WTP_STATIC_ADDR = 49,
    ELEMENT_ECN_SUPPORT = 53,		//移动标准已不使用

    ELEMENT_OUTSIDE_RADIUS_INFO = 211,
    ELEMENT_RFG_INFO_TLV = 215,

    ELEMENT_ADD_WLAN = 1024,
    ELEMENT_ANTENNA = 1025,
    ELEMENT_ASSIGNED_AP_BSSID = 1026,
    ELEMENT_DEL_WLAN = 1027,
    ELEMENT_DIRECT_SEQU_CTL = 1028,
    ELEMENT_INFO_ELEMENT = 1029,
    ELEMENT_MAC_OPERATION = 1030,
    ELEMENT_MULTI_DOMAIN_CAP = 1032,
    ELEMENT_OFDM_CTL = 1033,
    ELEMENT_RATE_SET = 1034,
    ELEMENT_STATION = 1036,
    ELEMENT_STATION_QoS_PROFILE = 1037,
    ELEMENT_STATION_SESSION_KEY = 1038,
    ELEMENT_SUPP_RATES = 1040,
    ELEMENT_TXPOWER = 1041,
    ELEMENT_UPDATE_WLAN = 1044,
    ELEMENT_WTP_QoS = 1045,
    ELEMENT_WTP_RADIO_CONFIG = 1046,
    ELEMENT_WTP_RADIO_INFO = 1048,

    ELEMENT_ROMING_NOTICE = 1051,
    ELEMENT_OFFLINE_NOTICE = 1052,

    ELEMENT_ADD_IP_DOMAIN_NAME_SETS = 1053,
    ELEMENT_DEL_IP_DOMAIN_NAME_SETS = 1054,
    ELEMENT_NAT_AND_DHCP_CONFIG = 1055,
    ELEMENT_AP_LOAD_BALANCE = 1057,
    ELEMENT_WIRELESS_RATE_SET = 1058,
    ELEMENT_LRSSI_REFUSE = 1059,
    ELEMENT_LOCAL_AC_ADDR = 1060,
    ELEMENT_CONNECTION_MODE = 1061,
    ELEMENT_RUNNING_MODE = 1062,
    ELEMENT_CLOUD_AC_ADDR = 1063,
	ELEMENT_SPECTRUM_ANALYSIS = 1064,

    ELEMENT_AP_REPORT_STA_SWITCH = 1100,
    ELEMENT_WIRELESS_OPTIMIZATION_TLV = 1101,

    ELEMENT_CONF_INFO = 1102,
    ELEMENT_CONF_INFO_ALL = 1103,

    ELEMENT_TERMINAL_DATA_SELECTION = 1104,
    ELEMENT_VLAN_INTERFACE = 1107,

    ELEMENT_CONTROL_SET = 1108,

    ELEMENT_AUTO_CHANNEL_SELECT = 1110,

    ELEMENT_WIRELESS_LOCATION_TLV = 1112,
	ELEMENT_STA_FLOW_REPORT_INTERVAL = 1115,
    ELEMENT_LOGIN_PASSWORD_AND_SSH_PASSWORD = 1116,

    ELEMENT_LAN_PORTAL_TLV = 1118,

    ELEMENT_ADD_BLACK_IP_DOMAIN_NAME_SETS = 1301,     //black unauthenticated stragety
    ELEMENT_DEL_BLACK_IP_DOMAIN_NAME_SETS = 1302,

}MSG_ELEMENT_TYPE;

class CBuffer;

class CWTPBoardDataTlv : public CElement
{
public:
    CWTPBoardDataTlv(){ set_element_type(ELEMENT_WTP_BOARD_DATA); }
    ~CWTPBoardDataTlv(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");

private:
    uint32_t vendor_identify;
    CWTPModelNumber model_number;
    CWTPSerialNumber serial_number;
    CWTPBoardId board_id;
    CWTPBoardRevision board_revision;
    CWTPBoardMacAddress board_macaddress;
};

class CVendorSpecPayLoadTlv : public CElement
{
public:
    CVendorSpecPayLoadTlv()
    {
        set_element_type(ELEMENT_VENDOR_SPEC_PAYLOAD);
        vendor_identifier = DUNCHONG_IDENTIFY;
    }
    ~CVendorSpecPayLoadTlv(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
private:
    uint32_t vendor_identifier;
public:

    // AC ---> AP
    CAPTransConfTlv ap_trans;
    CVSRadioConfTlv radio_conf;
    CVSVlanConfTlv vlan_conf;
    CVSAPSpaceInfoTlv ap_space_info;
    CVSABPRadioConfTlv abp_radio_conf;
    CVSRadioProbeConfTlv radio_probe_conf;
    CVSEchoConfTlv echo_conf;
    CVSTrafficStaticsTlv traffic_statics_conf;
    CVSPacketPowerConfTlv packet_power_conf;
    CVSChannelReuseConfTlv channel_reuse_conf;
    CVSStationInfoReqTlv station_info_req;
    // CVSPortalConfTlv portal_conf;   // not use
    CVSMcastEnhanceTlv mcast_enhance;
    CVSRadio11ACConfTlv radio_11ac;
    CVSWlanInfoTlv wlan_info;
    CVSReverseSSHTlv reverse_ssh;
    CVS8021xConfTlv auth_8021x;
    CVSSrcIPConfTlv src_ip_conf;
    CVSNtpServerConfTlv ntp_server;
    CVSPortalCustomConfTlv portal_custom;
    CVSTimeStampConfTlv time_stamp;
    CVSByPassConfTlv by_pass;

    CVSAddAccessControlTlv add_access_ctrl;
    CVSModifyAccessControlTlv modify_access_ctrl;
    CVSDelAccessControlTlv del_access_ctrl;

    CVSAddAccessRuleTlv add_access_rule;
    CVSModifyAccessRuleTlv modify_access_rule;
    CVSDelAccessRuleTlv del_access_rule;

    CVSAddAccessStrategyTlv add_access_strategy;
    CVSModifyAccessStrategyTlv modify_access_trategy;
    CVSDelAccessStrategyTlv del_access_strategy;

    CVSAddSpaceInfoTlv add_space_info;
    CVSModifySpaceInfoTlv modify_space_info;
    CVSDelSpaceInfoTlv del_space_info;

    CVSAddTimeConfTlv add_time_conf;
    CVSModifyTimeConfTlv modify_time_conf;
    CVSDelTimeConfTlv del_time_conf;

    CVSAddUserRoleTlv add_user_role;
    CVSModifyUserRoleTlv modify_user_role;
    CVSDelUserRoleTlv del_user_role;

    // AP ---> AC
    CVSResultStrTlv result_str;
    CVSStationIPAddrTlv station_ip;
    CVSReportStaInfoByMacTlv sta_info;      // 只上报请求的mac的信息
    CVSActlReportStaInfoTlv actl_sta_info;  // 多个终端一起上报
    CVSActlReportStaStateTlv sta_state;     // 多个Add Station
    CVSReportAPInfoTlv ap_info;
    // CVSStaRomingTlv sta_roming;  // not use
    CAPLanIPTlv ap_lanip;
    CVSStaAuthReqTlv sta_auth_req;          // 应该是SW解析HTTP响应后直接认证成功
    CVSAutoChannelSelectInfoTlv auto_channel_select_info;
    CVSResultStrTlv result;

    //  all
    CVSActlUserInfoPktTlv actl_user_info;
    CVSWxAuthInfoTlv wx_auth;
};

class CACNameTlv : public CElement
{
private:
    string value;
public:
    CACNameTlv()
    {
        set_element_type(ELEMENT_AC_NAME);
        value = "";
    }
    ~CACNameTlv(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CCtrlIpv4AddressTlv : public CElement
{
private:
    uint32_t ip_addr;
    uint16_t wtp_count;
public:
    CCtrlIpv4AddressTlv()
    {
        set_element_type(ELEMENT_IPv4_ADDR);

        ip_addr = 0;
        wtp_count = 0;
    }
    ~CCtrlIpv4AddressTlv(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CWTPRadioInfoTlv : public CElement
{
private:
    uint8_t radio_id;
#ifdef __mips32__
    union {
        uint32_t radio_type;
        struct {
            uint32_t reserved : 28;
            uint32_t radio_type_11n : 1;
            uint32_t radio_type_11g : 1;
            uint32_t radio_type_11a : 1;
            uint32_t radio_type_11b : 1;
        }s;
    }u_radio_type;
#else
    union {
        uint32_t radio_type;
        struct {
            uint32_t radio_type_11b : 1;
            uint32_t radio_type_11a : 1;
            uint32_t radio_type_11g : 1;
            uint32_t radio_type_11n : 1;
            uint32_t reserved : 28;
        }s;
    }u_radio_type;
#endif
public:
    CWTPRadioInfoTlv()
    {
        set_element_type(ELEMENT_WTP_RADIO_INFO);
        radio_id = 0;
        u_radio_type.radio_type = 0;
    }
    ~CWTPRadioInfoTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CWTPDescriptorTlv : public CElement
{
private:
    uint8_t max_radios;
    uint8_t radio_inuse;
    uint8_t encrypt_number;
#ifdef __mips32__
    union {
        uint8_t encrypt_wbid;
        struct {
            uint8_t reserved : 3;
            uint8_t wbid : 5;
        }s;
    }u_encrypt_wbid;
#else
    union {
        uint8_t encrypt_wbid;
        struct {
            uint8_t wbid : 5;
            uint8_t reserved : 3;
        }s;
    }u_encrypt_wbid;
#endif
    uint16_t encrypt_capabilities;

    CWTPDescriptorHardwareVersionTlv hardware_version;
    CWTPDescriptorSoftwareVersionTlv software_version;

public:
    CWTPDescriptorTlv()
    {
        set_element_type(ELEMENT_WTP_DESCRIPTOR);

        max_radios = 0;
        radio_inuse = 0;
        encrypt_number = 0;
        u_encrypt_wbid.encrypt_wbid = 0;
        encrypt_capabilities = 0;
    }
    ~CWTPDescriptorTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CResultTlv : public CElement
{
private:
    uint32_t result;
public:
    CResultTlv()
    {
        set_element_type(ELEMENT_RESULT_CODE);
        result = 0;
    }
    ~CResultTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};


class CWTPRadioConfTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t short_preamble;
    uint8_t bssids_number;
    uint8_t dtim_period;
    uint8_t bssid[6];
    uint16_t beacon_period;
    uint8_t country_string[4];
public:
    CWTPRadioConfTlv()
    {
        set_element_type(ELEMENT_WTP_RADIO_CONFIG);
        radio_id = 0;
        short_preamble = 0;
        bssids_number = 0;
        dtim_period = 0;
        bzero(bssid, sizeof(bssid));
        beacon_period = 0;
        bzero(country_string, sizeof(country_string));
    }
    ~CWTPRadioConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CTxPowerTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t reserved;
    uint16_t cur_tx_power;
public:
    CTxPowerTlv()
    {
        set_element_type(ELEMENT_TXPOWER);
        radio_id = 0;
        reserved = 0;
        cur_tx_power = 0;
    }
    ~CTxPowerTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};
class CDSCtrlTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t reserved;
    uint8_t cur_channel;
    uint8_t cur_cca;
    uint32_t energy_detect_threshold;
public:
    CDSCtrlTlv()
    {
        set_element_type(ELEMENT_DIRECT_SEQU_CTL);
        radio_id = 0;
        reserved = 0;
        cur_channel = 0;
        cur_cca = 0;
        energy_detect_threshold = 0;
    }
    ~CDSCtrlTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CRadioAdminStateTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t radio_state;

public:
    CRadioAdminStateTlv()
    {
        set_element_type(ELEMENT_RADIO_ADMIN_STATE);
        radio_id = 0;
        radio_state = 0;
    }
    ~CRadioAdminStateTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CMacOperationTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t reserved;
    uint16_t rts_thr;
    uint8_t short_retry;
    uint8_t long_retry;
    uint16_t frag_thr;
    uint32_t tx_msdu_lifetime;
    uint32_t rx_msdu_lifetime;
public:
    CMacOperationTlv()
    {
        set_element_type(ELEMENT_MAC_OPERATION);
        radio_id = 0;
        reserved = 0;
        rts_thr = 0;
        short_retry = 0;
        long_retry = 0;
        frag_thr = 0;
        tx_msdu_lifetime = 0;
        rx_msdu_lifetime = 0;
    }
    ~CMacOperationTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");

};


class COFDMCtrlTlv : public CElement
{
private:
	uint8_t		radio_id;
	uint8_t		reserved;
	uint8_t		current_channel;
	uint8_t		band_width; // 1--20M 2--20/40M 3--40-M 4--40+M
	uint32_t	ti_threshold;

public:
	COFDMCtrlTlv()
    {
        set_element_type(ELEMENT_OFDM_CTL);
        radio_id = 0;
        reserved = 0;
        current_channel = 0;
        band_width = 0;
        ti_threshold = 0;
    }
    ~COFDMCtrlTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CRadioOperationalStateTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t radio_op_state;
    uint8_t radio_op_cause;
public:
    CRadioOperationalStateTlv()
    {
        set_element_type(ELEMENT_RADIO_OPT_STATE);
    }
    ~CRadioOperationalStateTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CDataTransferTlv : public CElement
{
private:
    uint8_t data_type;
    uint8_t data_mode;
    uint16_t data_length;
    string data;
public:
    CDataTransferTlv()
    {
        set_element_type(ELEMENT_DATA_TRANS_DATA);
        data_type = 0;
        data_mode = 0;
        data_length = 0;
    }
    ~CDataTransferTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class COutSideAuthConfTlv : public CElement
{
private:
    uint8_t  enable;
    uint32_t secret_len;
    string   secret;
    uint8_t  auth_ip_type;
    uint8_t  auth_ip[16];
    uint32_t auth_port;
    uint8_t  account_ip_type;
    uint8_t  account_ip[16];
    uint32_t account_port;
    uint32_t radius_force_offline_port;
public:
    COutSideAuthConfTlv()
    {
        set_element_type(ELEMENT_OUTSIDE_RADIUS_INFO);
        enable = 0;
        secret_len = 0;
        auth_ip_type = 0;
        bzero(auth_ip, sizeof(auth_ip));
        auth_port = 0;
        account_ip_type = 0;
        bzero(account_ip, sizeof(account_ip));
        account_port = 0;
        radius_force_offline_port = 0;
    }
    ~COutSideAuthConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};


class CAddDstWhiteConfTlv : public CElement
{
private:
    uint32_t count;
    vector<CDstAddr> dst_addrs;
public:
    CAddDstWhiteConfTlv()
    {
        set_element_type(ELEMENT_ADD_IP_DOMAIN_NAME_SETS);
        count = 0;
    }
    ~CAddDstWhiteConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CDelDstWhiteConfTlv : public CElement
{
private:
    uint32_t count;
    vector<CDstAddr> dst_addrs;
public:
    CDelDstWhiteConfTlv()
    {
        set_element_type(ELEMENT_DEL_IP_DOMAIN_NAME_SETS);
        count = 0;
    }
    ~CDelDstWhiteConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CNatDhcpConfTlv : public CElement
{
private:
    uint8_t  enable;
    uint8_t  nat_ip_type;
    uint8_t  nat_ip[16];
    uint32_t nat_netmask;

    uint8_t  dhcp_start_ip_type;
    uint8_t  dhcp_start_ip[16];
    uint8_t  dhcp_end_ip_type;
    uint8_t  dhcp_end_ip[16];
    uint32_t dhcp_lease_time;
public:
    CNatDhcpConfTlv()
    {
        set_element_type(ELEMENT_NAT_AND_DHCP_CONFIG);
        enable = 0;
        nat_ip_type = 0;
        bzero(nat_ip, sizeof(nat_ip));
        nat_netmask = 0;
        dhcp_start_ip_type = 0;
        bzero(dhcp_start_ip, sizeof(dhcp_start_ip));
        dhcp_end_ip_type = 0;
        bzero(dhcp_end_ip, sizeof(dhcp_end_ip));
        dhcp_lease_time = 0;
    }
    ~CNatDhcpConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAPReportStationInfoEnableTlv : public CElement
{
private:
    uint8_t enable;
public:
    CAPReportStationInfoEnableTlv()
    {
        set_element_type(ELEMENT_AP_REPORT_STA_SWITCH);
        enable = 0;
    }
    ~CAPReportStationInfoEnableTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CRomingConfTlv : public CElement
{
private:
    uint8_t  enable;
    uint32_t signal;
public:
    CRomingConfTlv()
    {
        set_element_type(ELEMENT_WIRELESS_OPTIMIZATION_TLV);
        enable = 0;
        signal = 0;
    }
    ~CRomingConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CWirelessLocationConfTlv : public CElement
{
private:
    uint8_t  wp_enable;
    uint8_t  wp_intv;
    uint8_t  wp_server_ip_type;
    uint8_t  wp_server_ip[16];
    uint16_t wp_server_port;
    uint8_t  wp_scan_type;
    uint8_t  wp_code;
    uint8_t  wp_proto;

    uint8_t  ef_enable;
    uint8_t  ef_code;
    uint8_t  ef_proto;
    uint8_t  ef_intv;
    uint8_t  ef_scan_type;
    uint8_t  ef_server_ip_type;
    uint8_t  ef_server_ip[16];
    uint16_t ef_server_port;

    uint16_t we_ad_intv;
    uint32_t we_channel_2g;
    uint32_t we_channel_5g;
    uint16_t we_ad_rssi;
public:
    CWirelessLocationConfTlv()
    {
        set_element_type(ELEMENT_WIRELESS_LOCATION_TLV);
        wp_enable = 0;
        wp_intv = 0;
        wp_server_ip_type = 0;
        bzero(wp_server_ip, sizeof(wp_server_ip));
        wp_server_port = 0;
        wp_scan_type = 0;
        wp_code = 0;
        wp_proto = 0;

        ef_enable = 0;
        ef_code = 0;
        ef_proto = 0;
        ef_intv = 0;
        ef_scan_type = 0;
        ef_server_ip_type = 0;
        bzero(ef_server_ip, sizeof(ef_server_ip));
        ef_server_port = 0;

        we_ad_intv = 0;
        we_channel_2g = 0;
        we_channel_5g = 0;
        we_ad_rssi = 0;
    }
    ~CWirelessLocationConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CRfgConfTlv : public CElement
{
private:
    uint8_t rfg_enable;
    uint8_t rfg_assocmax;
    uint8_t rfg_timeout;
    uint8_t rfg_maxsta;
    uint8_t rfg_method;
public:
    CRfgConfTlv()
    {
        set_element_type(ELEMENT_RFG_INFO_TLV);
        rfg_enable = 0;
        rfg_assocmax = 0;
        rfg_timeout = 0;
        rfg_maxsta = 0;
        rfg_method = 0;
    }
    ~CRfgConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAPLoadBalanceConfTlv : public CElement
{
private:
    uint8_t  load_balance_enable;
    uint16_t load_balance_threshold;
    uint16_t load_balance_interval;
public:
    CAPLoadBalanceConfTlv()
    {
        set_element_type(ELEMENT_AP_LOAD_BALANCE);
        load_balance_enable = 0;
        load_balance_threshold = 0;
        load_balance_interval = 0;
    }
    ~CAPLoadBalanceConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CRateSetConfTlv : public CElement
{
private:
    uint8_t radio_type_11a_len;
    uint8_t radio_type_11a_rate[16];
    uint8_t radio_type_11bg_len;
    uint8_t radio_type_11bg_rate[16];
    uint8_t radio_type_11n_len;
    uint8_t radio_type_11n_rate[16];
    uint8_t radio_type_11ac_len;
    uint8_t radio_type_11ac_rate[16];
public:
    CRateSetConfTlv()
    {
        set_element_type(ELEMENT_WIRELESS_RATE_SET);
        bzero(radio_type_11a_rate, sizeof(radio_type_11a_rate));
        bzero(radio_type_11bg_rate, sizeof(radio_type_11bg_rate));
        bzero(radio_type_11n_rate, sizeof(radio_type_11n_rate));
        bzero(radio_type_11ac_rate, sizeof(radio_type_11ac_rate));

        // 速率集因为没有开关选项，需要初始化为全部速率启用
        radio_type_11a_len = 1;
        radio_type_11a_rate[0] = 0xff;
        radio_type_11bg_len = 2;
        radio_type_11bg_rate[0] = 0xff;
        radio_type_11bg_rate[1] = 0xf0;
        radio_type_11n_len = 3;
        radio_type_11n_rate[0] = 0xff;
        radio_type_11n_rate[0] = 0xff;
        radio_type_11ac_len = 0;
    }
    ~CRateSetConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CLowRssiRefuseConfTlv : public CElement
{
private:
    uint8_t  low_rssi_refuse_enable;
    uint32_t low_rssi_threshold;
public:
    CLowRssiRefuseConfTlv()
    {
        set_element_type(ELEMENT_LRSSI_REFUSE);
        low_rssi_refuse_enable = 0;
        low_rssi_threshold  = 0;
    }
    ~CLowRssiRefuseConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CCloudACAddrConfTlv : public CElement
{
private:
    uint8_t cloud_addr_len;
    string  cloud_addr;
public:
    CCloudACAddrConfTlv()
    {
        set_element_type(ELEMENT_CLOUD_AC_ADDR);
        cloud_addr_len = 0;
    }
    ~CCloudACAddrConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CLocalACAddrConfTlv : public CElement
{
private:
    uint8_t  ac_addr_len;
    string   ac_addr;
public:
    CLocalACAddrConfTlv()
    {
        set_element_type(ELEMENT_LOCAL_AC_ADDR);
        ac_addr_len = 0;
    }
    ~CLocalACAddrConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CConnectionModeConfTlv : public CElement
{
private:
    uint8_t connection_mode;
public:
    CConnectionModeConfTlv()
    {
        set_element_type(ELEMENT_CONNECTION_MODE);
        connection_mode = 0;
    }
    ~CConnectionModeConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CRunningModeConfTlv : public CElement
{
private:
    uint8_t ap_running_mode;
public:
    CRunningModeConfTlv()
    {
        set_element_type(ELEMENT_RUNNING_MODE);
        ap_running_mode = 0;
    }
    ~CRunningModeConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CStationDataCollectionConfTlv : public CElement
{
private:
    uint8_t  msg_type;
    uint8_t  enable;
    uint32_t server_url_len;
    string   server_url;
    uint32_t scan_interval;
public:
    CStationDataCollectionConfTlv()
    {
        set_element_type(ELEMENT_TERMINAL_DATA_SELECTION);
        msg_type = 0;
        enable = 0;
        server_url_len = 0;
        scan_interval = 0;
    }
    ~CStationDataCollectionConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CSpectrumAnalysisConfTlv : public CElement
{
private:
    uint8_t  spectrum_analysis_enable;
    uint8_t  spectrum_analysis_mode;
    uint16_t spectrum_analysis_port;
public:
    CSpectrumAnalysisConfTlv()
    {
        set_element_type(ELEMENT_SPECTRUM_ANALYSIS);
        spectrum_analysis_enable = 0;
        spectrum_analysis_mode = 0;
        spectrum_analysis_port = 0;
    }
    ~CSpectrumAnalysisConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CLanVlanConfTlv : public CElement
{
private:
    uint32_t lan_vlan_id;
public:
    CLanVlanConfTlv()
    {
        set_element_type(ELEMENT_VLAN_INTERFACE);
        lan_vlan_id = 0;
    }
    ~CLanVlanConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAPReportStationInfoIntervalTlv : public CElement
{
private:
    uint32_t interval;
public:
    CAPReportStationInfoIntervalTlv()
    {
        set_element_type(ELEMENT_STA_FLOW_REPORT_INTERVAL);
        interval = 0;
    }
    ~CAPReportStationInfoIntervalTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAPPasswordConfTlv : public CElement
{
private:
    uint32_t front_password_len;
    string   front_password;
    uint32_t ssh_password_len;
    string   ssh_password;
public:
    CAPPasswordConfTlv()
    {
        set_element_type(ELEMENT_LOGIN_PASSWORD_AND_SSH_PASSWORD);
        front_password_len = 0;
        ssh_password_len = 0;
    }
    ~CAPPasswordConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAuditAppriConfTlv : public CElement
{
private:
    uint8_t audit_enable;
    uint8_t appri_enable;
    uint8_t remote_sync_enable;
    uint8_t opaque[3];
public:
    CAuditAppriConfTlv()
    {
        set_element_type(ELEMENT_CONTROL_SET);
        audit_enable = 0;
        appri_enable = 0;
        remote_sync_enable = 0;
        bzero(opaque, sizeof(opaque));
    }
    ~CAuditAppriConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CLanPortalConfTlv : public CElement
{
private:
    uint32_t unitid;
    uint32_t portal_enable;
    uint32_t portal_url_len;
    string   portal_url;
    uint8_t  reserved[16];
public:
    CLanPortalConfTlv()
    {
        set_element_type(ELEMENT_LAN_PORTAL_TLV);
        unitid = 0;
        portal_enable = 0;
        portal_url_len = 0;
        bzero(reserved, sizeof(reserved));
    }
    ~CLanPortalConfTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAddDstBlackConfTlv : public CElement
{
public:
    uint32_t count;
    vector<CDstAddr> dst_blacks;

    CAddDstBlackConfTlv()
    {
        set_element_type(ELEMENT_ADD_BLACK_IP_DOMAIN_NAME_SETS);
        count = 0;
    }
    ~CAddDstBlackConfTlv(){}
public:

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CDelDstBlackConfTlv : public CElement
{
public:
    uint32_t count;
    vector<CDstAddr> dst_blacks;

    CDelDstBlackConfTlv()
    {
        set_element_type(ELEMENT_DEL_BLACK_IP_DOMAIN_NAME_SETS);
        count = 0;
    }
    ~CDelDstBlackConfTlv(){}
public:

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CConfInfoEntry
{
private:
    uint32_t type;
    uint32_t len;
    string   value;
public:
    CConfInfoEntry()
    {
        type = 0;
        len = 0;
    }
    ~CConfInfoEntry(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};
class CConfInfoTlv : public CElement
{
public:
    uint32_t count;
    vector<CConfInfoEntry> conf_infos;

    CConfInfoTlv()
    {
        set_element_type(ELEMENT_CONF_INFO);
        count = 0;
    }
    ~CConfInfoTlv(){}
public:

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CConfInfoAllTlv : public CElement
{
private:
    uint8_t  ac_addr_len;
    string   ac_addr;
    uint8_t  cloud_addr_len;
    string   cloud_addr;
    uint8_t  current_run_mode;
public:
    CConfInfoAllTlv()
    {
        set_element_type(ELEMENT_CONF_INFO_ALL);
        ac_addr_len = 0;
        cloud_addr_len = 0;
        current_run_mode = 0;
    }

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CACTimeStampTlv : public CElement
{
private:
    uint32_t timestamp;
public:
    CACTimeStampTlv()
    {
        set_element_type(ELEMENT_AC_TIMESTAMP);
        timestamp = 0;
    }
    ~CACTimeStampTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CImageIdentifierTlv : public CElement
{
private:
    uint32_t vendor_id;
    CImageIDDataDevModelTlv dev_model;
    CImageIDDataSoftwareVersionTlv software_version;
    CImageIDDataFileNameTlv file_name;
    CImageIDDataFileServerTlv file_server;
    CImageIDDataDownloadTypeTlv download_type;
    CImageIDDataFtpUserNameTlv ftp_user_name;
    CImageIDDataFtpPasswordTlv ftp_password;
    CImageIDDataFtpPathTlv ftp_path;

public:
    CImageIdentifierTlv()
    {
        set_element_type(ELEMENT_IMAGE_IDENTIFIER);
        vendor_id = 0;
    }
    ~CImageIdentifierTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAutoChannelSelectTlv : public CElement
{
private:
    uint32_t identifier;
    uint8_t  select_enable;
    uint8_t  select_2g_channel;
    uint8_t  select_5g_channel;
public:
    CAutoChannelSelectTlv()
    {
        set_element_type(ELEMENT_AUTO_CHANNEL_SELECT);
    }
    ~CAutoChannelSelectTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAddStationTlv : public CElement
{
private:
    uint8_t  radio_id;
    uint8_t  mac_len;
    uint8_t  mac[6];
    uint32_t vlan_id;
    uint8_t wlan_id;
    uint8_t  ssid_len;
    string   ssid;
public:
    CAddStationTlv()
    {
        set_element_type(ELEMENT_ADD_STATION);
        radio_id = 0;
        mac_len = 0;
        bzero(mac, sizeof(mac));
        vlan_id = 0;
        wlan_id = 0;
        ssid_len = 0;
    }
    ~CAddStationTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CDelStationTlv : public CElement
{
private:
    uint8_t  radio_id;
    uint8_t  mac_len;
    uint8_t  mac[6];
    uint8_t wlan_id;
public:
    CDelStationTlv()
    {
        set_element_type(ELEMENT_DEL_STATION);
        radio_id = 0;
        mac_len = 0;
        bzero(mac, sizeof(mac));
        wlan_id = 0;
    }
    ~CDelStationTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CAddWlanTlv : public CElement
{
private:
    uint8_t	 radio_id;
	uint8_t	 wlan_id;
#ifdef __mips32__
    union{
        uint16_t capability;
        struct{
            uint16_t reserved1:6;
            uint16_t qos_enable:1;
            uint16_t reserved2:9;
        }s;
    }u_capability;
#else
    union{
        uint16_t capability;
        struct{
            uint16_t reserved2:9;
            uint16_t qos_enable:1;
            uint16_t reserved1:6;
        }s;
    }u_capability;
#endif
    uint8_t	 key_index;
	uint8_t	 key_status;
	uint16_t key_len;
	uint8_t	 key[32];
	uint32_t group_tsc32; // not use
	uint16_t group_tsc16; // not use
	uint8_t	 qos;         // not use
	uint8_t	 auth_type;   // 1--wep  2--wpa-psk 3--wpa2-psk 4--wpa-eap 5--wpa2-eap 0--open
	uint8_t	 mac_mode;    // not use
	uint8_t	 tunnel_mode; // 0---central  1--local
	uint8_t	 hide_ssid;   // 0---hide 1--not hide
	string	 ssid;

public:
    CAddWlanTlv()
    {
        set_element_type(ELEMENT_ADD_WLAN);
        radio_id = 0;
        wlan_id = 0;
        u_capability.capability = 0;
        key_index = 0;
        key_status = 0;
        key_len = 0;
        bzero(key, sizeof(key));
        group_tsc32 = 0;
        group_tsc16 = 0;
        qos = 0;
        auth_type = 0;
        mac_mode = 0;
        tunnel_mode = 0;
        hide_ssid = 0;
    }
    ~CAddWlanTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CDelWlanTlv : public CElement
{
private:
    uint8_t	 radio_id;
	uint8_t	 wlan_id;
public:
    CDelWlanTlv()
    {
        set_element_type(ELEMENT_DEL_WLAN);
        radio_id = 0;
        wlan_id = 0;
    }
    ~CDelWlanTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CUpdateWlanTlv : public CElement
{
private:
    uint8_t  radio_id;
    uint8_t  wlan_id;
#ifdef __mips32__
    union{
        uint16_t capability;
        struct{
            uint16_t reserved1:6;
            uint16_t qos_enable:1;
            uint16_t reserved2:9;
        }s;
    }u_capability;
#else
    union{
        uint16_t capability;
        struct{
            uint16_t reserved2:9;
            uint16_t qos_enable:1;
            uint16_t reserved1:6;
        }s;
    }u_capability;
#endif
    uint8_t  key_index;
    uint8_t  key_status;
    uint16_t key_length;
    uint8_t  key[32];
public:
    CUpdateWlanTlv()
    {
        set_element_type(ELEMENT_UPDATE_WLAN);
        radio_id = 0;
        wlan_id = 0;
        u_capability.capability = 0;
        key_index = 0;
        key_status = 0;
        key_length = 0;
        bzero(key, sizeof(key));
    }
    ~CUpdateWlanTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class C80211InfomationTlv : public CElement
{
private:
    uint8_t	 radio_id;
	uint8_t	 wlan_id;
#ifdef __mips32__
    union {
        uint8_t bp;
        struct {
            uint8_t B : 1;
            uint8_t P : 1;
            uint8_t reserved : 6;
        }s;
    }u_bp;
#else
    union {
        uint8_t bp;
        struct {
            uint8_t reserved : 6;
            uint8_t P : 1;
            uint8_t B : 1;
        }s;
    }u_bp;
#endif
    CWpaMiniTlv wpa;
    CRsnMiniTlv rsn;
public:
    C80211InfomationTlv()
    {
        set_element_type(ELEMENT_INFO_ELEMENT);
        radio_id = 0;
        wlan_id = 0;
        u_bp.bp = 0;
    }
    ~C80211InfomationTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CInitDownLoadTlv : public CElement
{
private:
public:
    CInitDownLoadTlv()
    {
        set_element_type(ELEMENT_INIT_DOWNLOAD);
    }
    ~CInitDownLoadTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CImageDataTlv : public CElement
{
private:
    uint8_t data[1024];
public:
    CImageDataTlv()
    {
        set_element_type(ELEMENT_IMAGE_DATA);
    }
    ~CImageDataTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};

class CImageInfoTlv : public CElement
{
private:
    uint32_t file_size;
    uint64_t hash0;
    uint64_t hash1;
public:
    CImageInfoTlv()
    {
        set_element_type(ELEMENT_IMAGE_INFO);
    }
    ~CImageInfoTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str, string ex = "");
    int LoadFrom(kvlist &kv, string ex="");
};














#endif
