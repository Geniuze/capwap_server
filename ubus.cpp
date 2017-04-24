#include <iostream>
using namespace std;

#include <list>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

extern "C" {
#include "libubus.h"
#include "libubox/blobmsg.h"
#include "libubox/blobmsg_json.h"
#include "libubox/ustream.h"
}

#include "log.h"
#include "server.h"
#include "ap_list.h"
#include "ubus.h"
#include "dstring.h"
#include "business.h"
#include "opt.h"

struct ubus_context *ubus_ctx;
struct blob_buf b;
struct ubus_request req;

const char * ubus_get_string(struct blob_attr *attr, const char * def)
{
	const char *value = NULL;
	if (attr && (blobmsg_type(attr) == BLOBMSG_TYPE_STRING))
		value = blobmsg_get_string(attr);

	return value ? value : def;
}

uint32_t ubus_get_u32(struct blob_attr *attr, uint32_t def)
{
	if (attr && (blobmsg_type(attr) == BLOBMSG_TYPE_INT32))
		return blobmsg_get_u32(attr);

	return def;
}

static int ubus_send_auth(const char * method, blob_buf *b)
{
    uint32_t id;
    int ret = ubus_lookup_id(ubus_ctx, UBUS_OBJECT_AUTH_SERVER, &id);
    if (0 != ret)
    {
        dlog(LOG_ERR, "%s.%d ubus_lookup_id error return %d", __FUNC__, __LINE__, ret);
        return ret;
    }
    bzero(&req, sizeof(req));
    return ubus_invoke_async(ubus_ctx, id, method, b->head, &req);
}

enum
{
    NOTIFY_STATUS_ID,
    NOTIFY_STATUS,
    NOTIFY_STATUS_MAC,
    NOTIFY_STATUS_IP,
    NOTIFY_STATUS_AP_MAC,
    NOTIFY_STATUS_PRIORITY,
    NOTIFY_STATUS_ACCOUNT,
    NOTIFY_STATUS_AUTHMODE,
    NOTIFY_STATUS_MAX,
};
static const struct blobmsg_policy notify_status_policy[] =
{
    { UBUS_STRING_ID, BLOBMSG_TYPE_INT32 },
    { UBUS_STRING_STATE, BLOBMSG_TYPE_INT32 },
    { UBUS_STRING_MAC, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_IP, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_APMAC, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_PRIORITY, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_ACCOUNT, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_AUTHMODE, BLOBMSG_TYPE_INT32 },
};

static int dcac_notify_status(struct ubus_context *ctx, struct ubus_object *obj,
                              struct ubus_request_data *req, const char *method, struct blob_attr *msg)
{
    int err;
    struct blob_attr* tb[NOTIFY_STATUS_MAX] = {NULL};
    struct ap_dev *ap = NULL;
    CBusiness *pbusiness = NULL;
    string str;

    err = blobmsg_parse(notify_status_policy, NOTIFY_STATUS_MAX, tb, blob_data(msg), blob_len(msg));
    if (0 != err)
    {
        dlog(LOG_ERR, "%s.%d blobmsg_parse error return %d", __FUNC__, __LINE__, err);
        return -1;
    }

    // 终端相关的业务使用异步业务处理,不需要回应
    pbusiness = create_business();
    if (NULL == pbusiness)
    {
        return -1;
    }

    int id = ubus_get_u32(tb[NOTIFY_STATUS_ID]);
    int state = ubus_get_u32(tb[NOTIFY_STATUS]);
    string mac = ubus_get_string(tb[NOTIFY_STATUS_MAC]);
    string ip  = ubus_get_string(tb[NOTIFY_STATUS_IP]);
    string ap_mac = ubus_get_string(tb[NOTIFY_STATUS_AP_MAC]);
    string priority = ubus_get_string(tb[NOTIFY_STATUS_PRIORITY]);
    string account = ubus_get_string(tb[NOTIFY_STATUS_ACCOUNT]);
    int auth_mode  = ubus_get_u32(tb[NOTIFY_STATUS_AUTHMODE]);

