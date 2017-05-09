#include <iostream>
using namespace std;

#include <vector>

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "capwap_elements.h"
#include "buffer.h"
#include "dstring.h"

int CWTPBoardDataTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive32(vendor_identify);

    dlog(LOG_DEBUG, "vendor-identify %#x", vendor_identify);

    if (DUNCHONG_IDENTIFY != vendor_identify)
    {
        dlog(LOG_ERR, "vendor-identifier error %#x", vendor_identify);
        return 0;
    }

    model_number.Parse(buffer);
    serial_number.Parse(buffer);
    board_id.Parse(buffer);
    board_revision.Parse(buffer);
    board_macaddress.Parse(buffer);

    return 0;
}

int CWTPBoardDataTlv::SaveTo(string &str, string ex)
{
    if (isValid())
    {
        model_number.SaveTo(str);
        serial_number.SaveTo(str);
        board_id.SaveTo(str);
        board_revision.SaveTo(str);
        board_macaddress.SaveTo(str);
    }

    return 0;
}

int CVendorSpecPayLoadTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
    {
        return 0;
    }

    buffer.retrive32(vendor_identifier);

    if (DUNCHONG_IDENTIFY != vendor_identifier)
    {
        dlog(LOG_ERR, "vendor-identifier error %#x", vendor_identifier);
        return 0;
    }

    while (buffer.GetOffset() < buffer.GetLength())
    {
        int start = buffer.GetOffset();
        int end = start;

        actl_user_info.Parse(buffer);
        actl_sta_info.Parse(buffer);
        station_ip.Parse(buffer);
        sta_state.Parse(buffer);
        ap_lanip.Parse(buffer);
        traffic_statics_conf.Parse(buffer);
        result.Parse(buffer);

        end = buffer.GetOffset();
        if (start == end)
        {
            SkipTlv(buffer);
        }
    }

    return 0;
}

int CVendorSpecPayLoadTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);
    buffer.store32(vendor_identifier);

    ap_trans.Assemble(buffer);
    radio_conf.Assemble(buffer);
    ap_space_info.Assemble(buffer);
    abp_radio_conf.Assemble(buffer);
    radio_probe_conf.Assemble(buffer);
    echo_conf.Assemble(buffer);
    traffic_statics_conf.Assemble(buffer);
    packet_power_conf.Assemble(buffer);
    channel_reuse_conf.Assemble(buffer);
    time_stamp.Assemble(buffer);
    by_pass.Assemble(buffer);
    vlan_conf.Assemble(buffer);
    mcast_enhance.Assemble(buffer);
    wx_auth.Assemble(buffer);
    wlan_info.Assemble(buffer);
    actl_user_info.Assemble(buffer);
    ntp_server.Assemble(buffer);
    portal_custom.Assemble(buffer);
    reverse_ssh.Assemble(buffer);

    return 0;
}
int CVendorSpecPayLoadTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    actl_user_info.SaveTo(str);
    actl_sta_info.SaveTo(str);
    station_ip.SaveTo(str);
    sta_state.SaveTo(str);
    ap_lanip.SaveTo(str);
    traffic_statics_conf.SaveTo(str);
    result.SaveTo(str, "");

    return 0;
}
int CVendorSpecPayLoadTlv::LoadFrom(kvlist &kv,string ex)
{
    if (!isValid())
        return 0;

    vendor_identifier = DUNCHONG_IDENTIFY;
    _elength += 4;

    if (ap_trans.isValid())
    {
        ap_trans.LoadFrom(kv, ex);
        _elength += ap_trans.length() + 4;
    }
    if (radio_conf.isValid())
    {
        radio_conf.LoadFrom(kv, ex);
        _elength += radio_conf.length() + 4;
    }
    if (ap_space_info.isValid())
    {
        ap_space_info.LoadFrom(kv, ex);
        _elength += ap_space_info.length() + 4;
    }
    if (abp_radio_conf.isValid())
    {
        abp_radio_conf.LoadFrom(kv, ex);
        _elength += abp_radio_conf.length() + 4;
    }
    if (radio_probe_conf.isValid())
    {
        radio_probe_conf.LoadFrom(kv, ex);
        _elength += radio_probe_conf.length() + 4;
    }
    if (echo_conf.isValid())
    {
        echo_conf.LoadFrom(kv, ex);
        _elength += echo_conf.length() + 4;
    }
    if (traffic_statics_conf.isValid())
    {
        traffic_statics_conf.LoadFrom(kv, ex);
        _elength += traffic_statics_conf.length() + 4;
    }
    if (packet_power_conf.isValid())
    {
        packet_power_conf.LoadFrom(kv, ex);
        _elength += packet_power_conf.length() + 4;
    }
    if (channel_reuse_conf.isValid())
    {
        channel_reuse_conf.LoadFrom(kv, ex);
        _elength += channel_reuse_conf.length() + 4;
    }
    if (time_stamp.isValid())
    {
        time_stamp.LoadFrom(kv, ex);
        _elength += time_stamp.length() + 4;
    }
    if (by_pass.isValid())
    {
        by_pass.LoadFrom(kv, ex);
        _elength += by_pass.length() + 4;
    }
    if (vlan_conf.isValid())
    {
        vlan_conf.LoadFrom(kv, ex);
        _elength += vlan_conf.length() + 4;
    }
    if (mcast_enhance.isValid())
    {
        mcast_enhance.LoadFrom(kv, ex);
        _elength += mcast_enhance.length() + 4;
    }
    if (wx_auth.isValid())
    {
        wx_auth.LoadFrom(kv, ex);
        _elength += wx_auth.length() + 4;
    }
    if (wlan_info.isValid())
    {
        wlan_info.LoadFrom(kv, ex);
        _elength += wlan_info.length() + 4;
    }
    if (actl_user_info.isValid())
    {
        actl_user_info.LoadFrom(kv, ex);
        _elength += actl_user_info.length() + 4;
    }
    if (ntp_server.isValid())
    {
        ntp_server.LoadFrom(kv, ex);
        _elength += ntp_server.length() + 4;
    }
    if (portal_custom.isValid())
    {
        portal_custom.LoadFrom(kv, ex);
        _elength += portal_custom.length() + 4;
    }
    if (reverse_ssh.isValid())
    {
        reverse_ssh.LoadFrom(kv, ex);
        _elength += reverse_ssh.length() + 4;
    }

    return 0;
}

int CACNameTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)value.c_str(), value.length());
    return 0;
}
int CACNameTlv::LoadFrom(kvlist &kv,string ex)
{
    if (!isValid())
        return 0;

    value.assign(GetValue(kv, STRING_AC_NAME));

    _elength = value.length();

    return 0;
}

int CCtrlIpv4AddressTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(ip_addr);
    buffer.store16(wtp_count);
    return 0;
}
int CCtrlIpv4AddressTlv::LoadFrom(kvlist &kv,string ex)
{

    if (!isValid())
        return 0;

    string sip_addr = GetValue(kv, STRING_CTRL_IPV4_ADDR);
    inet_pton(AF_INET, sip_addr.c_str(), &ip_addr);
    ip_addr = ntohl(ip_addr);

    _elength = 6;

    return 0;
}

int CWTPRadioInfoTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(radio_id);
    buffer.retrive32(u_radio_type.radio_type);

    return 0;
}

int CWTPRadioInfoTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + ex + "=" + toString(radio_id) + ";");
    str.append(TO_STR(STRING_RADIO_RADIO_TYPE) + ex + "=" + toString(u_radio_type.radio_type) + ";");

    return 0;
}
int CWTPRadioInfoTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store32(u_radio_type.radio_type);

    return 0;
}
int CWTPRadioInfoTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));

    u_radio_type.radio_type = toInt(GetValue(kv, TO_STR(STRING_RADIO_RADIO_TYPE) + ex));

    _elength = 5;

    return 0;
}

int CWTPDescriptorTlv::Parse(CBuffer &buffer)
{
    if ( 0 != ParseEH(buffer) )
        return 0;

    buffer.retrive8(max_radios);
    buffer.retrive8(radio_inuse);
    buffer.retrive8(encrypt_number);
    buffer.retrive8(u_encrypt_wbid.encrypt_wbid);
    buffer.retrive16(encrypt_capabilities);

    hardware_version.Parse(buffer);
    software_version.Parse(buffer);

    return 0;
}

int CWTPDescriptorTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    // str.append(STRING_MAX_RADIOS"=" + toString(max_radios) + ";");
    // str.append(STRING_RADIO_INUSE"=" + toString(radio_inuse) + ";");
    // str.append(STRING_ENCRYPT_NUMBER"=" + toString(encrypt_number) + ";");
    // str.append(STRING_ENCRYPT_WBID"=" + toString(u_encrypt_wbid.s.wbid) + ";");
    // str.append(STRING_ENCRYPT_CAPABILITIES"=" + toString(encrypt_capabilities) + ";");

    hardware_version.SaveTo(str);
    software_version.SaveTo(str);

    return 0;
}

int CResultTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive32(result);
    return 0;
}
int CResultTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;
    str.append(STRING_RESULT_CODE"=" + toString(result) + ";");
    return 0;
}
int CResultTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(result);

    return 0;
}
int CResultTlv::LoadFrom(kvlist &kv,string ex)
{
    if (!isValid())
        return 0;

    string sresult = GetValue(kv, STRING_RESULT_CODE);
    result = toInt(sresult);
    _elength += 4;

    return 0;
}

