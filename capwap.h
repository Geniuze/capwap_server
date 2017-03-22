#ifndef _CAPWAP_H_
#define _CAPWAP_H_

#include <vector>

#include "stdlib.h"
#include "string.h"
#include "buffer.h"
#include "capwap_elements.h"
#include "kvlist.h"

typedef enum {
    CAPWAP_PACKET_TYPE_DISCOVERY_REQ = 1,
    CAPWAP_PACKET_TYPE_DISCOVERY_RSP,
    CAPWAP_PACKET_TYPE_JOIN_REQ,
    CAPWAP_PACKET_TYPE_JOIN_RSP,
    CAPWAP_PACKET_TYPE_CONFIG_STATUS_REQ,
    CAPWAP_PACKET_TYPE_CONFIG_STATUS_RSP,
    CAPWAP_PACKET_TYPE_CONFIG_UPDATE_REQ,
    CAPWAP_PACKET_TYPE_CONFIG_UPDATE_RSP,
    CAPWAP_PACKET_TYPE_WTP_EVENT_REQ,
    CAPWAP_PACKET_TYPE_WTP_EVENT_RSP,
    CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_REQ,
    CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_RSP,
    CAPWAP_PACKET_TYPE_ECHO_REQ,
    CAPWAP_PACKET_TYPE_ECHO_RSP,
    CAPWAP_PACKET_TYPE_IMAGE_DATA_REQ,
    CAPWAP_PACKET_TYPE_IMAGE_DATA_RSP,
    CAPWAP_PACKET_TYPE_RESET_REQ,
    CAPWAP_PACKET_TYPE_RESET_RSP,
    CAPWAP_PACKET_TYPE_PRIMARY_DISCV_REQ,
    CAPWAP_PACKET_TYPE_PRIMARY_DISCV_RSP,
    CAPWAP_PACKET_TYPE_DATA_TRANSFER_REQ,
    CAPWAP_PACKET_TYPE_DATA_TRANSFER_RSP,
    CAPWAP_PACKET_TYPE_CLEAR_CONFIG_REQ,
    CAPWAP_PACKET_TYPE_CLEAR_CONFIG_RSP,
    CAPWAP_PACKET_TYPE_STATION_CONFIG_REQ,
    CAPWAP_PACKET_TYPE_STATION_CONFIG_RSP,
    CAPWAP_PACKET_TYPE_AP_CONFIG_REQ,
    CAPWAP_PACKET_TYPE_AP_CONFIG_RSP,
    CAPWAP_PACKET_TYPE_AP_INFO_REQ,
    CAPWAP_PACKET_TYPE_AP_INFO_RSP,
    CAPWAP_PACKET_TYPE_WLAN_CONFIG_REQ = 0x33dd01,
    CAPWAP_PACKET_TYPE_WLAN_CONFIG_RSP = 0x33dd02,
} capwap_packet_type_t;

class CCapwapUtil {
public:
    CCapwapUtil(){}
    ~CCapwapUtil(){}

    int Assemble(CBuffer &buffer) { return 0; }
    int Parse(CBuffer &buffer) { return 0; }
};


