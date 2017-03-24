#include <iostream>
using namespace std;

#include <string>
#include <vector>

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "log.h"
#include "capwap_vselement.h"
#include "buffer.h"
#include "dstring.h"


int CElement::skipEH(CBuffer &buffer)
{
    buffer.forward(4);
    return 0;
}
int CElement::ReAssembleEH(CBuffer &buffer)
{
    buffer.revert(_elength+4);
    AssembleEH(buffer);
    buffer.forward(_elength);
    return 0;
}
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

int CMiniElement::skipEH(CBuffer &buffer)
{
    buffer.forward(2);
    return 0;
}
int CMiniElement::ReAssembleEH(CBuffer &buffer)
{
    buffer.revert(_elength+2);
    AssembleEH(buffer);
    buffer.forward(_elength);
    return 0;
}
int CMiniElement::ParseEH(CBuffer &buffer)
{
    uint8_t type;

    buffer.retrive8(type);
    if (_etype == type)
    {
        buffer.retrive8(_elength);
        setValid(true);
        // dlog(LOG_DEBUG, "type %d length %d", _etype, _elength);
        return 0;
    }
    else
    {
        buffer.revert(1);
        return -1;
    }
}
int CMiniElement::AssembleEH(CBuffer &buffer)
{
    buffer.store8(_etype);
    buffer.store8(_elength);

    return 0;
}


int CWTPModelNumber::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
    {
        return 0;
    }

    value.assign((const char *)buffer.GetPtr(), (uint32_t)_elength);
    buffer.retriverawbytes(NULL, _elength);

    // dlog(LOG_DEBUG, "model-number %s", value.c_str());

    return 0;
}
int CWTPModelNumber::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_DEV_MODEL"=" + value + ";");
    return 0;
}

int CWTPSerialNumber::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
    {
        return 0;
    }

    value.assign((const char *)buffer.GetPtr(), (uint32_t)_elength);
    buffer.retriverawbytes(NULL, _elength);

    // dlog(LOG_DEBUG, "serial-number %s", value.c_str());

    return 0;
}
int CWTPSerialNumber::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_SERIAL_NUMBER"=" + value + ";");
    return 0;
}

int CWTPBoardId::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
    {
        return 0;
    }

    value.assign((const char *)buffer.GetPtr(), (uint32_t)_elength);
    buffer.retriverawbytes(NULL, _elength);

    // dlog(LOG_DEBUG, "board-id %s", value.c_str());

    return 0;
}
int CWTPBoardId::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_COMPANY_ID"=" + value + ";");
    return 0;
}

int CWTPBoardRevision::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
    {
        return 0;
    }

    value.assign((const char *)buffer.GetPtr(), (uint32_t)_elength);
    buffer.retriverawbytes(NULL, _elength);

    // dlog(LOG_DEBUG, "board_revision %s", value.c_str());

    return 0;
}

int CWTPBoardRevision::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_HARDWARE_VERSION"=" + value + ";");
    return 0;
}

int CWTPBoardMacAddress::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
    {
        return 0;
    }

    buffer.retriverawbytes(macaddr, _elength);

    // dlog(LOG_DEBUG, "macaddress %02x:%02x:%02x:%02x:%02x:%02x",
    //       macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

    return 0;
}
int CWTPBoardMacAddress::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    char mac[32] = {0};
    snprintf(mac, sizeof(mac), "%02X-%02X-%02X-%02X-%02X-%02X",
             macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

    str.append(STRING_AP_MAC"=" + toString(mac) + ";");

    return 0;
}

int CAPLanIPTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
    {
        return 0;
    }

    buffer.retrive8(ip_type);
    buffer.retrive32(ap_lanip);
    buffer.retrive8(ap_mac_len);
    buffer.retriverawbytes(ap_mac, ap_mac_len);

    //dlog(LOG_DEBUG, "%s.%d ip_type %d ap_lanip %#x ap_mac_len %d ap_mac %02x:%02x:%02x:%02x:%02x:%02x",
    //       __func__, __LINE__, ip_type, ap_lanip, ap_mac_len,
    //       ap_mac[0], ap_mac[1], ap_mac[2], ap_mac[3], ap_mac[4], ap_mac[5]);

    return 0;
}

int CAPLanIPTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    char mac[32] = {0};
    char ip[32] = {0};
    uint32_t netip = htonl(ap_lanip);

    // TODO IPV6 support
    inet_ntop(AF_INET, &netip, ip, sizeof(ip));
    snprintf(mac, sizeof(mac), "%02X-%02X-%02X-%02X-%02X-%02X",
             ap_mac[0], ap_mac[1], ap_mac[2], ap_mac[3], ap_mac[4], ap_mac[5]);

    str.append(STRING_LAN_IPADDR_TYPE"=" + toString((uint32_t)ip_type) + ";");
    str.append(STRING_LAN_IPADDR"=" + toString(ip) + ";");
    str.append(STRING_AP_MAC_LEN"=" + toString((uint32_t)ap_mac_len) + ";");
    str.append(STRING_AP_MAC"=" + toString(mac) + ";");

    return 0;
}

int CAPTransConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(trans_type);
    return 0;
}
int CAPTransConfTlv::LoadFrom(kvlist &kv,string ex)
{
    if (!isValid())
        return 0;

    string stype = GetValue(kv, STRING_AP_TRANS_TYPE);
    trans_type = (uint8_t)toInt(stype);
    _elength = 1;
    return 0;
}

int CWTPDescriptorHardwareVersionTlv::Parse(CBuffer &buffer)
{
    buffer.retrive32(wtp_descriptor_vendor);

    if (0 != ParseEH(buffer))
    {
        // 减去vendor所占的4个字节
        buffer.forward(4);
        return 0;
    }

    value.assign((const char *)buffer.GetPtr(), (uint32_t)_elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CWTPDescriptorHardwareVersionTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_HARDWARE_VERSION"=" + value + ";");
    return 0;
}

int CWTPDescriptorSoftwareVersionTlv::Parse(CBuffer &buffer)
{
    buffer.retrive32(wtp_descriptor_vendor);

    if (0 != ParseEH(buffer))
    {
        // 减去vendor所占的4个字节
        buffer.forward(4);
        return 0;
    }

    value.assign((const char *)buffer.GetPtr(), (uint32_t)_elength);
    buffer.retriverawbytes(NULL, _elength);
    return 0;
}
int CWTPDescriptorSoftwareVersionTlv::SaveTo(string &str)
{
    if (!isValid())
        return 0;

    str.append(STRING_SOFTWARE_VERSION"=" + value + ";");
    return 0;
}

int CVSRadioConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(amsdu);
    buffer.store8(ampdu);
    buffer.store8(radio_type_11nonly);
    buffer.store8(short_gi);
    buffer.store8(band_width);
    buffer.store8(max_support_mcs);
    buffer.store8(max_mandantory_mcs);
    buffer.store8(tx_antenna);
    buffer.store8(rx_antenna);
    buffer.store8(auto_channel_switch);
    buffer.store16(auto_channel_period);
    buffer.store8(auto_power_switch);
    buffer.storerawbytes(reserved, sizeof(reserved));

    return 0;
}
int CVSRadioConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    amsdu = (uint8_t)toInt(GetValue(kv, STRING_A_MSDU + ex));
    ampdu = (uint8_t)toInt(GetValue(kv, STRING_A_MPDU + ex));
    radio_type_11nonly = (uint8_t)toInt(GetValue(kv, STRING_11N_ONLY + ex));
    short_gi = (uint8_t)toInt(GetValue(kv, STRING_SHORT_GI + ex));
    band_width = (uint8_t)toInt(GetValue(kv, STRING_BAND_WIDTH + ex));
    max_support_mcs = (uint8_t)toInt(GetValue(kv, STRING_MAX_SUPPORT_MCS + ex));
    max_mandantory_mcs = (uint8_t)toInt(GetValue(kv, STRING_MAX_MANDANTORY_MCS + ex));
    tx_antenna = (uint8_t)toInt(GetValue(kv, STRING_TX_ANTENNA + ex));
    rx_antenna = (uint8_t)toInt(GetValue(kv, STRING_RX_ANTENNA + ex));
    auto_channel_switch = (uint8_t)toInt(GetValue(kv, STRING_AUTO_CHANNEL_SWITCH + ex));
    auto_channel_period = (uint16_t)toInt(GetValue(kv, STRING_AUTO_CHANNEL_PERIOD + ex));
    auto_power_switch = (uint8_t)toInt(GetValue(kv, STRING_AUTO_POWER_SWITCH + ex));
    // reserved 2 bytes
    _elength = 16;

    return 0;
}

int CVSAPSpaceInfoTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(space_len);
    buffer.storerawbytes((uint8_t*)space.c_str(), space_len);
    return 0;
}
int CVSAPSpaceInfoTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    space = GetValue(kv, STRING_AP_SPACE + ex);
    _elength += space.length();

    space_len = space.length();
    _elength += 4;

    return 0;

}

int CVSABPRadioConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store32(ack_timeout);
    buffer.store32(beacon_rate);
    buffer.store8(protect_mode);
    return 0;
}
int CVSABPRadioConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    ack_timeout = toInt(GetValue(kv, STRING_ACK_TIMEOUT + ex));
    beacon_rate = toInt(GetValue(kv, STRING_BEACON_RATE + ex));
    protect_mode = (uint8_t)toInt(GetValue(kv, STRING_PROTECT_MODE + ex));

    _elength = 10;
    return 0;
}

int CVSRadioProbeConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store32(probe_rssi);
    buffer.store32(min_contract_rate);
    buffer.storerawbytes((uint8_t*)reserved, sizeof(reserved));

    return 0;
}
int CVSRadioProbeConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    probe_rssi = toInt(GetValue(kv, STRING_PROBE_RSSI + ex));
    min_contract_rate = toInt(GetValue(kv, STRING_MIN_CONTRACT_RATE + ex));

    _elength = 21;

    return 0;
}

int CVSEchoConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(echo_interval);
    buffer.store32(echo_timeout_cnt);

    return 0;
}
int CVSEchoConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    echo_interval = toInt(GetValue(kv, STRING_ECHO_INTERVAL));
    echo_timeout_cnt = toInt(GetValue(kv, STRING_ECHO_TIMEOUT_COUNT));

    _elength = 8;
    return 0;
}

int CVSTrafficStaticsTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(ts_interval);
    buffer.store32(ts_switch);

    return 0;
}
int CVSTrafficStaticsTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    ts_interval = toInt(GetValue(kv, STRING_TRAFFIC_STATICS_INTERVAL + ex));
    ts_switch = toInt(GetValue(kv, STRING_TRAFFIC_SWITCH + ex));

    _elength = 8;

    return 0;
}

int CVSNtpServerConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store16(interval);
    buffer.store16(ntp_server_len);
    buffer.storerawbytes((uint8_t*)server.c_str(), ntp_server_len);

    return 0;
}
int CVSNtpServerConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    interval = (uint16_t)toInt(GetValue(kv, STRING_NTP_SERVER_INTERVAL + ex));
    _elength += 2;

    server = GetValue(kv, STRING_NTP_SERVER);
    _elength += server.length();

    ntp_server_len = server.length();
    _elength += 2;

    return 0;
}

int CVSPacketPowerConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(min_power);
    buffer.store32(min_throughput_threshold);
    buffer.store32(min_rate_threshold);
    buffer.store8(ppc_enable);

    return 0;
}
int CVSPacketPowerConfTlv::LoadFrom(kvlist &kv, string ex)
{
    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    min_power = (uint8_t)toInt(GetValue(kv, STRING_MIN_POWER + ex));
    min_throughput_threshold = toInt(GetValue(kv, STRING_MIN_THROUGHPUT_THRESHOLD + ex));
    min_rate_threshold = toInt(GetValue(kv, STRING_MIN_RATE_THRESHOLD + ex));
    ppc_enable = (uint8_t)toInt(GetValue(kv, STRING_PPC_ENABLE + ex));

    _elength = 11;
    return 0;
}

int CVSChannelReuseConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(cca_adj);
    buffer.store8(cca_thr);

    return 0;
}
int CVSChannelReuseConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = (uint8_t)toInt(GetValue(kv, STRING_RADIO_ID + ex));
    cca_adj = (uint8_t)toInt(GetValue(kv, STRING_CCA_ADJ + ex));
    cca_thr = (uint8_t)toInt(GetValue(kv, STRING_CCA_THR + ex));

    _elength = 3;

    return 0;
}