    dlog(LOG_DEBUG, "id:%d,state:%d,mac:%s,ip:%s,ap_mac:%s,priority:%s,account:%s,auth_mode:%d",
         id, state, mac.c_str(), ip.c_str(), ap_mac.c_str(), priority.c_str(), account.c_str(), auth_mode);

    str.append(STRING_PKT_ID"=" + toString(id) + ";");
    str.append(STRING_PKT_TYPE"=" + toString(1) + ";");
    str.append(STRING_MSG_TYPE"=" + toString(0) + ";");
    str.append(STRING_SUB_TYPE"=" + toString(state) + ";");
    str.append(STRING_STA_MAC"=" + mac + ";");
    str.append(STRING_USER_IP"=" + ip  + ";");
    str.append(STRING_AP_MAC"="  + ap_mac + ";");
    str.append(STRING_USER_ROLE"=default;");

    pbusiness->set_business_type(CAPWAP_BUSINESS_NOTIFY_STATUS);
    pbusiness->set_business_string(str);

    ap = find_ap_dev(ap_mac);

    pbusiness->set_business_ap_dev(ap);

    add_business(pbusiness);

    return 0;
}

enum
{
    OPT_TYPE,
    OPT_TABLE,
    OPT_KEY,      // 定位条件
    OPT_COLS,
    OPT_VALS,
    OPT_QUERY_OPTION,
    OPT_MAX,
};

static const struct blobmsg_policy opt_policy[] =
{
    { UBUS_STRING_TYPE, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_TABLE,BLOBMSG_TYPE_STRING },
    { UBUS_STRING_KEY,  BLOBMSG_TYPE_STRING },
    { UBUS_STRING_COLS, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_VALS, BLOBMSG_TYPE_STRING },
    { UBUS_STRING_QUERY_OPTION, BLOBMSG_TYPE_STRING },
};

static int dcac_opt(struct ubus_context *ctx, struct ubus_object *obj,
                    struct ubus_request_data *req, const char *method, struct blob_attr *msg)
{
    int err;
    struct blob_attr *tb[OPT_MAX] = {NULL};
    int result_code = 0;
    string result;

    err = blobmsg_parse(opt_policy, OPT_MAX, tb, blob_data(msg), blob_len(msg));
    if (0 != err)
    {
        dlog(LOG_ERR, "%s.%d blobmsg_parse error return %d", __FUNC__, __LINE__, err);
        return -1;
    }

    struct dcac_option op;
    op.type = ubus_get_string(tb[OPT_TYPE]);
    op.table = ubus_get_string(tb[OPT_TABLE]);
    op.key = ubus_get_string(tb[OPT_KEY]);
    op.cols = ubus_get_string(tb[OPT_COLS]);
    op.vals = ubus_get_string(tb[OPT_VALS]);
    op.query_option = ubus_get_string(tb[OPT_QUERY_OPTION]);

    result_code = opt_process(result, op);

    blob_buf_init(&b, 0);
    blobmsg_add_u32(&b, "result_code", result_code);
    blobmsg_add_string(&b, "result", result.c_str());

    ubus_send_reply(ubus_ctx, req, b.head);

    blob_buf_free(&b);

    return 0;
}


#define UBUS_METHOD_COMPAT(name, func, policy, policy_size) \
    {name, func, 0/*mask*/, policy, policy_size}

static struct ubus_method dcac_methods[] = {
    UBUS_METHOD_COMPAT(UBUS_STRING_NOTIFY_STATUS, dcac_notify_status, notify_status_policy, ARRAY_SIZE(notify_status_policy)),
    UBUS_METHOD_COMPAT(UBUS_STRING_OPT, dcac_opt, opt_policy, ARRAY_SIZE(opt_policy)),
};

static struct ubus_object_type dcac_object_type =
{
    UBUS_OBJECT_NAME,0,dcac_methods,ARRAY_SIZE(dcac_methods),
};

