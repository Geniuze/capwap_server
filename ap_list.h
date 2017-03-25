#ifndef _AP_LIST_H_
#define _AP_LIST_H_

#include "dbi.h"
#include "kvlist.h"

#define HW_ADDR_LEN 6

#define INFO_LEN32 32
#define INFO_LEN64 64
#define INFO_LEN128 128
#define MAX_RADIOS 8

#define CAPWAP_SERVER_DB "/etc/config/cwserver.db"
#define AP_LIST     "ap_list"
#define GROUP_LIST  "group_list"
#define RADIO_2G_LIST "radio_2g_list"
#define RADIO_5G_LIST "radio_5g_list"
#define WLAN_LIST "wlan_list"
#define WLAN_SECURE_LIST "wlan_secure_list"

#define RADIO_TYPE_11B 0x01
#define RADIO_TYPE_11A 0x02
#define RADIO_TYPE_11G 0x04
#define RADIO_TYPE_11N 0x08

#define DB_STRING_AP_MAC "mac"
#define DB_STRING_SERIAL_NUMBER "serial_number"
#define DB_STRING_POSITION "position"
#define DB_STRING_WAN_IP "wan_ip"
#define DB_STRING_LAN_IP "lan_ip"
#define DB_STRING_STATE "state"
#define DB_STRING_GROUP_NAME "group_name"
#define DB_STRING_DEV_MODEL "dev_model"
#define DB_STRING_HARDWARE_VERSION "hardware_version"
#define DB_STRING_SOFTWARE_VERSION "software_version"
#define DB_STRING_COMPANY_ID "company_id"
#define DB_STRING_DST_WHITE_LIST "dst_white_list"
#define DB_STRING_DST_BLACK_LIST "dst_black_list"
#define DB_STRING_SRC_WHITE_LIST "src_white_list"
#define DB_STRING_UPGRADE_CNT "upgrade_cnt"
#define DB_STRING_ONLINE_TIME "online_time"
#define DB_STRING_LEAVE_TIME "leave_time"
#define DB_STRING_CREATE_TIME "create_time"


#define INIT_AP_LIST_TABLE \
    "create table ["AP_LIST"] (" \
    DB_STRING_AP_MAC" char(32) not null,"       \
    DB_STRING_SERIAL_NUMBER" char(32) not null,"   \
    DB_STRING_POSITION" varchar(128),"     \
    DB_STRING_WAN_IP" varchar(128),"     \
    DB_STRING_LAN_IP" varchar(128),"          \
    DB_STRING_STATE" int(1) default 0,"       \
    DB_STRING_GROUP_NAME" varchar(128) default ('default'),"  \
    DB_STRING_DEV_MODEL" char(32),"                  \
    DB_STRING_HARDWARE_VERSION" varchar(64),"   \
    DB_STRING_SOFTWARE_VERSION" varchar(64),"   \
    DB_STRING_COMPANY_ID" varchar(128),"  \
    DB_STRING_DST_WHITE_LIST" char(32) default ('default'), " \
    DB_STRING_DST_BLACK_LIST" char(32) default ('default'), " \
    DB_STRING_SRC_WHITE_LIST" char(32) default ('default'), " \
    DB_STRING_UPGRADE_CNT" int(1) default 0, "     \
    DB_STRING_ONLINE_TIME" int(8),"     \
    DB_STRING_LEAVE_TIME" int(8),"    \
    DB_STRING_CREATE_TIME" int(8), " \
    "primary key('"DB_STRING_AP_MAC"')" \
    ")"
enum
{
    DB_INDEX_AP_MAC,
    DB_INDEX_SERIAL_NUMBER,
    DB_INDEX_POSITION,
    DB_INDEX_WAN_IP,
    DB_INDEX_LAN_IP,
    DB_INDEX_STATE,
    DB_INDEX_GROUP_NAME,
    DB_INDEX_DEV_MODEL,
    DB_INDEX_HARDWARE_VERSION,
    DB_INDEX_SOFTWARE_VERSION,
    DB_INDEX_COMPANY_ID,
    DB_INDEX_DST_WHITE_LIST,
    DB_INDEX_DST_BLACK_LIST,
    DB_INDEX_SRC_WHITE_LIST,
    DB_INDEX_UPGRADE_CNT,
    DB_INDEX_ONLINE_TIME,
    DB_INDEX_LEAVE_TIME,
    DB_INDEX_CREATE_TIME,
};

