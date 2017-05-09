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
#define ETHER_ADDR_RVAL(addr) (&addr[0]),(&addr[1]), \
        (&addr[2]),(&addr[3]), \
        (&addr[4]),(&addr[5])

#define IP_ADDR_FMT "%u.%u.%u.%u"
#define IP_ADDR_VAL(addr) (addr[0]),(addr[1]),(addr[2]),(addr[3])
#define IP_ADDR_RVAL(addr) (&addr[0]),(&addr[1]),(&addr[2]),(&addr[3])
#define __FUNC__ (toUpper(__func__).c_str())
#define _TO_STR(str) #str
#define TO_STR(str) _TO_STR(str)

#define AP_LIST     ap_list
#define GROUP_LIST  group_list
#define RADIO_2G_LIST radio_2g_list
#define RADIO_5G_LIST radio_5g_list
#define WLAN_LIST wlan_list
#define WLAN_SECURE_LIST wlan_secure_list
#define WP_LIST wp_list
#define LAN_PORTAL_LIST lan_portal_list
#define RFG_LIST rfg_list
#define RATE_SET_LIST rate_set_list
#define NTP_SERVER_LIST ntp_server_list
#define PORTAL_CUSTOM_LIST portal_custom_list
#define UPGRADE_STRATEGY_LIST upgrade_strategy_list

#define STRING_AP_MAC "ap_mac"
#define STRING_SERIAL_NUMBER "serial_number"
#define STRING_POSITION position
#define STRING_WAN_IPADDR "wan_ip"
#define STRING_LAN_IPADDR "lan_ip"
#define STRING_STATE "state"
#define STRING_GROUP_NAME group_name
#define STRING_DEV_MODEL "dev_model"
#define STRING_HARDWARE_VERSION "hw_version"
#define STRING_SOFTWARE_VERSION "sw_version"
#define STRING_COMPANY_ID "company"
#define STRING_DST_WHITE "dst_white_name"
#define STRING_DST_BLACK "dst_black_name"
#define STRING_SRC_WHITE "src_white_name"
#define STRING_UPGRADE_CNT "upgrade_count"
#define STRING_ONLINE_TIME "online_time"
#define STRING_LEAVE_TIME "offline_time"
#define STRING_CREATE_TIME "create_time"
#define STRING_TX_SPEED "tx_speed"
#define STRING_RX_SPEED "rx_speed"

#define STRING_NAME "name"
#define STRING_DESCRIBE "describe"
#define STRING_REPORT_STATION_INFO_ENABLE   report_station_info_enable
#define STRING_REPORT_STATION_INFO_INTERVAL report_station_info_interval
#define STRING_ROMING_CONFIG_ENABLE roming_enable
#define STRING_ROMING_CONFIG_SIGNAL roming_signal
#define STRING_LOW_RSSI_REFUSE_ENABLE low_rssi_refuse_enable
#define STRING_LOW_RSSI_THRESHOLD low_rssi_refuse_threshold
#define STRING_BY_PASS_ENABLE by_pass_enable
#define STRING_AP_LOADBALANCE_ENABLE lb_enable
#define STRING_AP_LOADBALANCE_THRESHOLD lb_threshold
#define STRING_AP_LOADBALANCE_INTERVAL lb_interval
#define STRING_LAN_VLAN_ID lan_vlan_id
#define STRING_WP_STRATEGY_NAME wp_strategy_name
#define STRING_RFG_STRATEGY_NAME rfg_strategy_name
#define STRING_RATE_SET_STRATEGY_NAME rateset_strategy_name
#define STRING_LAN_STRATEGY_NAME lan_strategy_name
#define STRING_RADIO_2G_STRATEGY_NAME radio_2g_strategy_name
#define STRING_RADIO_5G_STRATEGY_NAME radio_5g_strategy_name
#define STRING_WLAN_2G_STRATEGY_NAME wlan_2g_strategy_name
#define STRING_WLAN_5G_STRATEGY_NAME wlan_5g_strategy_name
#define STRING_NTP_STRATEGY_NAME ntp_strategy_name
#define STRING_PORTAL_PARAM_CUSTOM_STRATEGY_NAME portal_custom_strategy_name