static struct ubus_object dcac_object = 
{
    {{NULL, NULL}, NULL}, /* avl */
    UBUS_OBJECT_NAME,
    0, /* id */
    NULL,  /* path */
    &dcac_object_type, // type
    NULL,
    0,
    dcac_methods,
    ARRAY_SIZE(dcac_methods),
};

int server_run()
{
    uloop_init();

    init_server();

    ubus_ctx = ubus_connect(NULL);
    if (!ubus_ctx)
    {
        dlog(LOG_DEBUG,"ubus_connect error!");
        return 1;
    }
    ubus_add_uloop(ubus_ctx);
    ubus_add_object(ubus_ctx, &dcac_object);

    uloop_run();

    return 0;
}

int ubus_add_station(kvlist &kv)
{
    int ret = 0;

    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, UBUS_STRING_MAC, GetValue(kv, STRING_STA_MAC).c_str());
    blobmsg_add_string(&b, UBUS_STRING_IP, GetValue(kv, STRING_USER_IP).c_str());
    blobmsg_add_string(&b, UBUS_STRING_SSID, GetValue(kv, TO_STR(STRING_WLAN_ESSID)).c_str());
    blobmsg_add_string(&b, UBUS_STRING_APMAC, GetValue(kv, STRING_AP_MAC).c_str());
    blobmsg_add_u32(&b,    UBUS_STRING_RADIO, toInt(GetValue(kv, STRING_RADIO_ID)));
    blobmsg_add_string(&b, UBUS_STRING_SOURCE, UBUS_OBJECT_NAME);

    ret = ubus_send_auth("online", &b);
    blob_buf_free(&b);

    return ret;
}

int ubus_ack(kvlist &kv)
{
    int ret = 0;

    blob_buf_init(&b, 0);
    blobmsg_add_u32(&b, UBUS_STRING_ID, toInt(GetValue(kv, STRING_PKT_ID)));
    blobmsg_add_u32(&b, UBUS_STRING_STATE, toInt(GetValue(kv, STRING_STATE)));
    blobmsg_add_string(&b, UBUS_STRING_MAC, GetValue(kv, STRING_STA_MAC).c_str());

    ret = ubus_send_auth("ack", &b);
    blob_buf_free(&b);
    return ret;
}

int ubus_del_station(kvlist &kv)
{
    int ret = 0;
    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, UBUS_STRING_MAC, GetValue(kv, STRING_STA_MAC).c_str());
    blobmsg_add_string(&b, UBUS_STRING_IP, GetValue(kv, STRING_USER_IP).c_str());
    blobmsg_add_string(&b, UBUS_STRING_SSID, GetValue(kv, TO_STR(STRING_WLAN_ESSID)).c_str());
    blobmsg_add_string(&b, UBUS_STRING_APMAC, GetValue(kv, STRING_AP_MAC).c_str());

    ret = ubus_send_auth("offline", &b);
    blob_buf_free(&b);
    return ret;
}

int ubus_multi_add_station(kvlist &kv)
{
    int ret = 0;

    blob_buf_init(&b, 0);
    blobmsg_add_string(&b, UBUS_STRING_MAC, GetValue(kv, STRING_STA_MAC).c_str());
    blobmsg_add_string(&b, UBUS_STRING_IP, GetValue(kv, STRING_USER_IP).c_str());
    blobmsg_add_string(&b, UBUS_STRING_SSID, GetValue(kv, TO_STR(STRING_WLAN_ESSID)).c_str());
    blobmsg_add_string(&b, UBUS_STRING_RADIO, GetValue(kv, STRING_RADIO_ID).c_str());
    blobmsg_add_string(&b, UBUS_STRING_APMAC, GetValue(kv, STRING_AP_MAC).c_str());
    blobmsg_add_string(&b, UBUS_STRING_SOURCE, UBUS_OBJECT_NAME);
    blobmsg_add_u32(&b, UBUS_STRING_NUM, toInt(GetValue(kv, STRING_USER_COUNT)));

    ret = ubus_send_auth("multi-online", &b);
    blob_buf_free(&b);
    return ret;
}