#define DB_STRING_STATION_TRAFFIC_ENABLE "sta_traffic_enable"
#define DB_STRING_STATION_TRAFFIC_INTERVAL "sta_traffic_interval"
#define DB_STRING_STATION_ROMING_ENABLE "sta_roming_enable"
#define DB_STRING_STATION_ROMING_SIGNAL "sta_roming_signal"
#define DB_STRING_LOW_SIGNAL_ENABLE "sta_low_signal_enable"
#define DB_STRING_LOW_SIGNAL_THRESHOLD "sta_low_signal_threshold"
#define DB_STRING_BY_PASS_ENABLE "sta_by_pass_enable"
#define DB_STRING_WIRELESS_POSITION_NAME "wp_name"
#define DB_STRING_SPECTNUM_NAME "spectnum_name"
#define DB_STRING_STATION_DATA_COLLECTION "sta_collection"
#define DB_STRING_LAN_STRATEGY_NAME "lan_strategy"
#define DB_STRING_RADIO_2G_STRATEGY_NAME "radio2g_strategy"
#define DB_STRING_RADIO_5G_STRATEGY_NAME "radio5g_strategy"
#define DB_STRING_WLAN_2G_STRATEGY_NAME "wlan2g_strategy"
#define DB_STRING_WLAN_5G_STRATEGY_NAME "wlan5g_strategy"
#define DB_STRING_NTP_STRATEGY_NAME "ntp_strategy"
#define DB_STRING_PORTAL_PARAM_CUSTOM_STRATEGY_NAME "portal_custom"
#define DB_STRING_LOAD_BALANCE_ENABLE "lb_enable"
#define DB_STRING_SCAN_INTERVAL "scan_intv"
#define DB_STRING_LOAD_BALANCE_THRESHOLD "lb_threshold"
#define DB_STRING_VLAN_INTER_ID "vlan_inter_id"

#define INIT_GROUP_LIST_TABLE \
    "create table ["GROUP_LIST"] (" \
    DB_STRING_GROUP_NAME" varchar(64) not null," \
    DB_STRING_STATION_TRAFFIC_ENABLE" int(1) default 0," \
    DB_STRING_STATION_TRAFFIC_INTERVAL" int(4) default 30" \
    DB_STRING_STATION_ROMING_ENABLE" int(1) default 0," \
    DB_STRING_STATION_ROMING_SIGNAL" int(4) default 70," \
    DB_STRING_LOW_SIGNAL_ENABLE" int(1) default 0," \
    DB_STRING_LOW_SIGNAL_THRESHOLD" int(4) default 70," \
    DB_STRING_BY_PASS_ENABLE" int(1) default 0,"  \
    DB_STRING_LOAD_BALANCE_ENABLE" int(1) default 0,"            \
    DB_STRING_SCAN_INTERVAL" int(4) default 0,"                  \
    DB_STRING_LOAD_BALANCE_THRESHOLD" int(4) default 0,"          \
    DB_STRING_VLAN_INTER_ID" int(4) default 0,"                  \
    DB_STRING_WIRELESS_POSITION_NAME" varchar(64) default '',"   \
    DB_STRING_SPECTNUM_NAME" varchar(64) default '',"  \
    DB_STRING_STATION_DATA_COLLECTION" varchar(64) default '',"  \
    DB_STRING_LAN_STRATEGY_NAME" varchar(64) default '', "  \
    DB_STRING_RADIO_2G_STRATEGY_NAME" varchar(64) default 'default', " \
    DB_STRING_RADIO_5G_STRATEGY_NAME" varchar(64) default 'default', " \
    DB_STRING_WLAN_2G_STRATEGY_NAME" varchar(64) default 'default', "  \
    DB_STRING_WLAN_5G_STRATEGY_NAME" varchar(64) default 'default', "  \
    DB_STRING_NTP_STRATEGY_NAME" varchar(64) default '',"   \
    DB_STRING_PORTAL_PARAM_CUSTOM_STRATEGY_NAME" varchar(64) default ''," \
    "primary key('"DB_STRING_GROUP_NAME"')"           \
    ")"

enum
{
    DB_INDEX_GROUP_LIST_GROUP_NAME,
    DB_INDEX_STATION_TRAFFIC_ENABLE,
    DB_INDEX_STATION_TRAFFIC_INTERVAL,
    DB_INDEX_STATION_ROMING_ENABLE,
    DB_INDEX_STATION_ROMING_SIGNAL,
    DB_INDEX_LOW_SIGNAL_ENABLE,
    DB_INDEX_LOW_SIGNAL_THRESHOLD,
    DB_INDEX_BY_PASS_ENABLE,
    DB_INDEX_LOAD_BALANCE_ENABLE,
    DB_INDEX_SCAN_INTERVAL,
    DB_INDEX_LOAD_BALANCE_THRESHOLD,
    DB_INDEX_VLAN_INTER_ID,
    DB_INDEX_WIRELESS_POSITION_NAME,
    DB_INDEX_SPECTNUM_NAME,
    DB_INDEX_STATION_DATA_COLLECTION,
    DB_INDEX_LAN_STRATEGY_NAME,
    DB_INDEX_RADIO_2G_STRATEGY_NAME,
    DB_INDEX_RADIO_5G_STRATEGY_NAME,
    DB_INDEX_WLAN_2G_STRATEGY_NAME,
    DB_INDEX_WLAN_5G_STRATEGY_NAME,
    DB_INDEX_NTP_STRATEGY_NAME,
    DB_INDEX_PORTAL_PARAM_CUSTOM_STRATEGY_NAME,
};

