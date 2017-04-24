#ifndef _DCAC_UBUS_H_
#define _DCAC_UBUS_H_

#include "kvlist.h"

#define UBUS_OBJECT_NAME "capwap_server"
#define UBUS_OBJECT_AUTH_SERVER "ibox.authd"

#define UBUS_STRING_AP_LIST "ap_list"
#define UBUS_STRING_NOTIFY_STATUS "notify_status"
#define UBUS_STRING_OPT "options"

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
#define UBUS_STRING_DEVMODEL "dev_model"
#define UBUS_STRING_LANIP "lan_ip"
#define UBUS_STRING_WANIP "wan_ip"
#define UBUS_STRING_SOFTVER "software_version"
#define UBUS_STRING_GROUP "group_name"
#define UBUS_STRING_POSITION "position"
#define UBUS_STRING_UPRATE "up_rate"
#define UBUS_STRING_DOWNRATE "down_rate"
#define UBUS_STRING_ONLINE "online_time"
#define UBUS_STRING_LEAVE "leave_time"

#define UBUS_STRING_ID "id"
#define UBUS_STRING_STATE "status"
#define UBUS_STRING_PRIORITY "priority"
#define UBUS_STRING_ACCOUNT "account"
#define UBUS_STRING_AUTHMODE "auth_mode"

#define UBUS_STRING_COUNT "count"
#define UBUS_STRING_DATA  "data"

#define UBUS_STRING_PAGE_NUMBER "page_number"
#define UBUS_STRING_PAGE_COUNT "page_count"
#define UBUS_STRING_ORDER_BY "order_by"
#define UBUS_STRING_ORDER "order"
#define UBUS_STRING_COND_KEY "cond_key"
#define UBUS_STRING_COND_KEY_VALUE "cond_key_value"
#define UBUS_STRING_COND_GROUP "cond_group"
#define UBUS_STRING_COND_GROUP_NAME "cond_group_name"
#define UBUS_STRING_COND_DST_WHITE "cond_dst_white"
#define UBUS_STRING_COND_DST_WHITE_VALUE "cond_dst_white_value"
#define UBUS_STRING_COND_STATE "cond_state"
#define UBUS_STRING_COND_STATE_VALUE "cond_state_value"
#define UBUS_STRING_COND_UPGRADE "cond_upgrade"
#define UBUS_STRING_COND_UPGRADE_VALUE "cond_upgrade_value"

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
