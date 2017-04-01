#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "capwap.h"
#include "dstring.h"

int CPreamble::Parse(CBuffer &buffer)
{
    buffer.retrive8(u.preamble);
    // dlog(LOG_DEBUG, "type %d  version %d", u.s.type, u.s.version);

    return 0;
}
int CPreamble::Assemble(CBuffer &buffer)
{
    buffer.store8(u.preamble);

    return 0;
}

int CCapwapProtoHeader::Parse(CBuffer &buffer)
{
    buffer.retrive16(u_header.header);
    buffer.retrive8(u_header_flags.header_flags);
    buffer.retrive16(fragment_id);
    buffer.retrive16(u_fragment.fragment);

    /*
    dlog(LOG_DEBUG, "header-len %d radio-id %d wireless-binding-id %d payload-type %d",
           u_header.s.header_len, u_header.s.radio_id,
           u_header.s.wireless_binding_id, u_header.s.paload_type);

    dlog(LOG_DEBUG, "fragment %d last-fragment %d wireless-header %d "
           "radio-mac-header %d keep-alive %d reserved %d",
           u_header_flags.s.fragment, u_header_flags.s.last_fragment,
           u_header_flags.s.wireless_header,
           u_header_flags.s.radio_mac_header,
           u_header_flags.s.keep_alive, u_header_flags.s.reserved);
    dlog(LOG_DEBUG, "fragment-id %d", fragment_id);
    dlog(LOG_DEBUG, "fragment-offset %d reserved %d", u_fragment.s.fragment_offset,
           u_fragment.s.reserved);
    */

    return 0;
}
int CCapwapProtoHeader::Assemble(CBuffer &buffer)
{

    // dlog(LOG_DEBUG, "header-len %d wireless-binding-id %d", u_header.s.header_len, u_header.s.wireless_binding_id);
    buffer.store16(u_header.header);
    buffer.store8(u_header_flags.header_flags);
    buffer.store16(fragment_id);
    buffer.store16(u_fragment.fragment);

    return 0;
}
int CCapwapCtrlHeader::Parse(CBuffer &buffer)
{
    buffer.retrive32(message_type);
    buffer.retrive8(sequence_num);
    buffer.retrive16(message_element_length);
    buffer.retrive8(flags);
/*
    dlog(LOG_DEBUG, "enterprise-specific %d enterprise_num %d",
           u_message_type.s.enterprise_specific, u_message_type.s.enterprise_num);

    dlog(LOG_DEBUG, "sequence-num %d message-element-length %d flags %d",
           sequence_num, message_element_length, flags);
*/
    return 0;
}
int CCapwapCtrlHeader::Assemble(CBuffer &buffer)
{
    buffer.store32(message_type);
    buffer.store8(sequence_num);
    buffer.store16(message_element_length);
    buffer.store8(flags);

    return 0;
}
int CCapwapHeader::Parse(CBuffer &buffer)
{
    if (isHeaderParsed())
        return 0;

    preamble.Parse(buffer);
    protoHeader.Parse(buffer);
    ctrlHeader.Parse(buffer);

    return 0;
}
int CCapwapHeader::Assemble(CBuffer &buffer)
{
    preamble.Assemble(buffer);
    protoHeader.Assemble(buffer);
    ctrlHeader.Assemble(buffer);

    element_start = buffer.GetOffset();

    return 0;
}
int CCapwapHeader::SaveTo(string &str)
{
    return 0;
}
int CCapwapHeader::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapHeader::LoadFrom(kvlist &kv);

int CCapwapDiscoveryReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    board_data.Parse(buffer);
    pay_load.Parse(buffer);

    return 0;
}

int CCapwapDiscoveryReq::Assemble(CBuffer &buffer)
{
    return 0;
}