#define DB_STRING_RADIO_2G_ENABLE "enable"
#define DB_STRING_RADIO_2G_RADIO_TYPE "radio_type"
#define DB_STRING_RADIO_2G_CHANNEL "channel"
#define DB_STRING_RADIO_2G_TXPOWER "txpower"
#define DB_STRING_RADIO_2G_SPACE "space"
#define DB_STRING_RADIO_2G_CHWIDTH "chwidth"
#define DB_STRING_RADIO_2G_RTS "rts"
#define DB_STRING_RADIO_2G_FRAGMENT "frag"
#define DB_STRING_RADIO_2G_BEACON_INTV "beacon_intv"
#define DB_STRING_RADIO_2G_RESPONSE_TIMEOUT "rsp_timeout"
#define DB_STRING_RADIO_2G_MIN_SEND_SPEED "min_speed"
#define DB_STRING_RADIO_2G_BEACON_SEND_SPEED "beacon_speed"
#define DB_STRING_RADIO_2G_PROTECT_MODE "protect_mode"
#define DB_STRING_RADIO_2G_PROBE_SUPPRESSION "probe_suppression"
#define DB_STRING_RADIO_2G_DTIM_INTV "dtim_intv"
#define DB_STRING_RADIO_2G_CCA_ADJ "cca_adj"
#define DB_STRING_RADIO_2G_CCA_THR "cca_thr"
#define DB_STRING_RADIO_2G_A_MPDU "a_mpdu"
#define DB_STRING_RADIO_2G_A_MSDU "a_msdu"
#define DB_STRING_RADIO_2G_SHORT_GI "short_gi"
#define DB_STRING_RADIO_2G_COUNTRY_CODE "country_code"
#define DB_STRING_RADIO_2G_11NONLY "nonly"
#define DB_STRING_RADIO_2G_SHORT_PREAMBLE "short_preamble"
#define DB_STRING_RADIO_2G_AUTO_CHANNEL "auto_channel"
#define DB_STRING_RADIO_2G_AUTO_POWER "auto_power"
#define DB_STRING_RADIO_2G_PKT_POWER_ENABLE "pkt_power_enable"
#define DB_STRING_RADIO_2G_PKT_POWER_MIN_POWER "pkt_power_min_power"
#define DB_STRING_RADIO_2G_PKT_POWER_MIN_THROUGHPUT_THRESHOLD "pkt_power_min_throughput_threshold"
#define DB_STRING_RADIO_2G_PKT_POWER_MIN_RATE_THRESHOLD "pkt_power_min_rate_threshold"

#define INIT_RADIO_2G_LIST_TABLE \
    "create table ["RADIO_2G_LIST"] (" \
    DB_STRING_RADIO_2G_STRATEGY_NAME" varchar(64) not null," \
    DB_STRING_RADIO_2G_ENABLE" int(1) default 1," \
    DB_STRING_RADIO_2G_RADIO_TYPE" int(4) default 13," \
    DB_STRING_RADIO_2G_CHANNEL" int(4) default 6," \
    DB_STRING_RADIO_2G_TXPOWER" int(1) default 20," \
    DB_STRING_RADIO_2G_SPACE" int(1) default 3," \
    DB_STRING_RADIO_2G_CHWIDTH" int(1) default 0,"\
    DB_STRING_RADIO_2G_RTS" int(4) default 2346," \
    DB_STRING_RADIO_2G_FRAGMENT" int(4) default 2346," \
    DB_STRING_RADIO_2G_BEACON_INTV" int(4) default 100," \
    DB_STRING_RADIO_2G_RESPONSE_TIMEOUT" int(4) default 77," \
    DB_STRING_RADIO_2G_MIN_SEND_SPEED" int(4) default 11," \
    DB_STRING_RADIO_2G_BEACON_SEND_SPEED" int(4) default 11," \
    DB_STRING_RADIO_2G_PROTECT_MODE" int(1) default 0," \
    DB_STRING_RADIO_2G_PROBE_SUPPRESSION" int(1) default 0," \
    DB_STRING_RADIO_2G_DTIM_INTV" int(4) default 1," \
    DB_STRING_RADIO_2G_CCA_ADJ" int(4) default 0," \
    DB_STRING_RADIO_2G_CCA_THR" int(4) default 10," \
    DB_STRING_RADIO_2G_A_MPDU" int(1) default 1," \
    DB_STRING_RADIO_2G_A_MSDU" int(1) default 1," \
    DB_STRING_RADIO_2G_SHORT_GI" int(1) default 0," \
    DB_STRING_RADIO_2G_COUNTRY_CODE" char(8) default 'CN'," \
    DB_STRING_RADIO_2G_11NONLY" int(1) default 0," \
    DB_STRING_RADIO_2G_SHORT_PREAMBLE" int(1) default 0," \
    DB_STRING_RADIO_2G_AUTO_CHANNEL" int(1) default 0," \
    DB_STRING_RADIO_2G_AUTO_POWER" int(1) default 0," \
    DB_STRING_RADIO_2G_PKT_POWER_ENABLE" int(1) default 0," \
    DB_STRING_RADIO_2G_PKT_POWER_MIN_POWER" int(1) default 0,"          \
    DB_STRING_RADIO_2G_PKT_POWER_MIN_THROUGHPUT_THRESHOLD" int(4) default 0," \
    DB_STRING_RADIO_2G_PKT_POWER_MIN_RATE_THRESHOLD" int(4) default 0," \
    "primary key('"DB_STRING_RADIO_2G_STRATEGY_NAME"')" \
    ")"

