#ifndef _CAPWAP_SERVER_CAPWAP_H_
#define _CAPWAP_SERVER_CAPWAP_H_
#include "server.h"
#include "ap_list.h"

class CBuffer;
typedef enum {
    CAPWAP_STATE_START,
    CAPWAP_STATE_IDLE,
    CAPWAP_STATE_SULKING,
    CAPWAP_STATE_DISCOVERY,
    CAPWAP_STATE_JOIN,
    CAPWAP_STATE_IMAGE_DATA,
    CAPWAP_STATE_CONFIGURE,
    CAPWAP_STATE_DATA_CHECK,
    CAPWAP_STATE_RUN,
    CAPWAP_STATE_RESET,
    CAPWAP_STATE_QUIT,

    CAPWAP_STATE_INVALID,
    CAPWAP_STATE_MAX,
}capwap_state_t;

typedef enum {
    CAPWAP_MESSAGE_SAVE,
    CAPWAP_MESSAGE_DONE,
    CAPWAP_MESSAGE_INVALID,
}capwap_message_process_t;

#define sec *1000

enum {
    CAPWAP_DISCOVERY_TIMEOUT = 10 sec,
    CAPWAP_JOIN_TIMEOUT = 10 sec,
    CAPWAP_IMAGE_DATA_TIMEOUT = 600 sec,
    CAPWAP_CONFIGURE_TIMEOUT = 30 sec,
    CAPWAP_DATA_CHECK_TIMEOUT = 10 sec,
    CAPWAP_DATA_TRANSFER_TIMEOUT = 30 sec,
    CAPWAP_RESET_TIMEOUT = 30 sec,

    CAPWAP_INIT_CONFIG_TIMEOUT = 1 sec, // 该定时器为接收到echo后多长时间后发送初始化报文
};

extern const char *capwap_state_string[CAPWAP_STATE_MAX];

int capwap_read_cb(struct client *cl, char *buf, size_t len);
int capwap_state_change_cb(struct client *cl);
int capwap_state_timeout(struct client *cl);

int capwap_state_start(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_idle(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_sulking(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_discovery(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_join(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_image_data(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_configure(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_data_check(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_run(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_reset(struct ap_dev *ap, CBuffer &buffer);
int capwap_state_quit(struct ap_dev *ap, CBuffer &buffer);

#endif