int CCapwapDiscoveryReq::SaveTo(string &value)
{
    board_data.SaveTo(value);
    pay_load.SaveTo(value);

    return 0;
}
int CCapwapDiscoveryReq::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapDiscoveryRsp::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapDiscoveryRsp::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);

    ac_name.Assemble(buffer);
    ipv4_addr.Assemble(buffer);
    pay_load.Assemble(buffer);

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);

    return 0;
}
int CCapwapDiscoveryRsp::SaveTo(string &value)
{
    return 0;
}
int CCapwapDiscoveryRsp::LoadFrom(kvlist &kv)
{
    dlog(LOG_DEBUG, "%s.%d", __func__, __LINE__);

    ac_name.LoadFrom(kv);
    ipv4_addr.LoadFrom(kv);
    pay_load.LoadFrom(kv);

    return 0;
}

int CCapwapJoinReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    do {
        CWTPRadioInfoTlv radio_info_tlv;
        radio_info_tlv.Parse(buffer);
        if (radio_info_tlv.isValid())
            radios.push_back(radio_info_tlv);
        else
            break;
    }while (true);

    board_data.Parse(buffer);
    descriptor.Parse(buffer);
    pay_load.Parse(buffer);

    return 0;
}
int CCapwapJoinReq::SaveTo(string &str)
{
    for (size_t i = 0; i<radios.size(); i++)
    {
        radios[i].SaveTo(str, toString(i));
    }

    board_data.SaveTo(str);
    descriptor.SaveTo(str);
    pay_load.SaveTo(str);

    return 0;
}
int CCapwapJoinReq::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapJoinReq::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapJoinRsp::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapJoinRsp::SaveTo(string &str)
{
    return 0;
}
int CCapwapJoinRsp::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);

    result.Assemble(buffer);

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);

    return 0;
}
int CCapwapJoinRsp::LoadFrom(kvlist &kv)
{
    result.LoadFrom(kv);

    return 0;
}

int CCapwapConfigureStatusReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    while (buffer.GetLength() != buffer.GetOffset())
    {
        uint32_t start = buffer.GetOffset();
        uint32_t end   = buffer.GetOffset();

        CWTPRadioConfTlv radio_conf;
        CWTPRadioInfoTlv radio_info;
        CTxPowerTlv tx_power;
        CDSCtrlTlv ds_ctrl;

        radio_conf.Parse(buffer);
        if (radio_conf.isValid())
            radio_confs.push_back(radio_conf);

        radio_info.Parse(buffer);
        if (radio_info.isValid())
            radio_infos.push_back(radio_info);

        tx_power.Parse(buffer);
        if (tx_power.isValid())
            tx_powers.push_back(tx_power);

        ds_ctrl.Parse(buffer);
        if (ds_ctrl.isValid())
            ds_ctrls.push_back(ds_ctrl);

        pay_load.Parse(buffer);

        end = buffer.GetOffset();
        if (start == end)
        {
            dlog(LOG_ERR, "Parse error len %d offset %d", buffer.GetLength(), buffer.GetOffset());
            break;
        }
    }
    return 0;
}

