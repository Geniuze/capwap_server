#ifndef _DCAC_UBUS_H_
#define _DCAC_UBUS_H_

extern struct ubus_context *ubus_ctx;
extern struct blob_buf b;

int server_run();

#endif
