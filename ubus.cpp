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

#define UBUS_OBJECT_NAME "capwap_server"

struct ubus_context *ubus_ctx;
struct blob_buf b;

enum {
    AP_LIST_COUNTPERPAGE,
    AP_LIST_PAGE,
    AP_LIST_MAX,
};

static const struct blobmsg_policy ap_list_policy[] = {
	{ "start", BLOBMSG_TYPE_INT32 },
	{ "page",  BLOBMSG_TYPE_INT32 },
};


static int dcac_ap_list(struct ubus_context *ctx, struct ubus_object *obj,
                        struct ubus_request_data *req, const char *method, struct blob_attr *msg)
{
    int run_count = ap_list_run_count();
    int all_count = ap_list_all_count();

    blob_buf_init(&b, 0);
	blobmsg_add_u32(&b, "RUN", run_count);
	blobmsg_add_u32(&b, "NOT RUN", all_count - run_count);
	blobmsg_add_u32(&b, "ALL", all_count);
	ubus_send_reply(ctx, req, b.head);

    blob_buf_free(&b);
    return 0;
}

#define UBUS_METHOD_COMPAT(name, func, policy, policy_size) \
    {name, func, 0/*mask*/, policy, policy_size}

static struct ubus_method dcac_methods[] = {
    UBUS_METHOD_COMPAT("ap_list", dcac_ap_list, ap_list_policy, ARRAY_SIZE(ap_list_policy)),
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
