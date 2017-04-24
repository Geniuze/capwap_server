#ifndef _CAPWAP_SERVER_SERVER_H_
#define _CAPWAP_SERVER_SERVER_H_

#include <list>

extern "C" {
#include "libubus.h"
#include "libubox/blobmsg.h"
#include "libubox/blobmsg_json.h"
#include "libubox/ustream.h"
}

#define LISTEN_PORT 5246
#define LIMIT_CLIENTS 20

#define CLIENT_INIT_TIMEOUT 10 * 1000

enum {
    LISTENER_PROTO_TCP,
    LISTENER_PROTO_UDP,
};

typedef size_t (*write_cb_t) (struct client *cl, char *buf, size_t len);
typedef int (*close_client_cb_t) (struct client *cl);

struct listener {
    struct uloop_fd fd;
    int socket;
    int n_clients;
    struct sockaddr_in addr;
    int proto; //tcp udp
    int blocked; //阻塞
};

struct ap_dev;
struct client {
    struct ustream *us;
    struct ustream_fd sfd;       //tcp 终端使用
    struct uloop_fd fd;          //udp 终端使用
 	struct uloop_timeout timeout;
    int packets;
    //capwap_state state;
    struct sockaddr_in peer_addr;
    struct listener *listener;
    struct ap_dev *ap;
    write_cb_t write_cb;
    close_client_cb_t close_client_cb;
};

struct tcp_header {
    unsigned int type;
    unsigned int len;
};

extern list<struct listener *> listeners;
extern list<struct client *> clients;

int udp_accept_client(int fd, struct listener *l);
int tcp_accept_client(int fd, struct listener *l);
int udp_close_client(struct client *cl);
int tcp_close_client(struct client *cl);
void tcp_listener_cb(struct uloop_fd *fd, unsigned int events);
void udp_listener_cb(struct uloop_fd *fd, unsigned int events);
int init_tcp_server();
int init_udp_server();
int init_server();

#define IDENTIFY 0xa55a9cbb

#endif
