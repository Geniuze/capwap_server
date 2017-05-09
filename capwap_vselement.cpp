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
#include "jsoncpp/json.h"


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
int CElement::SkipTlv(CBuffer &buffer)
{
    uint16_t type;
    uint16_t length;

    buffer.retrive16(type);
    buffer.retrive16(length);
    buffer.retriverawbytes(NULL, length);

    dlog(LOG_ERR, "%s.%d type %d length %d", __func__, __LINE__, type, length);

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
int CWTPModelNumber::SaveTo(string &str, string ex)
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
int CWTPSerialNumber::SaveTo(string &str, string ex)
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
int CWTPBoardId::SaveTo(string &str, string ex)
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

int CWTPBoardRevision::SaveTo(string &str, string ex)
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
int CWTPBoardMacAddress::SaveTo(string &str, string ex)
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

int CAPLanIPTlv::SaveTo(string &str, string ex)
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

    // str.append(STRING_LAN_IPADDR_TYPE"=" + toString(ip_type) + ";");
    str.append(STRING_LAN_IPADDR"=" + toString(ip) + ";");
    // str.append(STRING_AP_MAC_LEN"=" + toString(ap_mac_len) + ";");
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

    // 本地转发
    trans_type = 0;

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
int CWTPDescriptorHardwareVersionTlv::SaveTo(string &str, string ex)
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
int CWTPDescriptorSoftwareVersionTlv::SaveTo(string &str, string ex)
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

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    amsdu = toInt8(GetValue(kv, TO_STR(STRING_RADIO_A_MSDU) + ex));
    ampdu = toInt8(GetValue(kv, TO_STR(STRING_RADIO_A_MPDU) + ex));
    radio_type_11nonly = toInt8(GetValue(kv, TO_STR(STRING_RADIO_11NONLY) + ex));
    short_gi = toInt8(GetValue(kv, TO_STR(STRING_RADIO_SHORT_GI) + ex));
    band_width = toInt8(GetValue(kv, TO_STR(STRING_RADIO_CHWIDTH) + ex));
    // max_support_mcs = toInt8(GetValue(kv, STRING_MAX_SUPPORT_MCS + ex));
    // max_mandantory_mcs = toInt8(GetValue(kv, STRING_MAX_MANDANTORY_MCS + ex));
    tx_antenna = toInt8(GetValue(kv, TO_STR(STRING_RADIO_SPACE) + ex));
    rx_antenna = toInt8(GetValue(kv, TO_STR(STRING_RADIO_SPACE) + ex));
    auto_channel_switch = toInt8(GetValue(kv, TO_STR(STRING_RADIO_AUTO_CHANNEL) + ex));
    auto_channel_period = toInt16(GetValue(kv, TO_STR(STRING_RADIO_AUTO_CHANNEL_PERIOD) + ex));
    auto_power_switch = toInt8(GetValue(kv, TO_STR(STRING_RADIO_AUTO_POWER) + ex));
    // reserved 2 bytes
    _elength = 16;

    return 0;
}

int CVSVlanConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(wlan_id);
    buffer.store32(vlan_id);
    buffer.store16(sta_isolate);
    buffer.store8(mac_filter_rules);
    buffer.store16(max_stas);
    buffer.store8(portal_enable);
    buffer.store32(url_size);
    buffer.storerawbytes((uint8_t*)url.c_str(), url_size);
    buffer.store8(tunnel_enable);
    buffer.store32(ssid_up_traffic);
    buffer.store32(ssid_down_traffic);
    buffer.store32(user_up_traffic);
    buffer.store32(user_down_traffic);
    buffer.store8(essid_encode);

    return 0;
}

int CVSVlanConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    int auth_type = toInt8(GetValue(kv, TO_STR(STRING_WLAN_AUTH_TYPE) + ex));

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    wlan_id = toInt8(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));
    vlan_id = toInt32(GetValue(kv, TO_STR(STRING_WLAN_VLAN_ID) + ex));
    sta_isolate = toInt16(GetValue(kv, TO_STR(STRING_WLAN_USER_ISOLATE) + ex));
    mac_filter_rules = toInt8(GetValue(kv, TO_STR(STRING_WLAN_MAC_FILTER_RULES) + ex)); //
    max_stas = toInt16(GetValue(kv, TO_STR(STRING_WLAN_MAX_USER) + ex));
    portal_enable = auth_type & AUTH_TYPE_PORTALAUTH;
    url = GetValue(kv, TO_STR(STRING_WLAN_PORTAL_URL) + ex);
    url_size = url.length();
    tunnel_enable = toInt8(GetValue(kv, TO_STR(STRING_WLAN_TUNNEL_ENABLE) + ex));
    ssid_up_traffic = toInt32(GetValue(kv, TO_STR(STRING_WLAN_SSID_UP) + ex));
    ssid_down_traffic = toInt32(GetValue(kv, TO_STR(STRING_WLAN_SSID_DOWN) + ex));
    user_up_traffic = toInt32(GetValue(kv, TO_STR(STRING_WLAN_USER_UP) + ex));
    user_down_traffic = toInt32(GetValue(kv, TO_STR(STRING_WLAN_USER_DOWN) + ex));
    essid_encode = toInt8(GetValue(kv, TO_STR(STRING_WLAN_ESSID_ENCODE) + ex));

    _elength = 34 + url_size;

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

    space = GetValue(kv, TO_STR(STRING_POSITION) + ex);
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

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    ack_timeout = toInt(GetValue(kv, TO_STR(STRING_RADIO_RESPONSE_TIMEOUT) + ex));
    beacon_rate = toInt(GetValue(kv, TO_STR(STRING_RADIO_BEACON_SEND_SPEED) + ex));
    protect_mode = toInt8(GetValue(kv, TO_STR(STRING_RADIO_PROTECT_MODE) + ex));

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

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    probe_rssi = toInt(GetValue(kv, TO_STR(STRING_RADIO_PROBE_SUPPRESSION) + ex));
    min_contract_rate = toInt(GetValue(kv, TO_STR(STRING_RADIO_MIN_SEND_SPEED) + ex));

    _elength = 21;

    return 0;
}

int CVSEchoConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(echo_timeout_interval);
    buffer.store32(echo_timeout_cnt);

    return 0;
}
int CVSEchoConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    echo_timeout_interval = toInt(GetValue(kv, STRING_ECHO_INTERVAL));
    echo_timeout_cnt = toInt(GetValue(kv, STRING_ECHO_COUNT));

    _elength = 8;
    return 0;
}

int CVSTrafficStaticsTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive64(tx_speed);
    buffer.retrive64(rx_speed);

    return 0;
}
int CVSTrafficStaticsTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_TX_SPEED"=" + toString(tx_speed) + ";");
    str.append(STRING_RX_SPEED"=" + toString(rx_speed) + ";");

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
    ts_switch = toInt(GetValue(kv, STRING_TRAFFIC_STATICS_ENABLE + ex));

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

    interval = toInt16(GetValue(kv, TO_STR(STRING_NTP_SYNC_INTERVAL) + ex));
    _elength += 2;

    server = GetValue(kv, TO_STR(STRING_NTP_SERVER_ADDR) + ex);
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
    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    min_power = toInt8(GetValue(kv, TO_STR(STRING_RADIO_PKT_POWER_MIN_POWER) + ex));
    min_throughput_threshold = toInt(GetValue(kv, TO_STR(STRING_RADIO_PKT_POWER_MIN_THROUGHPUT_THRESHOLD) + ex));
    min_rate_threshold = toInt(GetValue(kv, TO_STR(STRING_RADIO_PKT_POWER_MIN_RATE_THRESHOLD) + ex));
    ppc_enable = toInt8(GetValue(kv, TO_STR(STRING_RADIO_PKT_POWER_ENABLE) + ex));

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

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    cca_adj = toInt8(GetValue(kv, TO_STR(STRING_RADIO_CCA_ADJ) + ex));
    cca_thr = toInt8(GetValue(kv, TO_STR(STRING_RADIO_CCA_THR) + ex));

    _elength = 3;

    return 0;
}