int CWTPRadioConfTlv::Parse(CBuffer &buffer)
{
    if ( 0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(radio_id);
    buffer.retrive8(short_preamble);
    buffer.retrive8(bssids_number);
    buffer.retrive8(dtim_period);
    buffer.retriverawbytes(bssid, sizeof(bssid));
    buffer.retrive16(beacon_period);
    buffer.retriverawbytes(country_string, sizeof(country_string));

    return 0;
}
int CWTPRadioConfTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    char buf[32] = {0};
    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(bssid));

    str.append(STRING_RADIO_ID + ex + "="
               + toString(radio_id) + ";");
    str.append(TO_STR(STRING_RADIO_SHORT_PREAMBLE) + ex + "="
               + toString(short_preamble) + ";");
    // str.append(STRING_BSSIDS_NUMBER + ex + "="
    //            + toString(bssids_number) + ";");
    str.append(TO_STR(STRING_RADIO_DTIM_INTV) + ex + "="
               + toString(dtim_period) + ";");
    // str.append(STRING_BSSID + ex + "="
    //            + toString(buf) + ";");
    str.append(TO_STR(STRING_RADIO_BEACON_INTV) + ex + "="
               + toString(beacon_period) + ";");
//    str.append(STRING_COUNTRY_STRING + ex + "="
//               + country_string + ";");

    return 0;
}
int CWTPRadioConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(short_preamble);
    buffer.store8(bssids_number);
    buffer.store8(dtim_period);
    buffer.storerawbytes(bssid, sizeof(bssid));
    buffer.store16(beacon_period);
    buffer.storerawbytes(country_string, sizeof(country_string));

    return 0;
}
int CWTPRadioConfTlv::LoadFrom(kvlist &kv, string ex)
{
    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    _elength += 1;

    short_preamble = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_SHORT_PREAMBLE) + ex));
    _elength += 1;

    // bssids_number = (uint8_t)toInt(GetValue(kv, STRING_BSSIDS_NUMBER + ex));
    _elength += 1;

    dtim_period = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_DTIM_INTV) + ex));
    _elength += 1;

    // bssid 下发为空
    _elength += 6;

    beacon_period = (uint16_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_BEACON_INTV) + ex));
    _elength += 2;

    string scountry = GetValue(kv, TO_STR(STRING_RADIO_COUNTRY_CODE) + ex);
    strncpy((char *)country_string, scountry.c_str(), sizeof(country_string));
    _elength += 4;

    return 0;
}

int CTxPowerTlv::Parse(CBuffer &buffer)
{
    if ( 0 != ParseEH(buffer) )
        return 0;

    buffer.retrive8(radio_id);
    buffer.retrive8(reserved);
    buffer.retrive16(cur_tx_power);
    return 0;
}
int CTxPowerTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + ex + "="
               + toString(radio_id) + ";");
    str.append(TO_STR(STRING_RADIO_TXPOWER) + ex + "="
               + toString(cur_tx_power) + ";");

    return 0;
}
int CTxPowerTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(reserved);
    buffer.store16(cur_tx_power);

    return 0;
}
int CTxPowerTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    cur_tx_power = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_TXPOWER) + ex));

    _elength = 4;

    return 0;
}

int CDSCtrlTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(radio_id);
    buffer.retrive8(reserved);
    buffer.retrive8(cur_channel);
    buffer.retrive8(cur_cca);
    buffer.retrive32(energy_detect_threshold);
    return 0;
}
int CDSCtrlTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + ex + "="
               + toString(radio_id) + ";");
    str.append(TO_STR(STRING_RADIO_CHANNEL) + ex + "="
               + toString(cur_channel) + ";");
    str.append(TO_STR(STRING_RADIO_CCA_ADJ) + ex + "="
               + toString(cur_cca) + ";");
    str.append(TO_STR(STRING_RADIO_CCA_THR) + ex + "="
               + toString(energy_detect_threshold) + ";");
    return 0;
}
int CDSCtrlTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(reserved);
    buffer.store8(cur_channel);
    buffer.store8(cur_cca);
    buffer.store32(energy_detect_threshold);

    return 0;
}
int CDSCtrlTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    // reserved
    cur_channel = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_CHANNEL) + ex));
    // cur_cca = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_CCA_ADJ) + ex));
    // energy_detect_threshold = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_CCA_THR) + ex));

    _elength = 8;
    return 0;
}

int CRadioAdminStateTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(radio_state);
    return 0;
}
int CRadioAdminStateTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    radio_state = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_ENABLE) + ex));

    _elength = 2;
    return 0;
}

int CMacOperationTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(reserved);
    buffer.store16(rts_thr);
    buffer.store8(short_retry);
    buffer.store8(long_retry);
    buffer.store16(frag_thr);
    buffer.store32(tx_msdu_lifetime);
    buffer.store32(rx_msdu_lifetime);

    return 0;
}
int CMacOperationTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    // reserved
    rts_thr = (uint16_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_RTS) + ex));
    // short_retry = (uint8_t)toInt(GetValue(kv, STRING_SHORT_RETRY + ex));
    // long_retry = (uint8_t)toInt(GetValue(kv, STRING_LONG_RETRY + ex));
    frag_thr = (uint16_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_FRAGMENT) + ex));
    // tx_msdu_lifetime = toInt(GetValue(kv, STRING_TX_MSDU_LIFETIME + ex));
    // rx_msdu_lifetime = toInt(GetValue(kv, STRING_RX_MSDU_LIFETIME + ex));

    _elength = 16;

    return 0;
}

int COFDMCtrlTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(reserved);
    buffer.store8(current_channel);
    buffer.store8(band_width);
    buffer.store32(ti_threshold);

    return 0;
}
int COFDMCtrlTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    // reserved
    current_channel = (uint8_t)toInt(GetValue(kv, TO_STR(STRING_RADIO_CHANNEL) + ex));
    // band_width = (uint8_t)toInt(GetValue(kv, STRING_RADIO_CHWIDTH + ex));
    // ti_threshold = toInt(GetValue(kv, STRING_TI_THRESHOLD + ex));

    _elength = 8;

    return 0;
}

int CRadioOperationalStateTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(radio_id);
    buffer.retrive8(radio_op_state);
    buffer.retrive8(radio_op_cause);

    return 0;
}

int CRadioOperationalStateTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + ex + "=" + toString(radio_id) + ";");
    str.append(TO_STR(STRING_RADIO_ENABLE) + ex + "=" + toString(radio_op_state) + ";");
    // str.append(STRING_RADIO_CAUSE + ex + "=" + toString(radio_op_cause) + ";");
    return 0;
}

int CDataTransferTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(data_type);
    buffer.retrive8(data_mode);
    buffer.retrive16(data_length);
    data.assign((char*)buffer.GetPtr(), data_length);
    buffer.retriverawbytes(NULL, data_length);

    return 0;
}
int CDataTransferTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    // str.append(STRING_DATA_TYPE"=" + toString(data_type) + ";");
    // str.append(STRING_DATA_MODE"=" + toString(data_mode) + ";");
    // str.append(STRING_DATA_DATA"=" + data + ";");

    return 0;
}

int CAPReportStationInfoEnableTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(enable);

    return 0;
}

int CAPReportStationInfoEnableTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    enable = toInt8(GetValue(kv, TO_STR(STRING_REPORT_STATION_INFO_ENABLE)));

    _elength = 1;
    return 0;
}

int CRomingConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.store8(enable);
    buffer.store32(signal);

    return 0;
}

int CRomingConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    enable = toInt8(GetValue(kv, TO_STR(STRING_ROMING_CONFIG_ENABLE)));
    signal = toInt(GetValue(kv, TO_STR(STRING_ROMING_CONFIG_SIGNAL)));

    _elength = 5;

    return 0;
}

int CWirelessLocationConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(wp_enable);
    buffer.store8(wp_intv);
    buffer.store8(wp_server_ip_type);
    if (IP_TYPE_IPV4 == wp_server_ip_type)
    {
        buffer.storerawbytes(wp_server_ip, 4);
    }
    else
    {
        buffer.storerawbytes(wp_server_ip, 16);
    }
    buffer.store16(wp_server_port);
    buffer.store8(wp_scan_type);
    buffer.store8(wp_code);
    buffer.store8(wp_proto);

    buffer.store8(ef_enable);
    buffer.store8(ef_code);
    buffer.store8(ef_proto);
    buffer.store8(ef_intv);
    buffer.store8(ef_scan_type);
    buffer.store8(ef_server_ip_type);
    if (IP_TYPE_IPV4 == ef_server_ip_type)
        buffer.storerawbytes(ef_server_ip, 4);
    else
        buffer.storerawbytes(ef_server_ip, 16);
    buffer.store16(ef_server_port);

    buffer.store16(we_ad_intv);
    buffer.store32(we_channel_2g);
    buffer.store32(we_channel_5g);
    buffer.store16(we_ad_rssi);

    return 0;

}
int CWirelessLocationConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    wp_enable = toInt8(GetValue(kv, TO_STR(STRING_WP_ENABLE)));
    wp_intv   = toInt8(GetValue(kv, TO_STR(STRING_WP_INTERVAL)));
    // wp_server_ip_type = toInt8(GetValue(kv, STRING_WP_SERVER_IP_TYPE));
    wp_server_ip_type = IP_TYPE_IPV4;

    _elength += 3;

    if (IP_TYPE_IPV4 == wp_server_ip_type)
    {
        int ipaddr[4] = {0};
        string addr = GetValue(kv, TO_STR(STRING_WP_SERVER_IP));
        sscanf(addr.c_str(), IP_ADDR_FMT, IP_ADDR_RVAL(ipaddr));
        wp_server_ip[0] = ipaddr[0];
        wp_server_ip[1] = ipaddr[1];
        wp_server_ip[2] = ipaddr[2];
        wp_server_ip[3] = ipaddr[3];
        _elength += 4;
    }
    else
    {
        _elength += 16;
    }
    wp_server_port = toInt16(GetValue(kv, TO_STR(STRING_WP_SERVER_PORT)));
    wp_scan_type = toInt8(GetValue(kv, TO_STR(STRING_WP_SCAN_TYPE)));
    wp_code = toInt8(GetValue(kv, TO_STR(STRING_WP_CODE)));
    wp_proto = toInt8(GetValue(kv, TO_STR(STRING_WP_PROTO)));

    ef_enable = toInt8(GetValue(kv, TO_STR(STRING_EF_ENABLE)));
    ef_code = toInt8(GetValue(kv, TO_STR(STRING_EF_CODE)));
    ef_proto = toInt8(GetValue(kv, TO_STR(STRING_EF_PROTO)));
    ef_intv = toInt8(GetValue(kv, TO_STR(STRING_EF_INTERVAL)));
    ef_scan_type = toInt8(GetValue(kv, TO_STR(STRING_EF_SCAN_TYPE)));
    // ef_server_ip_type = toInt8(GetValue(kv, TO_STR(STRING_EF_SERVER_IP_TYPE)));
    ef_server_ip_type = IP_TYPE_IPV4;

    _elength += 11;

    if (IP_TYPE_IPV4 == ef_server_ip_type)
    {
        int ipaddr[4] = {0};
        string addr = GetValue(kv, TO_STR(STRING_EF_SERVER_IP));
        sscanf(addr.c_str(), IP_ADDR_FMT, IP_ADDR_RVAL(ipaddr));
        ef_server_ip[0] = ipaddr[0];
        ef_server_ip[1] = ipaddr[1];
        ef_server_ip[2] = ipaddr[2];
        ef_server_ip[3] = ipaddr[3];
        _elength += 4;
    }
    else
    {
        _elength += 16;
    }
    ef_server_port = toInt16(GetValue(kv, TO_STR(STRING_EF_SERVER_PORT)));

    we_ad_intv = toInt16(GetValue(kv, TO_STR(STRING_WE_AD_INTERVAL)));
    we_channel_2g = toInt(GetValue(kv, TO_STR(STRING_WE_CHANNEL_2G)));
    we_channel_5g = toInt(GetValue(kv, TO_STR(STRING_WE_CHANNEL_5G)));
    we_ad_rssi = toInt16(GetValue(kv, TO_STR(STRING_WE_AD_RSSI)));

    _elength += 14;
    return 0;
}

int CRfgConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(rfg_enable);
    buffer.store8(rfg_assocmax);
    buffer.store8(rfg_timeout);
    buffer.store8(rfg_maxsta);
    buffer.store8(rfg_method);

    return 0;
}
int CRfgConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    rfg_enable = toInt8(GetValue(kv, TO_STR(STRING_RFG_ENABLE)));
    rfg_assocmax = toInt8(GetValue(kv, TO_STR(STRING_RFG_ASSOC_MAX)));
    rfg_timeout = toInt8(GetValue(kv, TO_STR(STRING_RFG_TIMEOUT)));
    rfg_maxsta = toInt8(GetValue(kv, TO_STR(STRING_RFG_MAX_STA)));
    rfg_method = toInt8(GetValue(kv, TO_STR(STRING_RFG_METHOD)));

    _elength = 5;

    return 0;
}

int CAPLoadBalanceConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.store8(load_balance_enable);
    buffer.store16(load_balance_threshold);
    buffer.store16(load_balance_interval);

    return 0;
}
int CAPLoadBalanceConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    load_balance_enable = toInt8(GetValue(kv, TO_STR(STRING_AP_LOADBALANCE_ENABLE)));
    load_balance_threshold = toInt16(GetValue(kv, TO_STR(STRING_AP_LOADBALANCE_THRESHOLD)));
    load_balance_interval = toInt16(GetValue(kv, TO_STR(STRING_AP_LOADBALANCE_INTERVAL)));

    _elength = 5;
    return 0;
}

int CRateSetConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_type_11a_len);
    buffer.storerawbytes(radio_type_11a_rate, radio_type_11a_len);
    buffer.store8(radio_type_11bg_len);
    buffer.storerawbytes(radio_type_11bg_rate, radio_type_11bg_len);
    buffer.store8(radio_type_11n_len);
    buffer.storerawbytes(radio_type_11n_rate, radio_type_11n_len);
    buffer.store8(radio_type_11ac_len);
    buffer.storerawbytes(radio_type_11ac_rate, radio_type_11ac_len);

    return 0;
}

int CRateSetConfTlv::LoadFrom(kvlist &kv, string ex)
{
    uint32_t rate;
    if (!isValid())
        return 0;

    radio_type_11a_len = 1;
    rate = toInt(GetValue(kv, TO_STR(STRING_RATE_SET_11A)));
    radio_type_11a_rate[0] = rate & 0xff;
    radio_type_11a_rate[1] = rate & 0xff00;
    radio_type_11a_rate[2] = rate & 0xff0000;
    radio_type_11a_rate[3] = rate & 0xff000000;

    radio_type_11bg_len = 2;
    rate = toInt(GetValue(kv, TO_STR(STRING_RATE_SET_11BG)));
    radio_type_11bg_rate[0] = rate & 0xff;
    radio_type_11bg_rate[1] = rate & 0xff00;
    radio_type_11bg_rate[2] = rate & 0xff0000;
    radio_type_11bg_rate[3] = rate & 0xff000000;

    radio_type_11n_len = 3;
    rate = toInt(GetValue(kv, TO_STR(STRING_RATE_SET_11N)));
    radio_type_11n_rate[0] = rate & 0xff;
    radio_type_11n_rate[1] = rate & 0xff00;
    radio_type_11n_rate[2] = rate & 0xff0000;
    radio_type_11n_rate[3] = rate & 0xff000000;

    radio_type_11ac_len = 0;
    rate = toInt(GetValue(kv, TO_STR(STRING_RATE_SET_11AC)));
    radio_type_11ac_rate[0] = rate & 0xff;
    radio_type_11ac_rate[1] = rate & 0xff00;
    radio_type_11ac_rate[2] = rate & 0xff0000;
    radio_type_11ac_rate[3] = rate & 0xff000000;

    _elength = 4 + radio_type_11a_len + radio_type_11bg_len
        + radio_type_11n_len + radio_type_11ac_len;

    return 0;
}

int CLowRssiRefuseConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(low_rssi_refuse_enable);
    buffer.store32(low_rssi_threshold);

    return 0;
}
int CLowRssiRefuseConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    low_rssi_refuse_enable = toInt8(GetValue(kv, TO_STR(STRING_LOW_RSSI_REFUSE_ENABLE)));
    low_rssi_threshold = toInt(GetValue(kv, TO_STR(STRING_LOW_RSSI_THRESHOLD)));

    _elength = 5;

    return 0;
}

int CLanVlanConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(lan_vlan_id);

    return 0;
}
int CLanVlanConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    lan_vlan_id = toInt(GetValue(kv, TO_STR(STRING_LAN_VLAN_ID)));
    _elength = 4;

    return 0;
}

int CAPReportStationInfoIntervalTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(interval);

    return 0;
}
int CAPReportStationInfoIntervalTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    interval = toInt(GetValue(kv, TO_STR(STRING_REPORT_STATION_INFO_INTERVAL)));
    _elength = 4;

    return 0;
}

int CAuditAppriConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(audit_enable);
    buffer.store8(appri_enable);
    buffer.store8(remote_sync_enable);
    buffer.retriverawbytes(opaque, sizeof(opaque));

    return 0;
}
int CAuditAppriConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    // audit_enable = toInt(GetValue(kv, STRING_AUDIT_ENABLE));
    // appri_enable = toInt(GetValue(kv, STRING_APPRI_ENABLE));
    // remote_sync_enable = toInt(GetValue(kv, STRING_REMOTE_SYNC_ENABLE));
    // opaque

    _elength = 6;

    return 0;
}

int CLanPortalConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(unitid);
    buffer.store32(portal_enable);
    buffer.store32(portal_url_len);
    buffer.storerawbytes((uint8_t*)portal_url.c_str(), portal_url_len);
//    buffer.storerawbytes(reserved, sizeof(reserved));  // AP未解析字保留字段保留字段无意义

    return 0;
}
int CLanPortalConfTlv::LoadFrom(kvlist &kv, string ex)
{
    // unitid  //  not use
    portal_enable = toInt(GetValue(kv, TO_STR(STRING_LAN_PORTAL_ENABLE)));
    portal_url = GetValue(kv, TO_STR(STRING_LAN_PORTAL_URL));
    portal_url_len = portal_url.length();
    // reserved

    _elength = 12 + portal_url_len;

    return 0;
}

int CConnectionModeConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.store8(connection_mode);

    return 0;
}
int CConnectionModeConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    connection_mode = 1; // 固定为1
    _elength = 1;
    return 0;
}


int CAddWlanTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(wlan_id);
    buffer.store16(u_capability.capability);
    buffer.store8(key_index);
    buffer.store8(key_status);
    buffer.store16(key_len);
    buffer.storerawbytes(key, sizeof(key));
    buffer.store32(group_tsc32);
    buffer.store16(group_tsc16);
    buffer.store8(qos);
    buffer.store8(auth_type);
    buffer.store8(mac_mode);
    buffer.store8(tunnel_mode);
    buffer.store8(hide_ssid);
    buffer.storerawbytes((uint8_t*)ssid.c_str(), ssid.length());

    return 0;
}
int CAddWlanTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    string skey;

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    wlan_id = toInt8(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));
    u_capability.s.qos_enable = toInt16(GetValue(kv, TO_STR(STRING_WLAN_QOS_ENABLE) + ex));
    key_index = toInt8(GetValue(kv, TO_STR(STRING_WLAN_SECURE_KEY_INDEX) + ex));
    // key_status = toInt8(GetValue(kv, STRING_KEY_STATUS + ex)); // addwlan 中不使用此字段
    key_len = toInt16(GetValue(kv, TO_STR(STRING_WLAN_SECURE_KEY_LENGTH) + ex)); // WEP 使用
    skey = GetValue(kv, TO_STR(STRING_WLAN_SECURE_KEY) + ex);
    strncpy((char*)key, skey.c_str(), sizeof(key));
    // group_tsc32 = toInt32(GetValue(kv, STRING_GROUP_TSC32 + ex));
    // group_tsc16 = toInt32(GetValue(kv, STRING_GROUP_TSC16 + ex));
    // qos = 0;  //未启用 使用capability中的某一位表示qos是否开启
    auth_type = toInt8(GetValue(kv, TO_STR(STRING_WLAN_SECURE_TYPE) + ex));
    // mac_mode = toInt8(GetValue(kv, STRING_MAC_MODE + ex));
    // tunnel_mode = toInt8(GetValue(kv, STRING_TUNNEL_ENABLE + ex));
    hide_ssid = toInt8(GetValue(kv, TO_STR(STRING_WLAN_HIDE_SSID) + ex));
    ssid = GetValue(kv, TO_STR(STRING_WLAN_ESSID) + ex);

    _elength = 51 + ssid.length();

    return 0;
}

int CDelWlanTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(wlan_id);

    return 0;
}
int CDelWlanTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    wlan_id = toInt8(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));

    _elength = 2;
    return 0;
}

int CUpdateWlanTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(wlan_id);
    buffer.store16(u_capability.capability);
    buffer.store8(key_index);
    buffer.store8(key_status);
    buffer.store16(key_length);
    buffer.storerawbytes(key, sizeof(key));

    return 0;
}
int CUpdateWlanTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    wlan_id = toInt8(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));
    u_capability.s.qos_enable = toInt8(GetValue(kv, TO_STR(STRING_WLAN_QOS_ENABLE) + ex));
    key_index = toInt8(GetValue(kv, TO_STR(STRING_WLAN_SECURE_KEY_INDEX) + ex));
    key_status = toInt8(GetValue(kv, TO_STR(STRING_WLAN_SECURE_TYPE) + ex));
    key_length = toInt16(GetValue(kv, TO_STR(STRING_WLAN_SECURE_KEY_LENGTH) + ex));
    string skey = GetValue(kv, TO_STR(STRING_WLAN_SECURE_KEY) + ex);
    strncpy((char*)key, skey.c_str(), sizeof(key));

    _elength = 40;
    return 0;
}

int CAddStationTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(radio_id);
    buffer.retrive8(mac_len);
    buffer.retriverawbytes(mac, mac_len);
    buffer.retrive32(vlan_id);
    buffer.retrive8(wlan_id);
    buffer.retrive8(ssid_len);
    ssid.assign((const char *)buffer.GetPtr(), ssid_len);
    buffer.retriverawbytes(NULL, ssid_len);

    return 0;
}
int CAddStationTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;
    char buf[128] = {0};

    str.append(STRING_RADIO_ID"=" + toString(radio_id) + ";");

    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(mac));
    str.append(STRING_STA_MAC"=" + toString(buf) + ";");

    str.append(TO_STR(STRING_WLAN_VLAN_ID)"=" + toString(vlan_id) + ";");
    str.append(TO_STR(STRING_WLAN_WLAN_ID)"=" + toString(wlan_id) + ";");
    str.append(TO_STR(STRING_WLAN_ESSID)"=" + ssid + ";");

    return 0;
}

int CDelStationTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(radio_id);
    buffer.retrive8(mac_len);
    buffer.retriverawbytes(mac, mac_len);
    buffer.retrive8(wlan_id);

    return 0;
}
int CDelStationTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    char buf[128] = {0};
    str.append(STRING_RADIO_ID"=" + toString(radio_id) + ";");

    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(mac));
    str.append(STRING_STA_MAC"=" + toString(buf) + ";");

    str.append(TO_STR(STRING_WLAN_WLAN_ID)"=" + toString(wlan_id) + ";");

    return 0;
}

int CConfInfoEntry::Parse(CBuffer &buffer)
{
    buffer.retrive32(type);
    buffer.retrive32(len);
    value.assign((const char *)buffer.GetPtr(), len);
    buffer.retriverawbytes(NULL, len);

    return 0;
}
int CConfInfoEntry::SaveTo(string &str, string ex)
{
    // str.append(STRING_TYPE + ex + "=" + toString(type) + ";");
    // str.append(STRING_LENGTH + ex + "=" + toString(len) + ";");
    // str.append(STRING_VALUE + ex + "=" + value + ";");

    return 0;
}

int CConfInfoTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive32(count);

    conf_infos.resize(count);
    for (size_t i=0; i<count; i++)
    {
        conf_infos[i].Parse(buffer);
    }

    return 0;
}
int CConfInfoTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    for (size_t i=0; i<conf_infos.size(); i++)
    {
        conf_infos[i].SaveTo(str, toString(i));
    }
    return 0;
}

int CConfInfoAllTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(ac_addr_len);
    ac_addr.assign((const char *)buffer.GetPtr(), ac_addr_len);
    buffer.retriverawbytes(NULL, ac_addr_len);

    buffer.retrive8(cloud_addr_len);
    cloud_addr.assign((const char *)buffer.GetPtr(), cloud_addr_len);
    buffer.retriverawbytes(NULL, cloud_addr_len);

    buffer.retrive8(current_run_mode);

    return 0;
}
int CConfInfoAllTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;
    // str.append(STRING_AC_ADDR"=" + ac_addr + ";");
    // str.append(STRING_CLOUD_ADDR"=" + cloud_addr + ";");
    // str.append(STRING_CURREND_RUNMODE"=" + toString(current_run_mode) + ";");

    return 0;
}

int CACTimeStampTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(timestamp);

    return 0;
}
int CACTimeStampTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    timestamp = time(NULL);

    _elength = 4;

    return 0;
}

int CImageIdentifierTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive32(vendor_id);

    dev_model.Parse(buffer);
    software_version.Parse(buffer);
    file_name.Parse(buffer);
    file_server.Parse(buffer);
    download_type.Parse(buffer);
    ftp_user_name.Parse(buffer);
    ftp_password.Parse(buffer);
    ftp_path.Parse(buffer);

    return 0;
}
int CImageIdentifierTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    dev_model.SaveTo(str, ex);
    software_version.SaveTo(str, ex);
    file_name.SaveTo(str, ex);
    file_server.SaveTo(str, ex);
    download_type.SaveTo(str, ex);
    ftp_user_name.SaveTo(str, ex);
    ftp_password.SaveTo(str, ex);
    ftp_path.SaveTo(str, ex);

    return 0;
}

int CImageIdentifierTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(vendor_id);

    dev_model.Assemble(buffer);
    software_version.Assemble(buffer);
    file_name.Assemble(buffer);
    file_server.Assemble(buffer);
    download_type.Assemble(buffer);
    ftp_user_name.Assemble(buffer);
    ftp_password.Assemble(buffer);
    ftp_path.Assemble(buffer);

    return 0;
}
int CImageIdentifierTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    // vendor_id = toInt(GetValue(kv, STRING_VENDOR_ID));
    _elength += 4;
    if (dev_model.isValid())
    {
        dev_model.LoadFrom(kv);
        _elength += dev_model.length() + 4;
    }
    if (software_version.isValid())
    {
        software_version.LoadFrom(kv);
        _elength += software_version.length() + 4;
    }
    if (file_name.isValid())
    {
        file_name.LoadFrom(kv);
        _elength += file_name.length() + 4;
    }
    if (file_server.isValid())
    {
        file_server.LoadFrom(kv);
        _elength += file_server.length() + 4;
    }
    if (download_type.isValid())
    {
        download_type.LoadFrom(kv);
        _elength += download_type.length() + 4;
    }
    if (ftp_user_name.isValid())
    {
        ftp_user_name.LoadFrom(kv);
        _elength += ftp_user_name.length() + 4;
    }
    if (ftp_password.isValid())
    {
        ftp_password.LoadFrom(kv);
        _elength += ftp_password.length() + 4;
    }
    if (ftp_path.isValid())
    {
        ftp_path.LoadFrom(kv);
        _elength += ftp_path.length() + 4;
    }

    return 0;
}

int CAutoChannelSelectTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);
    buffer.store32(identifier);
    buffer.store8(select_enable);
    buffer.store8(select_2g_channel);
    buffer.store8(select_5g_channel);

    return 0;
}
int CAutoChannelSelectTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    // identifier = toInt(GetValue(kv, STRING_IDENTIFIER));
    // select_enable = toInt8(GetValue(kv, STRING_SELECT_ENABLE));
    // select_2g_channel = toInt8(GetValue(kv, STRING_SELECT_2G_CHANNEL));
    // select_5g_channel = toInt8(GetValue(kv, STRING_SELECT_5G_CHANNEL));

    _elength = 7;
    return 0;
}

int CInitDownLoadTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    return 0;
}
int CInitDownLoadTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;
    return 0;
}

int CImageDataTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    return 0;
}
int CImageDataTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    _elength = 0;
    return 0;
}

int CImageInfoTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(file_size);
    buffer.store64(hash0);
    buffer.store64(hash1);

    return 0;
}
int CImageInfoTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    _elength = 20;

    return 0;
}

int CAPPasswordConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(front_password_len);
    buffer.storerawbytes((uint8_t *)front_password.c_str(), front_password_len);

    buffer.store32(ssh_password_len);
    buffer.storerawbytes((uint8_t *)ssh_password.c_str(), ssh_password_len);

    return 0;
}
int CAPPasswordConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    front_password = GetValue(kv, STRING_FRONT_PASSWORD);
    front_password_len = front_password.length();

    ssh_password = GetValue(kv, STRING_SSH_PASSWORD);
    ssh_password_len = ssh_password.length();

    _elength = 8 + front_password_len + ssh_password_len;

    return 0;
}

int CLocalACAddrConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(ac_addr_len);
    buffer.storerawbytes((uint8_t*)ac_addr.c_str(), ac_addr_len);

    return 0;
}
int CLocalACAddrConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    ac_addr = GetValue(kv, STRING_AC_ADDR);
    ac_addr_len = ac_addr.length();

    _elength = 1 + ac_addr_len;

    return 0;
}

int CRunningModeConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(ap_running_mode);

    return 0;
}
int CRunningModeConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    ap_running_mode = toInt8(GetValue(kv, STRING_RUN_MODE));

    _elength = 1;

    return 0;
}