#define STRING_RADIO_ENABLE radio_enable
#define STRING_RADIO_RADIO_TYPE radio_type
#define STRING_RADIO_CHANNEL radio_channel
#define STRING_RADIO_TXPOWER radio_txpower
#define STRING_RADIO_SPACE radio_space
#define STRING_RADIO_LDPC radio_ldpc
#define STRING_RADIO_ACTX radio_actx
#define STRING_RADIO_ACRX radio_acrx
#define STRING_RADIO_RATE_11AC radio_rate_11ac
#define STRING_RADIO_CHWIDTH radio_chwidth
#define STRING_RADIO_RTS radio_rts
#define STRING_RADIO_FRAGMENT radio_fragment
#define STRING_RADIO_BEACON_INTV radio_beacon_interval
#define STRING_RADIO_RESPONSE_TIMEOUT radio_response_timeout
#define STRING_RADIO_MIN_SEND_SPEED radio_min_send_speed
#define STRING_RADIO_BEACON_SEND_SPEED radio_beacon_send_speed
#define STRING_RADIO_PROTECT_MODE radio_protect_mode
#define STRING_RADIO_PROBE_SUPPRESSION radio_probe_suppression
#define STRING_RADIO_DTIM_INTV radio_dtim_interval
#define STRING_RADIO_CCA_ADJ radio_cca_adj
#define STRING_RADIO_CCA_THR radio_cca_thr
#define STRING_RADIO_A_MPDU radio_ampdu
#define STRING_RADIO_A_MSDU radio_amsdu
#define STRING_RADIO_SHORT_GI radio_shortgi
#define STRING_RADIO_COUNTRY_CODE radio_country_code
#define STRING_RADIO_11NONLY radio_11nonly
#define STRING_RADIO_SHORT_PREAMBLE radio_short_preamble
#define STRING_RADIO_AUTO_CHANNEL radio_auto_channel
#define STRING_RADIO_AUTO_CHANNEL_PERIOD radio_auto_channel_period
#define STRING_RADIO_AUTO_POWER radio_auto_power
#define STRING_RADIO_PKT_POWER_ENABLE pkt_power_enable
#define STRING_RADIO_PKT_POWER_MIN_POWER pkt_power_min_power
#define STRING_RADIO_PKT_POWER_MIN_THROUGHPUT_THRESHOLD radio_pkt_power_min_throughput_threshold
#define STRING_RADIO_PKT_POWER_MIN_RATE_THRESHOLD radio_pkt_power_min_rate_threshold

#define STRING_INDEX "id"
#define STRING_WLAN_ESSID wlan_essid  //禁止修改
#define STRING_WLAN_ESSID_ENCODE wlan_essid_encode
#define STRING_WLAN_AUTH_TYPE wlan_auth_type
#define STRING_WLAN_PORTAL_TYPE "wlan_portal_type"
#define STRING_WLAN_PORTAL_COSTOM_NAME "wlan_portal_custom_name" // portal页定制策略
#define STRING_WLAN_PORTAL_URL wlan_portal_url
#define STRING_WLAN_WX_USER "wlan_wx_user_name"
#define STRING_WLAN_SECURE_STRATEGY wlan_secure_strategy_name
#define STRING_WLAN_HIDE_SSID wlan_hide_ssid
#define STRING_WLAN_WDS_ENABLE wlan_wds_enable
#define STRING_WLAN_VLAN_ID wlan_vlan_id
#define STRING_WLAN_WLAN_ID wlan_wlan_id
#define STRING_WLAN_MAX_USER wlan_max_user
#define STRING_WLAN_SSID_DOWN wlan_ssid_down_rate
#define STRING_WLAN_SSID_UP wlan_ssid_up_rate
#define STRING_WLAN_USER_DOWN wlan_user_down_rate
#define STRING_WLAN_USER_UP wlan_user_up_rate
#define STRING_WLAN_QOS_ENABLE wlan_qos_enable
#define STRING_WLAN_TUNNEL_ENABLE wlan_tunnel_enable
#define STRING_WLAN_USER_ISOLATE wlan_user_isolate
#define STRING_WLAN_MULTICAST_TO_UNICAST_ENABLE wlan_multicast_to_unicast_enable
#define STRING_WLAN_MULTICAST_TO_UNICAST_MAX_STA wlan_multicast_to_unicast_maxsta
#define STRING_WLAN_MULTICAST_TO_UNICAST_TIMEOUT wlan_multicast_to_unicast_timeout
#define STRING_WLAN_MAC_FILTER_RULES mac_filter_rules

#define STRING_WLAN_SECURE_TYPE wlan_secure_type
#define STRING_WLAN_SECURE_KEY_TYPE "wlan_secure_key_type"
#define STRING_WLAN_SECURE_KEY_INDEX wlan_secure_key_index
#define STRING_WLAN_SECURE_KEY_LENGTH wlan_secure_key_length
#define STRING_WLAN_SECURE_KEY wlan_secure_key

#define STRING_WP_ENABLE wp_enable
#define STRING_WP_INTERVAL wp_interval
#define STRING_WP_SERVER_IP wp_server_ip
#define STRING_WP_SERVER_PORT wp_server_port
#define STRING_WP_SCAN_TYPE wp_scan_type
#define STRING_WP_CODE wp_code
#define STRING_WP_PROTO wp_proto
#define STRING_EF_ENABLE ef_enable
#define STRING_EF_INTERVAL ef_interval
#define STRING_EF_SERVER_IP ef_server_ip
#define STRING_EF_SERVER_PORT ef_server_port
#define STRING_EF_SCAN_TYPE ef_scan_type
#define STRING_EF_CODE ef_code
#define STRING_EF_PROTO ef_proto
#define STRING_WE_AD_INTERVAL we_ad_interval
#define STRING_WE_CHANNEL_2G we_channel_2g
#define STRING_WE_CHANNEL_5G we_channel_5g
#define STRING_WE_AD_RSSI we_ad_rssi

#define STRING_LAN_PORTAL_ENABLE lan_portal_enable
#define STRING_LAN_PORTAL_URL lan_portal_url