enum
{
    DB_INDEX_RADIO_2G_LIST_STRATEGY_NAMED,
    DB_INDEX_RADIO_2G_ENABLE,
    DB_INDEX_RADIO_2G_RADIO_TYPE,
    DB_INDEX_RADIO_2G_CHANNEL,
    DB_INDEX_RADIO_2G_TXPOWER,
    DB_INDEX_RADIO_2G_SPACE,
    DB_INDEX_RADIO_2G_CHWIDTH,
    DB_INDEX_RADIO_2G_RTS,
    DB_INDEX_RADIO_2G_FRAGMENT,
    DB_INDEX_RADIO_2G_BEACON_INTV,
    DB_INDEX_RADIO_2G_RESPONSE_TIMEOUT,
    DB_INDEX_RADIO_2G_MIN_SEND_SPEED,
    DB_INDEX_RADIO_2G_BEACON_SEND_SPEED,
    DB_INDEX_RADIO_2G_PROTECT_MODE,
    DB_INDEX_RADIO_2G_PROBE_SUPPRESSION,
    DB_INDEX_RADIO_2G_DTIM_INTV,
    DB_INDEX_RADIO_2G_CCA_ADJ,
    DB_INDEX_RADIO_2G_CCA_THR,
    DB_INDEX_RADIO_2G_A_MPDU,
    DB_INDEX_RADIO_2G_A_MSDU,
    DB_INDEX_RADIO_2G_SHORT_GI,
    DB_INDEX_RADIO_2G_COUNTRY_CODE,
    DB_INDEX_RADIO_2G_11NONLY,
    DB_INDEX_RADIO_2G_SHORT_PREAMBLE,
    DB_INDEX_RADIO_2G_AUTO_CHANNEL,
    DB_INDEX_RADIO_2G_AUTO_POWER,
    DB_INDEX_RADIO_2G_PKT_POWER_ENABLE,
    DB_INDEX_RADIO_2G_PKT_POWER_MIN_POWER,
    DB_INDEX_RADIO_2G_PKT_POWER_MIN_THROUGHPUT_THRESHOLD,
    DB_INDEX_RADIO_2G_PKT_POWER_MIN_RATE_THRESHOLD,
};

#define DB_STRING_RADIO_5G_ENABLE "enable"
#define DB_STRING_RADIO_5G_RADIO_TYPE "radio_type"
#define DB_STRING_RADIO_5G_CHANNEL "channel"
#define DB_STRING_RADIO_5G_TXPOWER "txpower"
#define DB_STRING_RADIO_5G_SPACE "space"
#define DB_STRING_RADIO_5G_CHWIDTH "chwidth"
#define DB_STRING_RADIO_5G_RTS "rts"
#define DB_STRING_RADIO_5G_FRAGMENT "frag"
#define DB_STRING_RADIO_5G_BEACON_INTV "beacon_intv"
#define DB_STRING_RADIO_5G_RESPONSE_TIMEOUT "rsp_timeout"
#define DB_STRING_RADIO_5G_MIN_SEND_SPEED "min_speed"
#define DB_STRING_RADIO_5G_BEACON_SEND_SPEED "beacon_speed"
#define DB_STRING_RADIO_5G_PROTECT_MODE "protect_mode"
#define DB_STRING_RADIO_5G_PROBE_SUPPRESSION "probe_suppression"
#define DB_STRING_RADIO_5G_DTIM_INTV "dtim_intv"
#define DB_STRING_RADIO_5G_CCA_ADJ "cca_adj"
#define DB_STRING_RADIO_5G_CCA_THR "cca_thr"
#define DB_STRING_RADIO_5G_A_MPDU "a_mpdu"
#define DB_STRING_RADIO_5G_A_MSDU "a_msdu"
#define DB_STRING_RADIO_5G_SHORT_GI "short_gi"
#define DB_STRING_RADIO_5G_COUNTRY_CODE "country_code"
#define DB_STRING_RADIO_5G_11NONLY "nonly"
#define DB_STRING_RADIO_5G_SHORT_PREAMBLE "short_preamble"
#define DB_STRING_RADIO_5G_AUTO_CHANNEL "auto_channel"
#define DB_STRING_RADIO_5G_AUTO_POWER "auto_power"
#define DB_STRING_RADIO_5G_PKT_POWER_ENABLE "pkt_power_enable"
#define DB_STRING_RADIO_5G_PKT_POWER_MIN_POWER "pkt_power_min_power"
#define DB_STRING_RADIO_5G_PKT_POWER_MIN_THROUGHPUT_THRESHOLD "pkt_power_min_throughput_threshold"
#define DB_STRING_RADIO_5G_PKT_POWER_MIN_RATE_THRESHOLD "pkt_power_min_rate_threshold"

