#ifndef _DCAC_UBUS_H_
#define _DCAC_UBUS_H_

#include "kvlist.h"

#define UBUS_OBJECT_NAME "capwap_server"
#define UBUS_OBJECT_AUTH_SERVER "ibox.authd"

#define UBUS_STRING_NOTIFY_STATUS "notify_status"
#define UBUS_STRING_OPT "options"
#define UBUS_STRING_ECHO_CONFIG CAPWAP_ECHO_CONFIG
#define UBUS_STRING_TS_CONFIG CAPWAP_TS_CONFIG
#define UBUS_STRING_UPGRADE_CONFIG CAPWAP_UPGRADE_CONFIG
#define UBUS_STRING_UPGRADE_SERVER CAPWAP_UPGRADE_SERVER

#define UBUS_STRING_TYPE "type"   // add del modify query
#define UBUS_STRING_TABLE "table"
#define UBUS_STRING_KEY "key"     // 修改或删除时用来定位某一条或多条策略
#define UBUS_STRING_COLS "cols"   // 添加、修改时要赋值的列,查询时指定需要的列为空则为所有列
#define UBUS_STRING_VALS "vals"   // 添加、修改时要赋的值
#define UBUS_STRING_QUERY_OPTION "query_option"   // 查询时指定页数及一页显示最大个数

#define UBUS_STRING_MAC "mac"
#define UBUS_STRING_IP  "ip"
#define UBUS_STRING_SSID "ssid"
#define UBUS_STRING_APMAC "apmac"
#define UBUS_STRING_RADIO "radio"
#define UBUS_STRING_SOURCE "source"
#define UBUS_STRING_NUM "num"

#define UBUS_STRING_ID "id"
#define UBUS_STRING_STATE "status"
#define UBUS_STRING_PRIORITY "priority"
#define UBUS_STRING_ACCOUNT "account"
#define UBUS_STRING_AUTHMODE "auth_mode"

#define UBUS_STRING_ECHO_INTERVAL STRING_ECHO_INTERVAL
#define UBUS_STRING_ECHO_COUNT STRING_ECHO_COUNT
#define UBUS_STRING_TS_ENABLE STRING_TRAFFIC_STATICS_ENABLE
#define UBUS_STRING_TS_INTERVAL STRING_TRAFFIC_STATICS_INTERVAL
#define UBUS_STRING_UPGRADE_ENABLE CAPWAP_UPGRADE_CONFIG_ENABLE

#define UBUS_STRING_UPGRADE_DOWNLOAD_TYPE CAPWAP_UPGRADE_SERVER_TYPE
#define UBUS_STRING_UPGRADE_FILE_SERVER CAPWAP_UPGRADE_SERVER_ADDR
#define UBUS_STRING_UPGRADE_FTP_USER_NAME CAPWAP_UPGRADE_SERVER_USERNAME
#define UBUS_STRING_UPGRADE_FTP_PASSWORD CAPWAP_UPGRADE_SERVER_PASSWORD
#define UBUS_STRING_UPGRADE_FTP_PATH CAPWAP_UPGRADE_SERVER_PATH

extern struct ubus_context *ubus_ctx;
extern struct blob_buf b;

int server_run();
int ubus_add_station(kvlist &kv);
int ubus_ack(kvlist &kv);
int ubus_del_station(kvlist &kv);
int ubus_multi_add_station(kvlist &kv);

const char * ubus_get_string(struct blob_attr *attr, const char * def = "");
uint32_t ubus_get_u32(struct blob_attr *attr, uint32_t def = 0);

#endif