#define STRING_RFG_ENABLE rfg_enable
#define STRING_RFG_ASSOC_MAX rfg_assoc_max
#define STRING_RFG_TIMEOUT rfg_timeout
#define STRING_RFG_MAX_STA rfg_max_sta
#define STRING_RFG_METHOD rfg_method

#define STRING_RATE_SET_11A rate_set_11a
#define STRING_RATE_SET_11BG rate_set_11bg
#define STRING_RATE_SET_11N rate_set_11n
#define STRING_RATE_SET_11AC rate_set_11ac

#define STRING_NTP_SERVER_ADDR ntp_server_addr
#define STRING_NTP_SYNC_INTERVAL ntp_sync_interval

#define STRING_PORTAL_CUSTOM_DATA portal_custom_data
/**
   portal 参数定制数据库保存格式, 不能出现'='和';'

   {
    "portal_custom_count":"2","portal_custom_content":
    [
    {
     "portal_custom_key":"key_aa",
     "portal_custom_alias":"alias_aa",
     "portal_custom_value":"value_aa"
    },
    {
     "portal_custom_key":"key_bb",
     "portal_custom_alias":"alias_bb",
     "portal_custom_value":"value_bb"
    }
    ]
   }
*/
#define STRING_PORTAL_CUSTOM_COUNT portal_custom_count
#define STRING_PORTAL_CUSTOM_CONTENT portal_custom_content
#define STRING_PORTAL_CUSTOM_KEY portal_custom_key
#define STRING_PORTAL_CUSTOM_ALIAS portal_custom_alias
#define STRING_PORTAL_CUSTOM_VALUE portal_custom_value


// 终端相关宏定义
#define STRING_PKT_ID "pkt_id"
#define STRING_PKT_TYPE "pkt_type"
#define STRING_MSG_TYPE "msg_type"
#define STRING_SUB_TYPE "sub_type"
#define STRING_STA_MAC "sta_mac"
#define STRING_USER_IP "user_ip"
#define STRING_USER_ROLE "user_role"
#define STRING_USER_COUNT "user_count"
#define STRING_USER_SESSION_TIME "user_session_time"
#define STRING_TX_BYTES_HIGH "tx_bytes_high"
#define STRING_TX_BYTES "tx_bytes"
#define STRING_RX_BYTES_HIGH "rx_bytes_high"
#define STRING_RX_BYTES "rx_bytes"
#define STRING_TX_PAKCETS "tx_packets"
#define STRING_RX_PAKCETS "rx_packets"
#define STRING_DOWN_CAUSE "down_cause"

// AC自身配置相关宏
#define STRING_AC_NAME "ac_name"
#define STRING_CTRL_IPV4_ADDR "ac_ctrl_ipv4_addr"
#define STRING_ECHO_INTERVAL "echo_interval"
#define STRING_ECHO_COUNT "echo_count"
#define STRING_TRAFFIC_STATICS_INTERVAL "traffic_statics_interval"
#define STRING_TRAFFIC_STATICS_ENABLE "traffic_statics_enable"
#define STRING_UPGRADE_ENABLE "upgrade_enable"

// 升级相关配置宏
#define STRING_FILE_NAME "file_name"
#define STRING_FILE_SERVER "file_server"
#define STRING_DOWNLOAD_TYPE "download_type"
#define STRING_FTP_USER_NAME "ftp_user_name"
#define STRING_FTP_PASSWORD "ftp_password"
#define STRING_FTP_PATH "ftp_path"

// 其它宏
#define STRING_RESULT_CODE "result_code"
#define STRING_RESULT_STR "result_str"
#define STRING_RADIO_ID "radio_id"  // 数据库中无此配置
#define STRING_COUNT "count"

// 反向SSH宏
#define STRING_REVERSE_SSH_ENABLE "reverse_ssh_enable"
#define STRING_REVERSE_SSH_LISTEN_PORT "listen_port"
#define STRING_REVERSE_SSH_DATA_PORT "data_port"
#define STRING_REVERSE_SSH_SERVER_PORT "server_port"
#define STRING_REVERSE_SSH_ADDR "addr"

// AP密码修改宏
#define STRING_FRONT_PASSWORD "front_password"
#define STRING_SSH_PASSWORD "ssh_password"

// 修改AC地址
#define STRING_AC_ADDR "ac_ipaddr"

// 修改运行模式
#define STRING_RUN_MODE "run_mode"


string toString(uint32_t value);
string toString(uint64_t value);
string toString(int value);
string toString(time_t value);
string toString(char *value);
string toString(uint8_t value);
string toString(uint16_t value);
string toString(bool value);
uint8_t  toInt8(string value);
uint16_t toInt16(string value);
uint32_t toInt32(string value);
uint32_t toInt(string value);
uint64_t toInt64(string value);
vector<string> split(string &str, string d=";");
vector<string> split(const char *str, const char *d=";");
int format_macaddr(string &mac);
string toUpper(string src);
string toUpper(const char *src);


uint64_t htonll(uint64_t val);
uint64_t ntohll(uint64_t val);

#endif