#define INIT_RADIO_5G_LIST_TABLE \
    "create table ["RADIO_5G_LIST"] ("                       \
    DB_STRING_RADIO_5G_STRATEGY_NAME" varchar(64) not null," \
    DB_STRING_RADIO_5G_ENABLE" int(1) default 1," \
    DB_STRING_RADIO_5G_RADIO_TYPE" int(4) default 10," \
    DB_STRING_RADIO_5G_CHANNEL" int(4) default 149," \
    DB_STRING_RADIO_5G_TXPOWER" int(1) default 20," \
    DB_STRING_RADIO_5G_SPACE" int(1) default 3," \
    DB_STRING_RADIO_5G_CHWIDTH" int(1) default 0,"\
    DB_STRING_RADIO_5G_RTS" int(4) default 2346," \
    DB_STRING_RADIO_5G_FRAGMENT" int(4) default 2346," \
    DB_STRING_RADIO_5G_BEACON_INTV" int(4) default 100," \
    DB_STRING_RADIO_5G_RESPONSE_TIMEOUT" int(4) default 77," \
    DB_STRING_RADIO_5G_MIN_SEND_SPEED" int(4) default 11," \
    DB_STRING_RADIO_5G_BEACON_SEND_SPEED" int(4) default 11," \
    DB_STRING_RADIO_5G_PROTECT_MODE" int(1) default 0," \
    DB_STRING_RADIO_5G_PROBE_SUPPRESSION" int(1) default 0," \
    DB_STRING_RADIO_5G_DTIM_INTV" int(4) default 1," \
    DB_STRING_RADIO_5G_CCA_ADJ" int(4) default 0," \
    DB_STRING_RADIO_5G_CCA_THR" int(4) default 10," \
    DB_STRING_RADIO_5G_A_MPDU" int(1) default 1," \
    DB_STRING_RADIO_5G_A_MSDU" int(1) default 1," \
    DB_STRING_RADIO_5G_SHORT_GI" int(1) default 0," \
    DB_STRING_RADIO_5G_COUNTRY_CODE" char(8) default 'CN'," \
    DB_STRING_RADIO_5G_11NONLY" int(1) default 0," \
    DB_STRING_RADIO_5G_SHORT_PREAMBLE" int(1) default 0," \
    DB_STRING_RADIO_5G_AUTO_CHANNEL" int(1) default 0," \
    DB_STRING_RADIO_5G_AUTO_POWER" int(1) default 0," \
    DB_STRING_RADIO_5G_PKT_POWER_ENABLE" int(1) default 0,"             \
    DB_STRING_RADIO_5G_PKT_POWER_MIN_POWER" int(1) default 0,"          \
    DB_STRING_RADIO_5G_PKT_POWER_MIN_THROUGHPUT_THRESHOLD" int(4) default 0," \
    DB_STRING_RADIO_5G_PKT_POWER_MIN_RATE_THRESHOLD" int(4) default 0," \
    "primary key('"DB_STRING_RADIO_5G_STRATEGY_NAME"')" \
    ")"
