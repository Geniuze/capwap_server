#ifndef _DCAC_UBUS_H_
#define _DCAC_UBUS_H_

#include "kvlist.h"

#define UBUS_OBJECT_NAME "capwap_server"
#define UBUS_OBJECT_AUTH_SERVER "ibox.authd"

#define UBUS_STRING_NOTIFY_STATUS "notify_status"

#define UBUS_STRING_MAC "mac"
#define UBUS_STRING_IP  "ip"
#define UBUS_STRING_SSID "ssid"
#define UBUS_STRING_APMAC "apmac"
#define UBUS_STRING_RADIO "radio"
#define UBUS_STRING_SOURCE "source"

#define UBUS_STRING_ID "id"
#define UBUS_STRING_STATE "status"
#define UBUS_STRING_PRIORITY "priority"
#define UBUS_STRING_ACCOUNT "account"
#define UBUS_STRING_AUTHMODE "auth_mode"

extern struct ubus_context *ubus_ctx;
extern struct blob_buf b;

int server_run();
int ubus_add_station(kvlist &kv);
int ubus_ack(kvlist &kv);
int ubus_del_station(kvlist &kv);

const char * ubus_get_string(struct blob_attr *attr, const char * def = "");
uint32_t ubus_get_u32(struct blob_attr *attr, uint32_t def = 0);

#endif