int CPortalCustom::Assemble(CBuffer &buffer)
{
    // Assemble(buffer); //此消息不是消息元，不可组装头部信息
    buffer.store16(key_len);
    buffer.storerawbytes((uint8_t*)key.c_str(), key_len);
    buffer.store16(alias_len);
    buffer.storerawbytes((uint8_t*)alias.c_str(), alias_len);
    buffer.store16(value_len);
    buffer.storerawbytes((uint8_t*)value.c_str(), value_len);

    return 0;
}
int CPortalCustom::LoadFrom(kvlist &kv, string ex)
{
    key = GetValue(kv, TO_STR(STRING_PORTAL_CUSTOM_KEY) + ex);
    key_len = key.length();
    alias = GetValue(kv, TO_STR(STRING_PORTAL_CUSTOM_ALIAS) + ex);
    alias_len = alias.length();
    value = GetValue(kv, TO_STR(STRING_PORTAL_CUSTOM_VALUE) + ex);
    value_len = value.length();

    _elength += 6 + key_len + alias_len + value_len;

    return 0;
}

int CVSPortalCustomConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(count);

    for (size_t i=0; i<portal_customs.size(); i++)
    {
        portal_customs[i].Assemble(buffer);
    }

    return 0;
}
int CVSPortalCustomConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    _elength += 4;

    Json::Reader reader;
    Json::Value value;
    Json::Value tmp;

    string data = GetValue(kv, TO_STR(STRING_PORTAL_CUSTOM_DATA));
    if ("" == data)
    {
        dlog(LOG_ERR, "%s.%d {portal_custom_data is null}", __FUNC__, __LINE__);
        return 0;
    }

    if (!reader.parse(data, tmp))
    {
        dlog(LOG_ERR, "%s.%d {JSON PARSE ERROR %s}", __FUNC__, __LINE__, data.c_str());
        return 0;
    }

    count = toInt(tmp[TO_STR(STRING_PORTAL_CUSTOM_COUNT)].asString());
    value = tmp[TO_STR(STRING_PORTAL_CUSTOM_CONTENT)];

    portal_customs.resize(count);
    for (size_t i=0; i<portal_customs.size(); i++)
    {
        kvlist kv_akvs;
        SetValue(kv_akvs, TO_STR(STRING_PORTAL_CUSTOM_KEY) + toString(i),
                 value[i][TO_STR(STRING_PORTAL_CUSTOM_KEY)].asString());
        SetValue(kv_akvs, TO_STR(STRING_PORTAL_CUSTOM_ALIAS) + toString(i),
                 value[i][TO_STR(STRING_PORTAL_CUSTOM_ALIAS)].asString());
        SetValue(kv_akvs, TO_STR(STRING_PORTAL_CUSTOM_VALUE) + toString(i),
                 value[i][TO_STR(STRING_PORTAL_CUSTOM_VALUE)].asString());

        portal_customs[i].LoadFrom(kv_akvs, toString(i));
        _elength += portal_customs[i].length();
    }

    return 0;
}

int CVSTimeStampConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(time_stamp);

    return 0;
}
int CVSTimeStampConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    time_stamp = time(NULL);

    _elength = 4;

    return 0;
}

int CVSByPassConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(by_pass);

    return 0;
}
int CVSByPassConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    by_pass = toInt8(GetValue(kv, TO_STR(STRING_BY_PASS_ENABLE)));
    _elength = 1;

    return 0;
}

int CVSMcastEnhanceTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);
    buffer.store8(radio_id);
    buffer.store8(wlan_id);
    buffer.store16(broadcast_enable);
    buffer.store16(broadcast_max_sta);
    buffer.store16(broadcast_timeout);

    return 0;
}
int CVSMcastEnhanceTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    wlan_id = toInt8(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));
    broadcast_enable = toInt16(GetValue(kv, TO_STR(STRING_WLAN_MULTICAST_TO_UNICAST_ENABLE) + ex));
    broadcast_max_sta = toInt16(GetValue(kv, TO_STR(STRING_WLAN_MULTICAST_TO_UNICAST_MAX_STA) + ex));
    broadcast_timeout = toInt16(GetValue(kv, TO_STR(STRING_WLAN_MULTICAST_TO_UNICAST_TIMEOUT) + ex));

    _elength = 8;
    return 0;
}

int CVSWxAuthInfoTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(wlanid);
    buffer.store32(radioid);
    buffer.store32(wxenable);
    buffer.store32(urllen);
    buffer.storerawbytes((uint8_t*)wxurl.c_str(), urllen);

    return 0;
}
int CVSWxAuthInfoTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    uint8_t auth_type = toInt8(GetValue(kv, TO_STR(STRING_WLAN_AUTH_TYPE) + ex));

    wlanid = toInt32(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));
    radioid = toInt32(GetValue(kv, STRING_RADIO_ID + ex));
    wxenable = auth_type & AUTH_TYPE_WXAUTH;
    // wxurl = GetValue(kv, STRING_WX_URL);
    urllen = wxurl.length();

    _elength = 16 + urllen;
    return 0;
}

int CVSWlanInfoTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(radio_id);
    buffer.store8(wlan_id);
    buffer.store8(wds_enable);

    return 0;
}
int CVSWlanInfoTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = toInt8(GetValue(kv, STRING_RADIO_ID + ex));
    wlan_id = toInt8(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));
    wds_enable = toInt8(GetValue(kv, TO_STR(STRING_WLAN_WDS_ENABLE) + ex));

    _elength = 3;
    return 0;
}

int CActlStaStateTlv::Parse(CBuffer &buffer)
{

    // 没有头部，不组装头部消息

    buffer.retrive32(pkt_id);
    buffer.retrive8(pkt_type);
    buffer.retrive8(msg_type);
    buffer.retrive8(sub_msg_type);
    buffer.retrive8(user_ip_type);
    if (IP_TYPE_IPV4 == user_ip_type)
    {
        buffer.retriverawbytes(user_ip, 4);
    }
    else
    {
        buffer.retriverawbytes(user_ip, 16);
    }
    buffer.retrive8(sta_mac_len);
    buffer.retriverawbytes(sta_mac, sta_mac_len);
    buffer.retrive8(ap_mac_len);
    buffer.retriverawbytes(ap_mac, ap_mac_len);
    buffer.retrive32(user_role_len);
    user_role.assign((const char *)buffer.GetPtr(), user_role_len);
    buffer.retriverawbytes(NULL, user_role_len);
    buffer.retrive32(session_time);
    buffer.retrive8(radio_id);
    buffer.retrive8(wlan_id);
    buffer.retrive8(ssid_len);
    ssid.assign((const char *)buffer.GetPtr(), ssid_len);
    buffer.retriverawbytes(NULL, ssid_len);

    return 0;
}
int CActlStaStateTlv::SaveTo(string &str, string ex)
{
    // 此消息不是消息元，不会解析头部，所以不会设置为valid
    char buf[128] = {0};
    str.append(STRING_PKT_ID   + ex + "=" + toString(pkt_id)       + ";");
    str.append(STRING_PKT_TYPE + ex + "=" + toString(pkt_type)     + ";");
    str.append(STRING_MSG_TYPE + ex + "=" + toString(msg_type)     + ";");
    str.append(STRING_SUB_TYPE + ex + "=" + toString(sub_msg_type) + ";");

    if (IP_TYPE_IPV4 == user_ip_type)
    {
        snprintf(buf, sizeof(buf), IP_ADDR_FMT, IP_ADDR_VAL(user_ip));
    }
    str.append(STRING_USER_IP  + ex + "=" + toString(buf) + ";");

    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(sta_mac));
    str.append(STRING_STA_MAC  + ex + "=" + toString(buf) + ";");

    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(ap_mac));
    str.append(STRING_AP_MAC   + ex + "=" + toString(buf) + ";");

    str.append(STRING_USER_ROLE+ ex + "=" + user_role     + ";");
    str.append(STRING_USER_SESSION_TIME + ex + "=" + toString(session_time) + ";");
    str.append(STRING_RADIO_ID + ex + "=" + toString(radio_id) + ";");
    str.append(TO_STR(STRING_WLAN_WLAN_ID) + ex + "=" + toString(wlan_id) + ";");
    str.append(TO_STR(STRING_WLAN_ESSID) + ex + "=" + ssid + ";");

    return 0;
}

int CVSActlReportStaStateTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(count);

    sta_states.resize(count);
    for (size_t i = 0; i<count; i++)
    {
        sta_states[i].Parse(buffer);
    }

    return 0;
}
int CVSActlReportStaStateTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_USER_COUNT"=" + toString(count) + ";");

    for (size_t i = 0; i<sta_states.size(); i++)
    {
        sta_states[i].SaveTo(str, toString(i));
    }

    return 0;
}

int CVSStationIPAddrTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(ip_type);
    if (IP_TYPE_IPV4 == ip_type)
    {
        buffer.retriverawbytes(ip_addr, 4);
    }
    else
    {
        buffer.retriverawbytes(ip_addr, 16);
    }

    return 0;
}
int CVSStationIPAddrTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    char buf[128] = {0};
    snprintf(buf, sizeof(buf), IP_ADDR_FMT, IP_ADDR_VAL(ip_addr));

    str.append(STRING_USER_IP"=" + toString(buf) + ";");

    return 0;
}

int CVSActlUserInfoPktTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive32(pkt_id);
    buffer.retrive8(pkt_type);
    buffer.retrive8(msg_type);
    buffer.retrive8(sub_msg_type);
    buffer.retrive8(sta_ip_type);
    if (IP_TYPE_IPV4 == sta_ip_type)
        buffer.retriverawbytes(sta_ip, 4);
    else
        buffer.retriverawbytes(sta_ip, 16);
    buffer.retrive8(sta_mac_len);
    buffer.retriverawbytes(sta_mac, sta_mac_len);
    buffer.retrive8(ap_mac_len);
    buffer.retriverawbytes(ap_mac, ap_mac_len);
    buffer.retrive32(user_role_len);
    user_role.assign((const char *)buffer.GetPtr(), user_role_len);
    buffer.retriverawbytes(NULL, user_role_len);
    buffer.retrive32(session_time);
    buffer.retrive8(radio_id);
    buffer.retrive8(wlan_id);

    return 0;
}

int CVSActlUserInfoPktTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    char buf[128] = {0};

    str.append(STRING_PKT_ID + ex + "=" + toString(pkt_id) + ";");
    str.append(STRING_PKT_TYPE + ex + "=" + toString(pkt_type) + ";");
    str.append(STRING_MSG_TYPE + ex + "=" + toString(msg_type) + ";");
    str.append(STRING_SUB_TYPE + ex + "=" + toString(sub_msg_type) + ";");

    snprintf(buf, sizeof(buf), IP_ADDR_FMT, IP_ADDR_VAL(sta_ip));
    str.append(STRING_USER_IP + ex + "=" + buf + ";");

    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(sta_mac));
    str.append(STRING_STA_MAC + ex + "=" + buf + ";");

    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(ap_mac));
    str.append(STRING_AP_MAC + ex + "=" + buf + ";");

    str.append(STRING_USER_ROLE+ ex + "=" + user_role + ";");
    str.append(STRING_USER_SESSION_TIME + ex + "=" + toString(session_time) + ";");
    str.append(STRING_RADIO_ID + ex + "=" + toString(radio_id) + ";");
    str.append(TO_STR(STRING_WLAN_WLAN_ID) + ex + "=" + toString(wlan_id) + ";");

    return 0;
}

int CVSActlUserInfoPktTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store32(pkt_id);
    buffer.store8(pkt_type);
    buffer.store8(msg_type);
    buffer.store8(sub_msg_type);
    buffer.store8(sta_ip_type);
    if (IP_TYPE_IPV4 == sta_ip_type)
        buffer.storerawbytes(sta_ip, 4);
    else
        buffer.storerawbytes(sta_ip, 16);
    buffer.store8(sta_mac_len);
    buffer.storerawbytes(sta_mac, sta_mac_len);
    buffer.store8(ap_mac_len);
    buffer.storerawbytes(ap_mac, ap_mac_len);
    buffer.store32(user_role_len);
    buffer.storerawbytes((uint8_t *)user_role.c_str(), user_role_len);
    buffer.store32(session_time);
    buffer.store8(radio_id);
    buffer.store8(wlan_id);

    return 0;
}
int CVSActlUserInfoPktTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    int buf[16] = {0};
    pkt_id = toInt(GetValue(kv, STRING_PKT_ID + ex));
    pkt_type = toInt(GetValue(kv, STRING_PKT_TYPE + ex));
    msg_type = toInt(GetValue(kv, STRING_MSG_TYPE + ex));
    sub_msg_type = toInt(GetValue(kv, STRING_SUB_TYPE + ex));
    sta_ip_type = 4;
    sscanf(GetValue(kv, STRING_USER_IP + ex).c_str(), IP_ADDR_FMT, IP_ADDR_RVAL(buf));
    sta_ip[0] = buf[0];
    sta_ip[1] = buf[1];
    sta_ip[2] = buf[2];
    sta_ip[3] = buf[3];

    sta_mac_len = 6;
    sscanf(GetValue(kv, STRING_STA_MAC + ex).c_str(), ETHER_ADDR_FMT, ETHER_ADDR_RVAL(buf));
    sta_mac[0] = buf[0];
    sta_mac[1] = buf[1];
    sta_mac[2] = buf[2];
    sta_mac[3] = buf[3];
    sta_mac[4] = buf[4];
    sta_mac[5] = buf[5];

    ap_mac_len = 6;
    sscanf(GetValue(kv, STRING_AP_MAC + ex).c_str(), ETHER_ADDR_FMT, ETHER_ADDR_RVAL(buf));
    ap_mac[0] = buf[0];
    ap_mac[1] = buf[1];
    ap_mac[2] = buf[2];
    ap_mac[3] = buf[3];
    ap_mac[4] = buf[4];
    ap_mac[5] = buf[5];

    user_role = GetValue(kv, STRING_USER_ROLE + ex);
    user_role_len = user_role.length();

    session_time = toInt(GetValue(kv, STRING_USER_SESSION_TIME + ex));
    radio_id = toInt(GetValue(kv, STRING_RADIO_ID + ex));
    wlan_id = toInt(GetValue(kv, TO_STR(STRING_WLAN_WLAN_ID) + ex));

    _elength = 36 + user_role_len;

    return 0;
}

int CImageIDDataDevModelTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataDevModelTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_DEV_MODEL"=" + data + ";");

    return 0;
}
int CImageIDDataDevModelTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataDevModelTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    data = GetValue(kv, STRING_DEV_MODEL);
    _elength += data.length();

    return 0;
}

int CImageIDDataSoftwareVersionTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataSoftwareVersionTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_SOFTWARE_VERSION"=" + data + ";");

    return 0;
}

int CImageIDDataSoftwareVersionTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataSoftwareVersionTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    data = GetValue(kv, STRING_SOFTWARE_VERSION);
    _elength += data.length();
    return 0;
}

int CImageIDDataFileNameTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataFileNameTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_FILE_NAME"=" + data + ";");

    return 0;
}

int CImageIDDataFileNameTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataFileNameTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    data = GetValue(kv, STRING_FILE_NAME);
    _elength += data.length();
    return 0;
}

int CImageIDDataFileServerTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataFileServerTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_FILE_SERVER"=" + data + ";");

    return 0;
}

int CImageIDDataFileServerTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataFileServerTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    data = GetValue(kv, STRING_FILE_SERVER);
    _elength += data.length();
    return 0;
}

int CImageIDDataDownloadTypeTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataDownloadTypeTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_DOWNLOAD_TYPE"=" + data + ";");

    return 0;
}

int CImageIDDataDownloadTypeTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataDownloadTypeTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    data = GetValue(kv, STRING_DOWNLOAD_TYPE);
    _elength += data.length();
    return 0;
}
int CImageIDDataFtpUserNameTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataFtpUserNameTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_FTP_USER_NAME"=" + data + ";");

    return 0;
}

int CImageIDDataFtpUserNameTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataFtpUserNameTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    data = GetValue(kv, STRING_FTP_USER_NAME);
    _elength += data.length();
    return 0;
}
int CImageIDDataFtpPasswordTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataFtpPasswordTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_FTP_PASSWORD"=" + data + ";");

    return 0;
}

int CImageIDDataFtpPasswordTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataFtpPasswordTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    data = GetValue(kv, STRING_FTP_PASSWORD);
    _elength += data.length();
    return 0;
}
int CImageIDDataFtpPathTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    data.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CImageIDDataFtpPathTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_FTP_PATH"=" + data + ";");

    return 0;
}

int CImageIDDataFtpPathTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;
    AssembleEH(buffer);

    buffer.storerawbytes((uint8_t *)data.c_str(), _elength);
    return 0;
}
int CImageIDDataFtpPathTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;
    data = GetValue(kv, STRING_FTP_PATH);
    _elength += data.length();
    return 0;
}

int CActlStaInfo::Parse(CBuffer &buffer)
{
    buffer.retrive32(pkt_id);
    buffer.retrive8(pkt_type);
    buffer.retrive8(msg_type);
    buffer.retrive32(user_role_len);
    user_role.assign((const char *)buffer.GetPtr(), user_role_len);
    buffer.retriverawbytes(NULL, user_role_len);
    buffer.retrive8(user_ip_type);
    if (IP_TYPE_IPV4 == user_ip_type)
        buffer.retriverawbytes(user_ip, 4);
    else
        buffer.retriverawbytes(user_ip, 16);
    buffer.retrive8(mac_len);
    buffer.retriverawbytes(mac, mac_len);
    buffer.retrive32(acc_session_time);
    buffer.retrive32(tx_bytes_high);
    buffer.retrive32(tx_bytes);
    buffer.retrive32(rx_bytes_high);
    buffer.retrive32(rx_bytes);
    buffer.retrive32(tx_pkts);
    buffer.retrive32(rx_pkts);
    buffer.retrive8(acc_down_cause);
    buffer.retrive8(radio_id);
    buffer.retrive8(wlan_id);

    return 0;
}
int CActlStaInfo::SaveTo(string &str, string ex)
{
    char buf[128] = {0};

    str.append(STRING_PKT_ID + ex + "=" + toString(pkt_id) + ";");
    str.append(STRING_PKT_TYPE + ex + "=" + toString(pkt_type) + ";");
    str.append(STRING_MSG_TYPE + ex + "=" + toString(msg_type) + ";");
    str.append(STRING_USER_ROLE+ ex + "=" + user_role + ";");

    // str.append(STRING_IP_TYPE + ex + "=" + toString(user_ip_type) + ";");

    snprintf(buf, sizeof(buf), IP_ADDR_FMT, IP_ADDR_VAL(user_ip));
    str.append(STRING_USER_IP + ex + "=" + toString(buf) + ";");

    // str.append(STRING_MAC_LEN + ex + "=" + toString(mac_len) + ";");

    snprintf(buf, sizeof(buf), ETHER_ADDR_FMT, ETHER_ADDR_VAL(mac));
    str.append(STRING_STA_MAC + ex + "=" + toString(buf) + ";");

    str.append(STRING_USER_SESSION_TIME + ex + "=" + toString(acc_session_time) + ";");
    str.append(STRING_TX_BYTES_HIGH + ex + "=" + toString(tx_bytes_high) + ";");
    str.append(STRING_TX_BYTES + ex + "=" + toString(tx_bytes) + ";");
    str.append(STRING_RX_BYTES_HIGH + ex + "=" + toString(rx_bytes_high) + ";");
    str.append(STRING_RX_BYTES + ex + "=" + toString(rx_bytes) + ";");
    str.append(STRING_TX_PAKCETS + ex + "=" + toString(tx_pkts) + ";");
    str.append(STRING_RX_PAKCETS + ex + "=" + toString(rx_pkts) + ";");
    str.append(STRING_DOWN_CAUSE + ex + "=" + toString(acc_down_cause) + ";");
    str.append(STRING_RADIO_ID + ex + "=" + toString(radio_id) + ";");
    str.append(TO_STR(STRING_WLAN_WLAN_ID) + ex + "=" + toString(wlan_id) + ";");

    return 0;
}

int CVSActlReportStaInfoTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    buffer.retrive8(count);
    sta_infos.resize(count);
    for (size_t i=0; i<sta_infos.size(); i++)
    {
        sta_infos[i].Parse(buffer);
    }

    return 0;
}
int CVSActlReportStaInfoTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_USER_COUNT"=" + toString(count) + ";");
    for (size_t i=0; i<sta_infos.size(); i++)
    {
        sta_infos[i].SaveTo(str, toString(i));
    }

    return 0;
}

int CVSResultStrTlv::Parse(CBuffer &buffer)
{
    if (0 != ParseEH(buffer))
        return 0;

    result.assign((const char *)buffer.GetPtr(), _elength);
    buffer.retriverawbytes(NULL, _elength);

    return 0;
}
int CVSResultStrTlv::SaveTo(string &str, string ex)
{
    if (!isValid())
        return 0;

    str.append(STRING_RESULT_STR"=" + result + ";");

    return 0;
}

int CVSReverseSSHTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);

    buffer.store8(ssh_enable);
    buffer.store32(listen_port);
    buffer.store32(data_port);
    buffer.store32(server_port);
    buffer.store16(addr_len);
    buffer.storerawbytes((uint8_t*)addr.c_str(), addr_len);

    return 0;
}
int CVSReverseSSHTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    ssh_enable = toInt8(GetValue(kv, STRING_REVERSE_SSH_ENABLE + ex));
    listen_port = toInt(GetValue(kv, STRING_REVERSE_SSH_LISTEN_PORT + ex));
    data_port = toInt(GetValue(kv, STRING_REVERSE_SSH_DATA_PORT + ex));
    server_port = toInt(GetValue(kv, STRING_REVERSE_SSH_SERVER_PORT + ex));
    addr = GetValue(kv, STRING_REVERSE_SSH_ADDR + ex);
    addr_len = addr.length();

    _elength = 15 + addr_len;

    return 0;
}

int CVSRadio11ACConfTlv::Assemble(CBuffer &buffer)
{
    if (!isValid())
        return 0;

    AssembleEH(buffer);
    buffer.store8(radio_id);
    buffer.store8(ldpc_enable);
    buffer.store8(tx_enable);
    buffer.store8(rx_enable);
    buffer.store32(work_rate_11ac);

    return 0;
}
int CVSRadio11ACConfTlv::LoadFrom(kvlist &kv, string ex)
{
    if (!isValid())
        return 0;

    radio_id = toInt8(GetValue(kv, TO_STR(STRING_RADIO_ID) + ex));
    ldpc_enable = toInt8(GetValue(kv, TO_STR(STRING_RADIO_LDPC) + ex));
    tx_enable = toInt8(GetValue(kv, TO_STR(STRING_RADIO_ACTX) + ex));
    rx_enable = toInt8(GetValue(kv, TO_STR(STRING_RADIO_ACRX) + ex));
    work_rate_11ac = toInt(GetValue(kv, TO_STR(STRING_RADIO_RATE_11AC) + ex));

    _elength = 8;

    return 0;
}