enum
{
    DB_INDEX_RADIO_5G_LIST_STRATEGY_NAMED,
    DB_INDEX_RADIO_5G_ENABLE,
    DB_INDEX_RADIO_5G_RADIO_TYPE,
    DB_INDEX_RADIO_5G_CHANNEL,
    DB_INDEX_RADIO_5G_TXPOWER,
    DB_INDEX_RADIO_5G_SPACE,
    DB_INDEX_RADIO_5G_CHWIDTH,
    DB_INDEX_RADIO_5G_RTS,
    DB_INDEX_RADIO_5G_FRAGMENT,
    DB_INDEX_RADIO_5G_BEACON_INTV,
    DB_INDEX_RADIO_5G_RESPONSE_TIMEOUT,
    DB_INDEX_RADIO_5G_MIN_SEND_SPEED,
    DB_INDEX_RADIO_5G_BEACON_SEND_SPEED,
    DB_INDEX_RADIO_5G_PROTECT_MODE,
    DB_INDEX_RADIO_5G_PROBE_SUPPRESSION,
    DB_INDEX_RADIO_5G_DTIM_INTV,
    DB_INDEX_RADIO_5G_CCA_ADJ,
    DB_INDEX_RADIO_5G_CCA_THR,
    DB_INDEX_RADIO_5G_A_MPDU,
    DB_INDEX_RADIO_5G_A_MSDU,
    DB_INDEX_RADIO_5G_SHORT_GI,
    DB_INDEX_RADIO_5G_COUNTRY_CODE,
    DB_INDEX_RADIO_5G_11NONLY,
    DB_INDEX_RADIO_5G_SHORT_PREAMBLE,
    DB_INDEX_RADIO_5G_AUTO_CHANNEL,
    DB_INDEX_RADIO_5G_AUTO_POWER,
    DB_INDEX_RADIO_5G_PKT_POWER_ENABLE,
    DB_INDEX_RADIO_5G_PKT_POWER_MIN_POWER,
    DB_INDEX_RADIO_5G_PKT_POWER_MIN_THROUGHPUT_THRESHOLD,
    DB_INDEX_RADIO_5G_PKT_POWER_MIN_RATE_THRESHOLD,
};



#define DB_STRING_WLAN_STRATE_NAME "wlan_strategy_name"
#define DB_STRING_WLAN_ESSID "wlan_essid"
#define DB_STRING_WLAN_ESSID_ENCODE "wlan_essid_encode"
#define DB_STRING_WLAN_AUTH_TYPE "auth_type"
#define DB_STRING_WLAN_PORTAL_TYPE "portal_type"
#define DB_STRING_WLAN_PORTAL_URL "portal_url"
#define DB_STRING_WLAN_SECURE_STRATEGY "secure_strategy"
#define DB_STRING_WLAN_HIDE_SSID "hide_ssid"
#define DB_STRING_WLAN_WDS_ENABLE "wds_enable"
#define DB_STRING_WLAN_VLAN_ID "vlan_id"
#define DB_STRING_WLAN_WLAN_ID "wlan_id"
#define DB_STRING_WLAN_MAX_USER "max_user"
#define DB_STRING_WLAN_SSID_DOWN "ssid_down"
#define DB_STRING_WLAN_SSID_UP "ssid_up"
#define DB_STRING_WLAN_USER_DOWN "user_down"
#define DB_STRING_WLAN_USER_UP "user_up"
#define DB_STRING_WLAN_QOS_ENABLE "qos_enable"
#define DB_STRING_WLAN_TUNNEL_ENABLE "tunnel_enable"
#define DB_STRING_WLAN_USER_ISOLATE "user_isolate"
#define DB_STRING_WLAN_MULTICAST_TO_UNICAST "multi_to_unicast"

#define INIT_WLAN_LIST_TABLE \
    "create table ["WLAN_LIST"] ("                 \
    DB_STRING_WLAN_STRATE_NAME" varchar(64) not null," \
    DB_STRING_WLAN_ESSID" varchar(32) not null," \
    DB_STRING_WLAN_ESSID_ENCODE" int(1) default 0," \
    DB_STRING_WLAN_AUTH_TYPE" int(1) default 0," \
    DB_STRING_WLAN_PORTAL_TYPE" int(1) default 0," \
    DB_STRING_WLAN_PORTAL_URL" varchar(1024) default ''," \
    DB_STRING_WLAN_SECURE_STRATEGY" varchar(64) default ''," \
    DB_STRING_WLAN_HIDE_SSID" int(1) default 0," \
    DB_STRING_WLAN_WDS_ENABLE" int(1) default 0," \
    DB_STRING_WLAN_VLAN_ID" int(4) default 0," \
    DB_STRING_WLAN_WLAN_ID" int(1) default 0," \
    DB_STRING_WLAN_MAX_USER" int(4) default 0," \
    DB_STRING_WLAN_SSID_DOWN" int(4) defualt 0," \
    DB_STRING_WLAN_SSID_UP" int(4) default 0," \
    DB_STRING_WLAN_USER_DOWN" int(4) default 0," \
    DB_STRING_WLAN_USER_UP" int(4) default 0," \
    DB_STRING_WLAN_QOS_ENABLE" int(1) default 1," \
    DB_STRING_WLAN_TUNNEL_ENABLE" int(1) default 0," \
    DB_STRING_WLAN_USER_ISOLATE" int(1) default 0," \
    DB_STRING_WLAN_MULTICAST_TO_UNICAST" int(1) default 0," \
    "primary key ('"DB_STRING_WLAN_STRATE_NAME"')" \
    ")"

