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

int CWTPBoardDataTlv::SaveTo(string &str)
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

    ap_lanip.Parse(buffer);

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

    return 0;
}
int CVendorSpecPayLoadTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    ap_lanip.SaveTo(str);

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

int CWTPRadioInfoTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + toString((uint32_t)radio_id) + "=" + toString((uint32_t)radio_id) + ";");
    str.append(STRING_RADIO_TYPE_11A + toString((uint32_t)radio_id) + "=" +
               toString((uint32_t)u_radio_type.s.radio_type_11a) + ";");
    str.append(STRING_RADIO_TYPE_11B + toString((uint32_t)radio_id) + "=" +
               toString((uint32_t)u_radio_type.s.radio_type_11b) + ";");
    str.append(STRING_RADIO_TYPE_11G + toString((uint32_t)radio_id) + "=" +
               toString((uint32_t)u_radio_type.s.radio_type_11g) + ";");
    str.append(STRING_RADIO_TYPE_11N + toString((uint32_t)radio_id) + "=" +
               toString((uint32_t)u_radio_type.s.radio_type_11n) + ";");

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
    u_radio_type.s.radio_type_11b = (uint8_t)toInt(GetValue(kv, STRING_RADIO_TYPE_11B + ex));
    u_radio_type.s.radio_type_11a = (uint8_t)toInt(GetValue(kv, STRING_RADIO_TYPE_11A + ex));
    u_radio_type.s.radio_type_11g = (uint8_t)toInt(GetValue(kv, STRING_RADIO_TYPE_11G + ex));
    u_radio_type.s.radio_type_11n = (uint8_t)toInt(GetValue(kv, STRING_RADIO_TYPE_11N + ex));

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

int CWTPDescriptorTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_MAX_RADIOS"=" + toString((uint32_t)max_radios) + ";");
    str.append(STRING_RADIO_INUSE"=" + toString((uint32_t)radio_inuse) + ";");
    str.append(STRING_ENCRYPT_NUMBER"=" + toString((uint32_t)encrypt_number) + ";");
    str.append(STRING_ENCRYPT_WBID"=" + toString((uint32_t)u_encrypt_wbid.s.wbid) + ";");
    str.append(STRING_ENCRYPT_CAPABILITIES"=" + toString((uint32_t)encrypt_capabilities) + ";");

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
int CResultTlv::SaveTo(string &str)
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
int CWTPRadioConfTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    char buf[32] = {0};
    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(bssid));

    str.append(STRING_RADIO_ID + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)radio_id) + ";");
    str.append(STRING_SHORT_PREAMBLE + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)short_preamble) + ";");
    str.append(STRING_BSSIDS_NUMBER + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)bssids_number) + ";");
    str.append(STRING_DTIM_PERIOD + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)dtim_period) + ";");
    str.append(STRING_BSSID + toString((uint32_t)radio_id) + "="
               + toString(buf) + ";");
    str.append(STRING_BEACON_PERIOD + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)beacon_period) + ";");
