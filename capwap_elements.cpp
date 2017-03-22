#include <iostream>
using namespace std;

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "capwap_elements.h"
#include "buffer.h"
#include "dstring.h"


int CElement::ParseEH(CBuffer &buffer)
{
    uint16_t type;

    buffer.retrive16(type);
    if (_etype == type)
    {
        buffer.retrive16(_elength);
        setValid(true);
        // dlog(LOG_DEBUG, "type %d length %d", _etype, _elength);
        return 0;
    }
    else
    {
        buffer.revert(2);
        return -1;
    }
}
int CElement::AssembleEH(CBuffer &buffer)
{
    buffer.store16(_etype);
    buffer.store16(_elength);

    return 0;
}

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

int CDSCtrolTlv::Parse(CBuffer &buffer)
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
int CDSCtrolTlv::SaveTo(string &str)
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
int CDSCtrolTlv::Assemble(CBuffer &buffer)
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
int CDSCtrolTlv::LoadFrom(kvlist &kv, string ex)
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

int COFDMCtrolTlv::Assemble(CBuffer &buffer)
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
int COFDMCtrolTlv::LoadFrom(kvlist &kv, string ex)
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