enum
{
    DB_INDEX_WLAN_LIST_STRATE_NAME,
    DB_INDEX_WLAN_ESSID,
    DB_INDEX_WLAN_ESSID_ENCODE,
    DB_INDEX_WLAN_AUTH_TYPE,
    DB_INDEX_WLAN_PORTAL_TYPE,
    DB_INDEX_WLAN_PORTAL_URL,
    DB_INDEX_WLAN_SECURE_STRATEGY,
    DB_INDEX_WLAN_HIDE_SSID,
    DB_INDEX_WLAN_WDS_ENABLE,
    DB_INDEX_WLAN_VLAN_ID,
    DB_INDEX_WLAN_WLAN_ID,
    DB_INDEX_WLAN_MAX_USER,
    DB_INDEX_WLAN_SSID_DOWN,
    DB_INDEX_WLAN_SSID_UP,
    DB_INDEX_WLAN_USER_DOWN,
    DB_INDEX_WLAN_USER_UP,
    DB_INDEX_WLAN_QOS_ENABLE,
    DB_INDEX_WLAN_TUNNEL_ENABLE,
    DB_INDEX_WLAN_USER_ISOLATE,
    DB_INDEX_WLAN_MULTICAST_TO_UNICAST,
};

#define DB_STRING_WLAN_SECURE_STRATEGY_NAME "wlan_secure_strategy_name"
#define DB_STRING_WLAN_SECURE_TYPE "wlan_secure_type"
#define DB_STRING_WLAN_SECURE_KEY_TYPE "wlan_secure_key_type"
#define DB_STRING_WLAN_SECURE_KEY_INDEX "wlan_secure_key_index"
#define DB_STRING_WLAN_SECURE_KEY_LENGTH "wlan_secure_key_length"
#define DB_STRING_WLAN_SECURE_PASSWORD "wlan_secure_password"

#define INIT_WLAN_SECURE_LIST_TABLE \
    "create table ["WLAN_SECURE_LIST"] (" \
    DB_STRING_WLAN_SECURE_STRATEGY_NAME" varchar(64) not null," \
    DB_STRING_WLAN_SECURE_TYPE" int(1) default 0," \
    DB_STRING_WLAN_SECURE_KEY_TYPE" int(1) default 0," \
    DB_STRING_WLAN_SECURE_KEY_INDEX" int(1) default 1," \
    DB_STRING_WLAN_SECURE_KEY_LENGTH" int(4) default 0," \
    DB_STRING_WLAN_SECURE_PASSWORD" varchar(128) default ''," \
    "primary key ('"DB_STRING_WLAN_SECURE_STRATEGY_NAME"')" \
    ")"
enum
{
    DB_INDEX_WLAN_SECURE_LIST_STRATEGY_NAME,
    DB_INDEX_WLAN_SECURE_TYPE,
    DB_INDEX_WLAN_SECURE_KEY_TYPE,
    DB_INDEX_WLAN_SECURE_KEY_INDEX,
    DB_INDEX_WLAN_SECURE_KEY_LENGTH,
    DB_INDEX_WLAN_SECURE_PASSWORD,
};

#define WP_LIST "wp_list"
#define DB_STRING_WP_ENABLE "wp_enable"
#define DB_STRING_WP_INTERVAL "wp_interval"
#define DB_STRING_WP_SERVER_IP "wp_server_ip"
#define DB_STRING_WP_SERVER_PORT "wp_server_port"
#define DB_STRING_WP_SCAN_TYPE "wp_scan_type"
#define DB_STRING_WP_CODE "wp_code"
#define DB_STRING_WP_PROTO "wp_proto"
#define DB_STRING_EF_ENABLE "wp_enable"
#define DB_STRING_EF_INTERVAL "wp_interval"
#define DB_STRING_EF_SERVER_IP "wp_server_ip"
#define DB_STRING_EF_SERVER_PORT "wp_server_port"
#define DB_STRING_EF_SCAN_TYPE "wp_scan_type"
#define DB_STRING_EF_CODE "wp_code"
#define DB_STRING_EF_PROTO "wp_proto"
#define DB_STRING_WE_AD_INTERVAL "we_ad_interval"
#define DB_STRING_WE_CHANNEL_2G "we_channel_2g"
#define DB_STRING_WE_CHANNEL_5G "we_channel_5g"
#define DB_STRING_WE_AD_RSSI "we_ad_rssi"

#define INIT_WP_LIST_TABLE \
    "create table ["WP_LIST"] (" \
    DB_STRING_WIRELESS_POSITION_NAME" varchar(64) not null," \
    DB_STRING_WP_ENABLE" int(1) default 0," \
    DB_STRING_WP_INTERVAL" int(4) default 0," \
    DB_STRING_WP_SERVER_IP" varchar(128) default ''," \
    DB_STRING_WP_SERVER_PORT" int(4) default 0," \
    DB_STRING_WP_SCAN_TYPE" int(1) default 0," \
    DB_STRING_WP_CODE" int(1) default 0," \
    DB_STRING_WP_PROTO" int(1) default 0," \
    DB_STRING_EF_ENABLE" int(1) default 0," \
    DB_STRING_EF_INTERVAL" int(4) default 0," \
    DB_STRING_EF_SERVER_IP" varchar(128) default ''," \
    DB_STRING_EF_SERVER_PORT" int(4) default 0," \
    DB_STRING_EF_SCAN_TYPE" int(4) default 0," \
    DB_STRING_EF_CODE" int(1) default 0," \
    DB_STRING_EF_PROTO" int(1) default 0," \
    DB_STRING_WE_AD_INTERVAL" int(4) default 0," \
    DB_STRING_WE_CHANNEL_2G" int(4) default 0," \
    DB_STRING_WE_CHANNEL_5G" int(4) default 0," \
    DB_STRING_WE_AD_RSSI" int(4) default 0," \
    "primary key('"DB_STRING_WIRELESS_POSITION_NAME"')" \
    ")"