int CCapwapConfigureStatusReq::SaveTo(string &str)
{
    size_t i = 0;

    for (i = 0; i<radio_confs.size(); i++)
        radio_confs[i].SaveTo(str, toString(i));
    for (i = 0; i<radio_infos.size(); i++)
        radio_infos[i].SaveTo(str, toString(i));
    for (i = 0; i<tx_powers.size(); i++)
        tx_powers[i].SaveTo(str, toString(i));
    for (i = 0; i<ds_ctrls.size(); i++)
        ds_ctrls[i].SaveTo(str, toString(i));

    pay_load.SaveTo(str);

    return 0;
}
int CCapwapConfigureStatusReq::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapConfigureStatusReq::LoadFrom(kvlist &kv)
{
    return 0;
}
int CCapwapConfigureStatusRsp::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapConfigureStatusRsp::SaveTo(string &str)
{
    return 0;
}
int CCapwapConfigureStatusRsp::Assemble(CBuffer &buffer)
{
    size_t i;
    CCapwapHeader::Assemble(buffer);

    for (i=0; i<radio_admin_states.size(); i++)
    {
        radio_admin_states[i].Assemble(buffer);
        radio_confs[i].Assemble(buffer);
        radio_infos[i].Assemble(buffer);
        mac_operations[i].Assemble(buffer);
        tx_powers[i].Assemble(buffer);
        ds_ctrls[i].Assemble(buffer);
        ofdm_ctrls[i].Assemble(buffer);
        pay_loads[i].Assemble(buffer);
    }

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);
    return 0;
}
int CCapwapConfigureStatusRsp::LoadFrom(kvlist &kv)
{
    size_t i;
    for (i=0; i<radio_admin_states.size(); i++)
    {
        radio_admin_states[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<radio_confs.size(); i++)
    {
        radio_confs[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<radio_infos.size(); i++)
    {
        radio_infos[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<mac_operations.size(); i++)
    {
        mac_operations[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<tx_powers.size(); i++)
    {
        tx_powers[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<ds_ctrls.size(); i++)
    {
        ds_ctrls[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<ofdm_ctrls.size(); i++)
    {
        ofdm_ctrls[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<pay_loads.size(); i++)
    {
        pay_loads[i].LoadFrom(kv, toString(i));
    }

    return 0;
}
int CCapwapChangeStateReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    do {
        CRadioOperationalStateTlv state;
        state.Parse(buffer);
        if (state.isValid())
            radio_operation_states.push_back(state);
        else
            break;
    } while(true);

    result.Parse(buffer);
    pay_load.Parse(buffer);

    return 0;
}
int CCapwapChangeStateReq::SaveTo(string &str)
{
    for (size_t i=0; i<radio_operation_states.size(); i++)
    {
        radio_operation_states[i].SaveTo(str, toString(i));
    }
    result.SaveTo(str);
    pay_load.SaveTo(str);

    return 0;
}
int CCapwapChangeStateReq::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapChangeStateReq::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapChangeStateRsp::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapChangeStateRsp::SaveTo(string &str)
{
    return 0;
}
int CCapwapChangeStateRsp::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);

    return 0;
}
int CCapwapChangeStateRsp::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapDataTransferReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    data_transfer.Parse(buffer);
    pay_load.Parse(buffer);

    return 0;
}
int CCapwapDataTransferReq::SaveTo(string &str)
{
    data_transfer.SaveTo(str);
    pay_load.SaveTo(str);

    return 0;
}
int CCapwapDataTransferReq::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapDataTransferReq::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapDataTransferRsp::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapDataTransferRsp::SaveTo(string &str)
{
    return 0;
}
int CCapwapDataTransferRsp::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);

    result.Assemble(buffer);

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);

    return 0;
}
int CCapwapDataTransferRsp::LoadFrom(kvlist &kv)
{
    result.LoadFrom(kv);

    return 0;
}

int CCapwapEchoReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    pay_load.Parse(buffer);
    return 0;
}
int CCapwapEchoReq::SaveTo(string &str)
{
    pay_load.SaveTo(str);
    return 0;
}
int CCapwapEchoReq::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapEchoReq::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapEchoRsp::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    return 0;
}
int CCapwapEchoRsp::SaveTo(string &str)
{
    return 0;
}
int CCapwapEchoRsp::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);
    return 0;
}
int CCapwapEchoRsp::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapAPConfReq::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapAPConfReq::SaveTo(string &str)
{
    return 0;
}
int CCapwapAPConfReq::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);

    // outside_auth_conf.Assemble(buffer);
    // add_dst_white.Assemble(buffer);
    // del_dst_white.Assemble(buffer);
    // nat_dhcp_conf.Assemble(buffer);
    report_station_enable.Assemble(buffer);
    roming_conf.Assemble(buffer);
    wp_conf.Assemble(buffer);
    rfg_conf.Assemble(buffer);
    ap_lb_conf.Assemble(buffer);
    rate_set_conf.Assemble(buffer);
    low_rssi_conf.Assemble(buffer);
    // cloud_ac_conf.Assemble(buffer);
    // local_ac_conf.Assemble(buffer);
    // connection_mode_conf.Assemble(buffer);
    // running_mode_conf.Assemble(buffer);
    // spectrum_analysis_conf.Assemble(buffer);
    lan_vlan_conf.Assemble(buffer);
    report_station_interval.Assemble(buffer);
    // ap_password_conf.Assemble(buffer);
    audit_appri_conf.Assemble(buffer);
    lan_portal_conf.Assemble(buffer);
    // add_dst_black.Assemble(buffer);
    // del_dst_black.Assemble(buffer);
    pay_load.Assemble(buffer);

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);
    return 0;
}
int CCapwapAPConfReq::LoadFrom(kvlist &kv)
{
    // outside_auth_conf.LoadFrom(kv);
    // add_dst_white.LoadFrom(kv);
    // del_dst_white.LoadFrom(kv);
    // nat_dhcp_conf.LoadFrom(kv);
    report_station_enable.LoadFrom(kv);
    roming_conf.LoadFrom(kv);
    wp_conf.LoadFrom(kv);
    rfg_conf.LoadFrom(kv);
    ap_lb_conf.LoadFrom(kv);
    rate_set_conf.LoadFrom(kv);
    low_rssi_conf.LoadFrom(kv);
    // cloud_ac_conf.LoadFrom(kv);
    // local_ac_conf.LoadFrom(kv);
    // connection_mode_conf.LoadFrom(kv);
    // running_mode_conf.LoadFrom(kv);
    // spectrum_analysis_conf.LoadFrom(kv);
    lan_vlan_conf.LoadFrom(kv);
    report_station_interval.LoadFrom(kv);
    // ap_password_conf.LoadFrom(kv);
    audit_appri_conf.LoadFrom(kv);
    lan_portal_conf.LoadFrom(kv);
    // add_dst_black.LoadFrom(kv);
    // del_dst_black.LoadFrom(kv);
    pay_load.LoadFrom(kv);

    return 0;
}

int CCapwapWlanConfigReq::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapWlanConfigReq::SaveTo(string &str)
{
    return 0;
}
int CCapwapWlanConfigReq::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);
    for (size_t i=0; i<add_wlans.size(); i++)
    {
        add_wlans[i].Assemble(buffer);
        pay_loads[i].Assemble(buffer);
    }

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);
    return 0;
}
int CCapwapWlanConfigReq::LoadFrom(kvlist &kv)
{
    for (size_t i=0; i<add_wlans.size(); i++)
    {
        add_wlans[i].LoadFrom(kv, toString(i));
        pay_loads[i].LoadFrom(kv, toString(i));
    }

    return 0;
}
int CCapwapWlanConfigRsp::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    result.Parse(buffer);
    pay_load.Parse(buffer);

    return 0;
}
int CCapwapWlanConfigRsp::SaveTo(string &str)
{
    result.SaveTo(str);
    pay_load.SaveTo(str);

    return 0;
}