class CPreamble : public CCapwapUtil
{
private:

#ifdef __mips32__
    union {
        uint8_t preamble;
        struct {
            uint8_t version:4;
            uint8_t type   :4;
        }s;
    }u;
#else
    union {
        uint8_t preamble;
        struct {
            uint8_t type   :4;
            uint8_t version:4;
        }s;
    }u;
#endif

public:
    CPreamble(){ u.preamble = 0; }
    ~CPreamble(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
};

class CCapwapProtoHeader : public CCapwapUtil
{
public:
    CCapwapProtoHeader()
    {
        u_header.header = 0;
        u_header.s.header_len = 2;
        u_header.s.wireless_binding_id = 1;

        u_header_flags.header_flags = 0;

        fragment_id = 0;

        u_fragment.fragment = 0;
    }
    ~CCapwapProtoHeader(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
private:
#ifdef __mips32__
    union {
        uint16_t header;
        struct {
            uint16_t header_len : 5;
            uint16_t radio_id : 5;
            uint16_t wireless_binding_id : 5;
            uint16_t payload_type : 1;
        }s;
    }u_header;

    union {
        uint8_t header_flags;
        struct {
            uint8_t fragment : 1;
            uint8_t last_fragment : 1;
            uint8_t wireless_header : 1;
            uint8_t radio_mac_header : 1;
            uint8_t keep_alive : 1;
            uint8_t reserved : 3;
        }s;
    }u_header_flags;

    uint16_t fragment_id;

    union {
        uint16_t fragment;
        struct {
            uint16_t fragment_offset : 13;
            uint16_t reserved : 3;
        }s;
    }u_fragment;
#else
    union {
        uint16_t header;
        struct {
            uint16_t paload_type : 1;
            uint16_t wireless_binding_id : 5;
            uint16_t radio_id : 5;
            uint16_t header_len : 5;
        }s;
    }u_header;

    union {
        uint8_t header_flags;
        struct {
            uint8_t reserved : 3;
            uint8_t keep_alive : 1;
            uint8_t radio_mac_header : 1;
            uint8_t wireless_header : 1;
            uint8_t last_fragment : 1;
            uint8_t fragment : 1;
        }s;
    }u_header_flags;

    uint16_t fragment_id;

    union {
        uint16_t fragment;
        struct {
            uint16_t reserved : 3;
            uint16_t fragment_offset : 13;
        }s;
    }u_fragment;

#endif
};

class CCapwapCtrlHeader : public CCapwapUtil {
public:
    CCapwapCtrlHeader()
    {
        u_message_type.message_type = 0;
        sequence_num = 0;
        message_element_length = 0;
        flags = 0;
    }
    ~CCapwapCtrlHeader(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);

    void setMessageType(int type)
    {
        u_message_type.s.enterprise_specific = type;
    }
    void setElementsLength(int length)
    {
        message_element_length = length;
    }
    int GetPacketType()
    {
        return u_message_type.s.enterprise_specific;
    }
private:
#ifdef __mips32__
    union {
        uint32_t message_type;
        struct {
            uint32_t enterprise_num : 24;
            uint32_t enterprise_specific : 8;
        }s;
    }u_message_type;
#else
    union {
        uint32_t message_type;
        struct {
            uint32_t enterprise_specific : 8;
            uint32_t enterprise_num : 24;
        }s;
    }u_message_type;
#endif

    uint8_t sequence_num;
    uint16_t message_element_length;
    uint8_t flags;

};


class CCapwapHeader : public CCapwapUtil {
public:
    CPreamble preamble;
    CCapwapProtoHeader protoHeader;
    //CCapwapDTLSHdr dtlsHeader;
    CCapwapCtrlHeader ctrlHeader;
public:
    size_t element_start;
    size_t element_end;

private:
    bool header_parsed;

public:
    CCapwapHeader()
    {
        element_start = 0;
        element_end = 0;
        header_parsed = false;
    }
    ~CCapwapHeader(){}

    virtual int Parse(CBuffer &buffer);
    virtual int Assemble(CBuffer &buffer);
    virtual int SaveTo(string &str);
    virtual int LoadFrom(kvlist &kv);

