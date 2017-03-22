
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

    LLEMENT_TERMINAL_DATA_SELECTION = 1104,
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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv, string ex="");
private:
    uint32_t vendor_identifier;
public:
    CAPLanIPTlv ap_lanip;
    CAPTransConfTlv ap_trans;
    CVSRadioConfTlv radio_conf;
    CVSAPSpaceInfoTlv ap_space_info;
    CVSABPRadioConfTlv abp_radio_conf;
    CVSRadioProbeConfTlv radio_probe_conf;
    CVSEchoConfTlv echo_conf;
    CVSTrafficStaticsTlv traffic_statics_conf;
    CVSPacketPowerConfTlv packet_power_conf;
    CVSChannelReuseConfTlv channel_reuse_conf;
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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv, string ex="");
};

#endif

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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv, string ex="");
};
class CDSCtrolTlv : public CElement
{
private:
    uint8_t radio_id;
    uint8_t reserved;
    uint8_t cur_channel;
    uint8_t cur_cca;
    uint32_t energy_detect_threshold;
public:
    CDSCtrolTlv()
    {
        set_element_type(ELEMENT_DIRECT_SEQU_CTL);
        radio_id = 0;
        reserved = 0;
        cur_channel = 0;
        cur_cca = 0;
        energy_detect_threshold = 0;
    }
    ~CDSCtrolTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv, string ex="");

};


class COFDMCtrolTlv : public CElement
{
private:
	uint8_t		radio_id;
	uint8_t		reserved;
	uint8_t		current_channel;
	uint8_t		band_width;
	uint32_t	ti_threshold;

public:
	COFDMCtrolTlv()
    {
        set_element_type(ELEMENT_OFDM_CTL);
        radio_id = 0;
        reserved = 0;
        current_channel = 0;
        band_width = 0;
        ti_threshold = 0;
    }
    ~COFDMCtrolTlv(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
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
    int SaveTo(string &str);
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
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv, string ex="");
};