enum
{
    DB_INDEX_WP_LIST_NAME,
    DB_INDEX_WP_ENABLE,
    DB_INDEX_WP_INTERVAL,
    DB_INDEX_WP_SERVER_IP,
    DB_INDEX_WP_SERVER_PORT,
    DB_INDEX_WP_SCAN_TYPE,
    DB_INDEX_WP_CODE,
    DB_INDEX_WP_PROTO,
    DB_INDEX_EF_ENABLE,
    DB_INDEX_EF_INTERVAL,
    DB_INDEX_EF_SERVER_IP,
    DB_INDEX_EF_SERVER_PORT,
    DB_INDEX_EF_SCAN_TYPE,
    DB_INDEX_EF_CODE,
    DB_INDEX_EF_PROTO,
    DB_INDEX_WE_AD_INTERVAL,
    DB_INDEX_WE_CHANNEL_2G,
    DB_INDEX_WE_CHANNEL_5G,
    DB_INDEX_WE_AD_RSSI,
};


struct ap_dev {
    struct client *cl;
    char hw_addr[INFO_LEN32];
    char serial_number[INFO_LEN32];
    char position[INFO_LEN128];
    char wan_ip[INFO_LEN32];
    char lan_ip[INFO_LEN32];
    int state;
    char group_name[INFO_LEN128];
    char dev_model[INFO_LEN32];
    char hardware_version[INFO_LEN64];
    char software_version[INFO_LEN64];
    char company_id[INFO_LEN128];
    char dst_white_list[INFO_LEN32];
    char dst_black_list[INFO_LEN32];
    char src_white_list[INFO_LEN32];
    int upgrade_cnt;
    long long online_time;
    long long leave_time;
    long long create_time;

    struct uloop_timeout init_timeout; // 初始化定时器，收到第一个AP发送的echo报文后，启动定时器发送初始化报文
    int echo_cnt;                      // 用来确定是否第一次接收到该AP的 echo报文 然后进行初始化操作

    int max_radios;
    struct {
        int radio_id;
        int radio_type;
    }radios[MAX_RADIOS];
    int echo_timeout_cnt;
};

typedef map<string, struct ap_dev *> aplist;
extern aplist aps;

struct ap_dev *find_ap_dev(string &mac);
int add_ap_dev(struct ap_dev *ap);
int del_ap_dev(struct ap_dev *ap);
int del_ap_dev(string &mac);

int ap_dev_set_hw_addr(struct ap_dev *ap, const char *hw_addr);
int ap_dev_set_serial_number(struct ap_dev *ap, const char *serial_number);
int ap_dev_set_position(struct ap_dev *ap, const char *position);
int ap_dev_set_wan_ip(struct ap_dev *ap, const char *wan_ip);
int ap_dev_set_lan_ip(struct ap_dev *ap, const char *lan_ip);
int ap_dev_set_state(struct ap_dev *ap, int state);
int ap_dev_set_group_name(struct ap_dev *ap, const char *group_name);
int ap_dev_set_dev_model(struct ap_dev *ap, const char *dev_model);
int ap_dev_set_hardware_version(struct ap_dev *ap, const char *hardware_version);
int ap_dev_set_software_version(struct ap_dev *ap, const char *software_version);
int ap_dev_set_company_id(struct ap_dev *ap, const char *company_id);
int ap_dev_set_dst_white_list(struct ap_dev *ap, const char *dst_white_list);
int ap_dev_set_dst_black_list(struct ap_dev *ap, const char *dst_black_list);
int ap_dev_set_src_white_list(struct ap_dev *ap, const char *src_white_list);
int ap_dev_set_upgrade_cnt(struct ap_dev *ap, int upgrade_cnt);
int ap_dev_set_online_time(struct ap_dev *ap, long long online_time);
int ap_dev_set_leave_time(struct ap_dev *ap, long long leave_time);
int ap_dev_set_create_time(struct ap_dev *ap, long long create_time);

int ap_dev_set_radios(struct ap_dev *ap, kvlist &kv);

int ap_dev_load_from_db(struct ap_dev *ap, std::vector<std::string> &row);

string ap_list_str();
int ap_list_run_count();
int ap_list_all_count();

#endif