    void setMessageType(int type)
    {
        ctrlHeader.setMessageType(type);
    }
    void ReAssembleCtrlHeader(CBuffer &buffer)
    {
        int elements_length = element_end - element_start;
        ctrlHeader.setElementsLength(elements_length);

        buffer.revert(elements_length+8);
        ctrlHeader.Assemble(buffer);

        buffer.forward(elements_length);
    }
    void setHeader(CCapwapHeader *cwheader)
    {
        this->preamble = cwheader->preamble;
        this->protoHeader = cwheader->protoHeader;
        this->ctrlHeader = cwheader->ctrlHeader;

        header_parsed = true;
    }
    bool isHeaderParsed()
    {
        return header_parsed;
    }
};

class CCapwapDiscoveryReq : public CCapwapHeader {
public:
    CCapwapDiscoveryReq()
    {
        setMessageType(CAPWAP_PACKET_TYPE_DISCOVERY_REQ);
    }
    ~CCapwapDiscoveryReq(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
public:
    CWTPBoardDataTlv board_data;
    CVendorSpecPayLoadTlv pay_load;
};

class CCapwapDiscoveryRsp : public CCapwapHeader {
public:
    CACNameTlv ac_name;
    CCtrlIpv4AddressTlv ipv4_addr;
    CVendorSpecPayLoadTlv pay_load;
public:
    CCapwapDiscoveryRsp()
    {
        setMessageType(CAPWAP_PACKET_TYPE_DISCOVERY_RSP);
    }
    ~CCapwapDiscoveryRsp(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapJoinReq : public CCapwapHeader {
public:
    vector<CWTPRadioInfoTlv> radios;
    CWTPBoardDataTlv board_data;
    CWTPDescriptorTlv descriptor;
    CVendorSpecPayLoadTlv pay_load;
public:
    CCapwapJoinReq()
    {
        setMessageType(CAPWAP_PACKET_TYPE_JOIN_REQ);
    }
    ~CCapwapJoinReq(){}
    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapJoinRsp : public CCapwapHeader {
public:
    CResultTlv result;
public:
    CCapwapJoinRsp()
    {
        setMessageType(CAPWAP_PACKET_TYPE_JOIN_RSP);
    }
    ~CCapwapJoinRsp(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapConfigureStatusReq : public CCapwapHeader {
public:
    vector<CWTPRadioConfTlv> radio_confs;
    vector<CWTPRadioInfoTlv> radio_infos;
    vector<CTxPowerTlv> tx_powers;
    vector<CDSCtrolTlv> ds_ctrols;

    CVendorSpecPayLoadTlv pay_load;

public:
    CCapwapConfigureStatusReq()
    {
        setMessageType(CAPWAP_PACKET_TYPE_CONFIG_STATUS_REQ);
    }
    ~CCapwapConfigureStatusReq(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapConfigureStatusRsp: public CCapwapHeader {
public:
    vector<CRadioAdminStateTlv> radio_admin_states;
    vector<CWTPRadioConfTlv> radio_confs;
    vector<CWTPRadioInfoTlv> radio_infos;
    vector<CMacOperationTlv> mac_operations;
    vector<CTxPowerTlv> tx_powers;
    vector<CDSCtrolTlv> ds_ctrols;
    vector<COFDMCtrolTlv> ofdm_ctrols;

    vector<CVendorSpecPayLoadTlv> pay_loads;
public:
    CCapwapConfigureStatusRsp()
    {
        setMessageType(CAPWAP_PACKET_TYPE_CONFIG_STATUS_RSP);
    }
    ~CCapwapConfigureStatusRsp(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapChangeStateReq : public CCapwapHeader {
public:
    vector<CRadioOperationalStateTlv> radio_operation_states;
    CResultTlv result;
    CVendorSpecPayLoadTlv pay_load;
public:
    CCapwapChangeStateReq()
    {
        setMessageType(CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_REQ);
    }
    ~CCapwapChangeStateReq(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapChangeStateRsp : public CCapwapHeader {
public:
    CCapwapChangeStateRsp()
    {
        setMessageType(CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_RSP);
    }
    ~CCapwapChangeStateRsp(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapDataTransferReq : public CCapwapHeader {
public:
    CDataTransferTlv data_transfer;
    CVendorSpecPayLoadTlv pay_load;
public:
    CCapwapDataTransferReq()
    {
        setMessageType(CAPWAP_PACKET_TYPE_DATA_TRANSFER_REQ);
    }
    ~CCapwapDataTransferReq(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};

class CCapwapDataTransferRsp : public CCapwapHeader {
public:
    CResultTlv result;
public:
    CCapwapDataTransferRsp()
    {
        setMessageType(CAPWAP_PACKET_TYPE_DATA_TRANSFER_RSP);
    }
    ~CCapwapDataTransferRsp(){}

    int Parse(CBuffer &buffer);
    int Assemble(CBuffer &buffer);
    int SaveTo(string &str);
    int LoadFrom(kvlist &kv);
};






int capwap_packet_type(CCapwapHeader *cwheader);
CCapwapHeader * capwap_get_packet(int packet_type);


#endif