int CCapwapWlanConfigRsp::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapWlanConfigRsp::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapAPConfRsp::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    result.Parse(buffer);
    pay_load.Parse(buffer);

    return 0;
}
int CCapwapAPConfRsp::SaveTo(string &str)
{
    result.SaveTo(str);
    pay_load.SaveTo(str);

    return 0;
}
int CCapwapAPConfRsp::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapAPConfRsp::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapWTPEventReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    add_station.Parse(buffer);
    del_station.Parse(buffer);
    ds_ctrl.Parse(buffer);

    pay_load.Parse(buffer);

    return 0;
}
int CCapwapWTPEventReq::SaveTo(string &str)
{
    add_station.SaveTo(str);
    del_station.SaveTo(str);
    ds_ctrl.SaveTo(str);
    pay_load.SaveTo(str);

    return 0;
}
int CCapwapWTPEventReq::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapWTPEventReq::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapWTPEventRsp::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapWTPEventRsp::SaveTo(string &str)
{
    return 0;
}
int CCapwapWTPEventRsp::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);

    return 0;
}
int CCapwapWTPEventRsp::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapAPInfoReq::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    conf_info.Parse(buffer);
    conf_info_all.Parse(buffer);

    pay_load.Parse(buffer);

    return 0;
}
int CCapwapAPInfoReq::SaveTo(string &str)
{
    conf_info.SaveTo(str, "");
    conf_info_all.SaveTo(str, "");

    pay_load.SaveTo(str, "");

    return 0;
}
int CCapwapAPInfoReq::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapAPInfoReq::LoadFrom(kvlist &kv)
{
    return 0;
}

int CCapwapAPInfoRsp::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapAPInfoRsp::SaveTo(string &str)
{
    return 0;
}
int CCapwapAPInfoRsp::Assemble(CBuffer &buffer)
{
    CCapwapHeader::Assemble(buffer);

    result.Assemble(buffer);

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);

    return 0;
}
int CCapwapAPInfoRsp::LoadFrom(kvlist &kv)
{
    result.LoadFrom(kv, "");

    return 0;
}

