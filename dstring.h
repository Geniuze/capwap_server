#ifndef __DSTRING_H__
#define __DSTRING_H__

#include <string>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ETHER_ADDR_FMT "%02X-%02X-%02X-%02X-%02X-%02X"
#define ETHER_ADDR_VAL(addr) (addr[0]),(addr[1]),   \
        (addr[2]),(addr[3]),                        \
        (addr[4]),(addr[5])




#define STRING_AP_MAC "apmac"
#define STRING_AP_MAC_LEN "apmac_len"
#define STRING_SERIAL_NUMBER "serial_number"
#define STRING_DEV_MODEL "dev_model"
#define STRING_COMPANY_ID "company_id"
#define STRING_HARDWARE_VERSION "hardware_version"
#define STRING_SOFTWARE_VERSION "software_version"
#define STRING_LAN_IPADDR_TYPE "lan_ipaddr_type"
#define STRING_LAN_IPADDR "lan_ipaddr"
#define STRING_WAN_IPADDR "wan_ipaddr"
#define STRING_RADIO_ID "radio_id"
#define STRING_RADIO_TYPE_11N "radio_type_11n"
#define STRING_RADIO_TYPE_11A "radio_type_11a"
#define STRING_RADIO_TYPE_11B "radio_type_11b"
#define STRING_RADIO_TYPE_11G "radio_type_11g"
#define STRING_SHORT_PREAMBLE "short_preamble"
#define STRING_BSSID "bssid"
#define STRING_BSSIDS_NUMBER  "bssids_number"
#define STRING_DTIM_PERIOD "dtim_period"
#define STRING_BEACON_PERIOD "beacon_period"
#define STRING_COUNTRY_STRING "country"
#define STRING_CURRENT_TXPOWER "cur_txpower"
#define STRING_CURRENT_CHANNEL "cur_channel"
#define STRING_CURRENT_CCA "cur_cca"
#define STRING_ENERGY_DETECT_THRESHOLD "energy_detect_threshold"
#define STRING_CTRL_IPV4_ADDR "ctrl_ipv4_address"
#define STRING_AC_NAME "ac_name"
#define STRING_AP_TRANS_TYPE "ap_trans_type"
#define STRING_MAX_RADIOS "max_radios"
#define STRING_RADIO_INUSE "radio_inuse"
#define STRING_ENCRYPT_NUMBER "encrypt_number"
#define STRING_ENCRYPT_WBID "encrypt_wbid"
#define STRING_ENCRYPT_CAPABILITIES "encrypt_capabilities"
#define STRING_RESULT_CODE "result_code"
#define STRING_RADIO_STATE "radio_state"
#define STRING_RADIO_CAUSE "radio_cause"
#define STRING_RTS_THRESHOLD "rts_threshold"
#define STRING_SHORT_RETRY "short_retry"
#define STRING_FRAG_THR "frag_thr"
#define STRING_TX_MSDU_LIFETIME "tx_msdu_lifetime"
#define STRING_RX_MSDU_LIFETIME "rx_msdu_lifetime"
#define STRING_BAND_WIDTH "band_width"
#define STRING_TI_THRESHOLD "ti_threshold"

#define STRING_A_MSDU "amsdu"
#define STRING_A_MPDU "ampdu"
#define STRING_11N_ONLY "11n_only"
#define STRING_SHORT_GI "short_gi"
#define STRING_MAX_SUPPORT_MCS "max_support_mcs"
#define STRING_MAX_MANDANTORY_MCS "max_mandantory_mcs"
#define STRING_TX_ANTENNA "tx_antenna"
#define STRING_RX_ANTENNA "rx_antenna"
#define STRING_AUTO_CHANNEL_SWITCH "auto_channel_switch"
#define STRING_AUTO_CHANNEL_PERIOD "auto_channel_period"
#define STRING_AUTO_POWER_SWITCH "auto_power_switch"
#define STRING_ACK_TIMEOUT "ack_timeout"
#define STRING_BEACON_RATE "beacon_rate"
#define STRING_PROTECT_MODE "protect_mode"
#define STRING_PROBE_RSSI "probe_rssi"
#define STRING_MIN_CONTRACT_RATE "min_contract_rate"
#define STRING_ECHO_INTERVAL "echo_interval"
#define STRING_ECHO_TIMEOUT_COUNT "echo_timeout_cnt"
#define STRING_TRAFFIC_STATICS_INTERVAL "traffic_statics_interval"
#define STRING_TRAFFIC_SWITCH "traffic_switch"
#define STRING_NTP_SERVER_INTERVAL "ntp_server_interval"
#define STRING_NTP_SERVER "ntp_server"
#define STRING_MIN_POWER "min_power"
#define STRING_MIN_THROUGHPUT_THRESHOLD "min_throughput_threshold"
#define STRING_MIN_RATE_THRESHOLD "min_rate_threshold"
#define STRING_PPC_ENABLE "ppc_enable"
#define STRING_CCA_ADJ "cca_adj"
#define STRING_CCA_THR "cca_thr"
#define STRING_AP_SPACE "ap_space"
#define STRING_DATA_TYPE "data_type"
#define STRING_DATA_MODE "data_mode"
#define STRING_DATA_DATA "data"

string toString(uint32_t value);
string toString(int value);
string toString(time_t value);
string toString(char *value);
string toString(uint8_t value);
string toString(uint16_t value);
uint32_t toInt(string value);
uint64_t toInt64(string value);
vector<string> split(string &str, string d=";");
int format_macaddr(string &mac);

#endif