//    str.append(STRING_COUNTRY_STRING + toString((uint32_t)radio_id) + "="
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

    short_preamble = (uint8_t)toInt(GetValue(kv, STRING_SHORT_PREAMBLE + ex));
    _elength += 1;

    bssids_number = (uint8_t)toInt(GetValue(kv, STRING_BSSIDS_NUMBER + ex));
    _elength += 1;

    dtim_period = (uint8_t)toInt(GetValue(kv, STRING_DTIM_PERIOD + ex));
    _elength += 1;

    // bssid 下发为空
    _elength += 6;

    beacon_period = (uint16_t)toInt(GetValue(kv, STRING_BEACON_PERIOD + ex));
    _elength += 2;

    string scountry = GetValue(kv, STRING_COUNTRY_STRING + ex);
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
int CTxPowerTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)radio_id) + ";");
    str.append(STRING_CURRENT_TXPOWER + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)cur_tx_power) + ";");

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
    cur_tx_power = (uint8_t)toInt(GetValue(kv, STRING_CURRENT_TXPOWER + ex));

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
int CDSCtrlTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)radio_id) + ";");
    str.append(STRING_CURRENT_CHANNEL + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)cur_channel) + ";");
    str.append(STRING_CURRENT_CCA + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)cur_cca) + ";");
    str.append(STRING_ENERGY_DETECT_THRESHOLD + toString((uint32_t)radio_id) + "="
               + toString((uint32_t)energy_detect_threshold) + ";");
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
    cur_channel = (uint8_t)toInt(GetValue(kv, STRING_CURRENT_CHANNEL + ex));
    cur_cca = (uint8_t)toInt(GetValue(kv, STRING_CURRENT_CCA + ex));
    energy_detect_threshold = (uint8_t)toInt(GetValue(kv, STRING_ENERGY_DETECT_THRESHOLD + ex));

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
    radio_state = (uint8_t)toInt(GetValue(kv, STRING_RADIO_STATE + ex));

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
    rts_thr = (uint16_t)toInt(GetValue(kv, STRING_RTS_THRESHOLD + ex));
    short_retry = (uint8_t)toInt(GetValue(kv, STRING_SHORT_RETRY + ex));
    long_retry = (uint8_t)toInt(GetValue(kv, STRING_SHORT_RETRY + ex));
    frag_thr = (uint16_t)toInt(GetValue(kv, STRING_FRAG_THR + ex));
    tx_msdu_lifetime = toInt(GetValue(kv, STRING_TX_MSDU_LIFETIME + ex));
    rx_msdu_lifetime = toInt(GetValue(kv, STRING_RX_MSDU_LIFETIME + ex));

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
    current_channel = (uint8_t)toInt(GetValue(kv, STRING_CURRENT_CHANNEL + ex));
    band_width = (uint8_t)toInt(GetValue(kv, STRING_BAND_WIDTH + ex));
    ti_threshold = toInt(GetValue(kv, STRING_TI_THRESHOLD + ex));

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

int CRadioOperationalStateTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_RADIO_ID + toString(radio_id) + "=" + toString(radio_id) + ";");
    str.append(STRING_RADIO_STATE + toString(radio_id) + "=" + toString(radio_op_state) + ";");
    str.append(STRING_RADIO_CAUSE + toString(radio_id) + "=" + toString(radio_op_cause) + ";");
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
int CDataTransferTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_DATA_TYPE"=" + toString(data_type) + ";");
    str.append(STRING_DATA_MODE"=" + toString(data_mode) + ";");
    str.append(STRING_DATA_DATA"=" + data + ";");

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

    enable = toInt8(GetValue(kv, STRING_REPORT_STATION_INFO_ENABLE));

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

    enable = toInt8(GetValue(kv, STRING_ROMING_CONFIG_ENABLE));
    signal = toInt(GetValue(kv, STRING_ROMING_CONFIG_SIGNAL));

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

    wp_enable = toInt8(GetValue(kv, STRING_WP_ENABLE));
    wp_intv   = toInt8(GetValue(kv, STRING_WP_INTERVAL));
    wp_server_ip_type = toInt8(GetValue(kv, STRING_WP_SERVER_IP_TYPE));

    _elength += 3;

    if (IP_TYPE_IPV4 == wp_server_ip_type)
    {
        int ipaddr[4] = {0};
        string addr = GetValue(kv, STRING_WP_SERVER_IP_ADDR);
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
    wp_server_port = toInt16(GetValue(kv, STRING_WP_SERVER_PORT));
    wp_scan_type = toInt8(GetValue(kv, STRING_WP_SCAN_TYPE));
    wp_code = toInt8(GetValue(kv, STRING_WP_CODE));
    wp_proto = toInt8(GetValue(kv, STRING_WP_PROTO));

    ef_enable = toInt8(GetValue(kv, STRING_EF_ENABLE));
    ef_code = toInt8(GetValue(kv, STIRNG_EF_CODE));
    ef_proto = toInt8(GetValue(kv, STIRNG_EF_PROTO));
    ef_intv = toInt8(GetValue(kv, STIRNG_EF_INTERVAL));
    ef_scan_type = toInt8(GetValue(kv, STIRNG_EF_SCAN_TYPE));
    ef_server_ip_type = toInt8(GetValue(kv, STIRNG_EF_SERVER_IP_TYPE));

    _elength += 11;

    if (IP_TYPE_IPV4 == ef_server_ip_type)
    {
        int ipaddr[4] = {0};
        string addr = GetValue(kv, STRING_EF_SERVER_IP_ADDR);
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
    ef_server_port = toInt16(GetValue(kv, STRING_EF_SERVER_PORT));

    we_ad_intv = toInt16(GetValue(kv, STRING_WE_AD_INTERVAL));
    we_channel_2g = toInt(GetValue(kv, STRING_WE_CHANNEL_2G));
    we_channel_5g = toInt(GetValue(kv, STRING_WE_CHANNEL_5G));
    we_ad_rssi = toInt16(GetValue(kv, STRING_WE_AD_RSSI));

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

    rfg_enable = toInt8(GetValue(kv, STRING_RFG_ENABLE));
    rfg_assocmax = toInt8(GetValue(kv, STRING_RFG_ASSOCMAX));
    rfg_timeout = toInt8(GetValue(kv, STRING_RFG_TIMEOUT));
    rfg_maxsta = toInt8(GetValue(kv, STRING_RFG_MAXSTA));
    rfg_method = toInt8(GetValue(kv, STRING_RFG_METHOD));

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

    load_balance_enable = toInt8(GetValue(kv, STRING_AP_LOADBALANCE_ENABLE));
    load_balance_threshold = toInt16(GetValue(kv, STRING_AP_LOADBALANCE_THRESHOLD));
    load_balance_interval = toInt16(GetValue(kv, STRING_AP_LOADBALANCE_INTERVAL));

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

    radio_type_11a_len = toInt8(GetValue(kv, STRING_RATE_SET_11A_LEN));
    rate = toInt(GetValue(kv, STRING_RATE_SET_11A_RATE));
    radio_type_11a_rate[0] = rate & 0xff;
    radio_type_11a_rate[1] = rate & 0xff00;
    radio_type_11a_rate[2] = rate & 0xff0000;
    radio_type_11a_rate[3] = rate & 0xff000000;

    radio_type_11bg_len = toInt8(GetValue(kv, STRING_RATE_SET_11BG_LEN));
    rate = toInt(GetValue(kv, STRING_RATE_SET_11BG_RATE));
    radio_type_11bg_rate[0] = rate & 0xff;
    radio_type_11bg_rate[1] = rate & 0xff00;
    radio_type_11bg_rate[2] = rate & 0xff0000;
    radio_type_11bg_rate[3] = rate & 0xff000000;

    radio_type_11n_len = toInt8(GetValue(kv, STRING_RATE_SET_11N_LEN));
    rate = toInt(GetValue(kv, STRING_RATE_SET_11N_RATE));
    radio_type_11n_rate[0] = rate & 0xff;
    radio_type_11n_rate[1] = rate & 0xff00;
    radio_type_11n_rate[2] = rate & 0xff0000;
    radio_type_11n_rate[3] = rate & 0xff000000;

    radio_type_11ac_len = toInt8(GetValue(kv, STRING_RATE_SET_11AC_LEN));
    rate = toInt(GetValue(kv, STRING_RATE_SET_11AC_RATE));
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

    low_rssi_refuse_enable = toInt8(GetValue(kv, STRING_LOW_RSSI_REFUSE_ENABLE));
    low_rssi_threshold = toInt(GetValue(kv, STRING_LOW_RSSI_THRESHOLD));

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

    lan_vlan_id = toInt(GetValue(kv, STRING_LAN_VLAN_ID));
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

    interval = toInt(GetValue(kv, STRING_REPORT_STATION_INFO_INTERVAL));
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

    audit_enable = toInt(GetValue(kv, STRING_AUDIT_ENABLE));
    appri_enable = toInt(GetValue(kv, STRING_APPRI_ENABLE));
    remote_sync_enable = toInt(GetValue(kv, STRING_REMOTE_SYNC_ENABLE));
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
    buffer.storerawbytes(reserved, sizeof(reserved));

    return 0;
}
int CLanPortalConfTlv::LoadFrom(kvlist &kv, string ex)
{
    // unitid  //  not use
    portal_enable = toInt(GetValue(kv, STRING_LAN_PORTAL_ENABLE));
    portal_url = GetValue(kv, STRING_LAN_PORTAL_URL);
    portal_url_len = portal_url.length();
    // reserved

    _elength = 12 + portal_url_len + 16;

    return 0;
}