int CCapwapConfigUpdateReq::Parse(CBuffer &buffer)
{
    return 0;
}
int CCapwapConfigUpdateReq::SaveTo(string &str)
{
    return 0;
}
int CCapwapConfigUpdateReq::Assemble(CBuffer &buffer)
{
    size_t i = 0;
    CCapwapHeader::Assemble(buffer);

    ac_time_stamp.Assemble(buffer);
    for (i=0; i<radio_admin_states.size(); i++)
    {
        radio_admin_states[i].Assemble(buffer);
    }
    for (i=0; i<radio_confs.size(); i++)
    {
        radio_confs[i].Assemble(buffer);
    }
    for (i=0; i<mac_operations.size(); i++)
    {
        mac_operations[i].Assemble(buffer);
    }
    for (i=0; i<radio_infos.size(); i++)
    {
        radio_infos[i].Assemble(buffer);
    }
    for (i=0; i<tx_powers.size(); i++)
    {
        tx_powers[i].Assemble(buffer);
    }
    for (i=0; i<ds_ctrls.size(); i++)
    {
        ds_ctrls[i].Assemble(buffer);
    }
    for (i=0; i<ofdm_ctrls.size(); i++)
    {
        ofdm_ctrls[i].Assemble(buffer);
    }
    for (i=0; i<pay_loads.size(); i++)
    {
        pay_loads[i].Assemble(buffer);
    }
    image_identifier.Assemble(buffer);
    auto_channel_select.Assemble(buffer);

    element_end = buffer.GetOffset();
    ReAssembleCtrlHeader(buffer);

    return 0;
}
int CCapwapConfigUpdateReq::LoadFrom(kvlist &kv)
{
    size_t i;
    ac_time_stamp.LoadFrom(kv, "");
    for (i=0; i<radio_admin_states.size(); i++)
    {
        radio_admin_states[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<radio_confs.size(); i++)
    {
        radio_confs[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<mac_operations.size(); i++)
    {
        mac_operations[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<radio_infos.size(); i++)
    {
        radio_infos[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<tx_powers.size(); i++)
    {
        tx_powers[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<ds_ctrls.size(); i++)
    {
        ds_ctrls[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<ofdm_ctrls.size(); i++)
    {
        ofdm_ctrls[i].LoadFrom(kv, toString(i));
    }
    for (i=0; i<pay_loads.size(); i++)
    {
        pay_loads[i].LoadFrom(kv, toString(i));
    }
    image_identifier.LoadFrom(kv, toString(i));
    auto_channel_select.LoadFrom(kv, toString(i));

    return 0;
}

int CCapwapConfigUpdateRsp::Parse(CBuffer &buffer)
{
    CCapwapHeader::Parse(buffer);

    result.Parse(buffer);
    pay_load.Parse(buffer);

    return 0;
}
int CCapwapConfigUpdateRsp::SaveTo(string &str)
{
    result.SaveTo(str, "");
    pay_load.SaveTo(str, "");

    return 0;
}
int CCapwapConfigUpdateRsp::Assemble(CBuffer &buffer)
{
    return 0;
}
int CCapwapConfigUpdateRsp::LoadFrom(kvlist &kv)
{
    return 0;
}








uint32_t capwap_packet_type(CCapwapHeader *cwheader)
{
    return cwheader->getMessageType();
}

CCapwapHeader * capwap_get_packet(int packet_type)
{
    try{
        switch(packet_type)
        {
        case CAPWAP_PACKET_TYPE_DISCOVERY_REQ:
            return new CCapwapDiscoveryReq;
        case CAPWAP_PACKET_TYPE_DISCOVERY_RSP:
            return new CCapwapDiscoveryRsp;
        case CAPWAP_PACKET_TYPE_JOIN_REQ:
            return new CCapwapJoinReq;
        case CAPWAP_PACKET_TYPE_JOIN_RSP:
            return new CCapwapJoinRsp;
        case CAPWAP_PACKET_TYPE_CONFIG_STATUS_REQ:
            return new CCapwapConfigureStatusReq;
        case CAPWAP_PACKET_TYPE_CONFIG_STATUS_RSP:
            return new CCapwapConfigureStatusRsp;
        case CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_REQ:
            return new CCapwapChangeStateReq;
        case CAPWAP_PACKET_TYPE_CHANGE_STATE_EVENT_RSP:
            return new CCapwapChangeStateRsp;
        case CAPWAP_PACKET_TYPE_DATA_TRANSFER_REQ:
            return new CCapwapDataTransferReq;
        case CAPWAP_PACKET_TYPE_DATA_TRANSFER_RSP:
            return new CCapwapDataTransferRsp;
        case CAPWAP_PACKET_TYPE_ECHO_REQ:
            return new CCapwapEchoReq;
        case CAPWAP_PACKET_TYPE_ECHO_RSP:
            return new CCapwapEchoRsp;
        case CAPWAP_PACKET_TYPE_AP_CONFIG_REQ:
            return new CCapwapAPConfReq;
        case CAPWAP_PACKET_TYPE_AP_CONFIG_RSP:
            return new CCapwapAPConfRsp;
        case CAPWAP_PACKET_TYPE_WLAN_CONFIG_REQ:
            return new CCapwapWlanConfigReq;
        case CAPWAP_PACKET_TYPE_WLAN_CONFIG_RSP:
            return new CCapwapWlanConfigRsp;
        case CAPWAP_PACKET_TYPE_WTP_EVENT_REQ:
            return new CCapwapWTPEventReq;
        case CAPWAP_PACKET_TYPE_WTP_EVENT_RSP:
            return new CCapwapWTPEventRsp;
        case CAPWAP_PACKET_TYPE_AP_INFO_REQ:
            return new CCapwapAPInfoReq;
        case CAPWAP_PACKET_TYPE_AP_INFO_RSP:
            return new CCapwapAPInfoRsp;
        case CAPWAP_PACKET_TYPE_CONFIG_UPDATE_REQ:
            return new CCapwapConfigUpdateReq;
        case CAPWAP_PACKET_TYPE_CONFIG_UPDATE_RSP:
            return new CCapwapConfigUpdateRsp;
        default:
            dlog(LOG_ERR, "%s.%d Unknown this TYPE %d", __FILE__, __LINE__, packet_type);
            break;
        }
    }
    catch(...){
        return NULL;
    }
    return NULL;
}

