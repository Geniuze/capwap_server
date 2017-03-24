#include <iostream>
using namespace std;

#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

extern "C" {
#include "libubus.h"
#include "libubox/blobmsg.h"
#include "libubox/blobmsg_json.h"
#include "libubox/ustream.h"
}

#include "log.h"
#include "server.h"
#include "capwap_state.h"
#include "buffer.h"

int n_clients = 0;
list<struct listener *> listeners;
list<struct client *> clients;

static void client_timeout(struct uloop_timeout *timeout)
{
    struct client *cl = container_of(timeout, struct client, timeout);

    capwap_state_timeout(cl);
}

static void client_timeout_init(struct client *cl)
{
    cl->timeout.cb = client_timeout;
    uloop_timeout_set(&cl->timeout, CLIENT_INIT_TIMEOUT);
}
static void client_udp_read_cb(struct uloop_fd *fd, unsigned int events)
{
    struct client *cl = container_of(fd, struct client, fd);
    int buf_size = 1024;
    int ret = 0;
    char *buf = (char *)malloc(buf_size);
    bzero(buf, buf_size);

    do {
        ret = recv(fd->fd, buf, buf_size, MSG_PEEK);
        if (ret == buf_size) {
            buf_size *= 2;
            buf = (char*)realloc(buf, buf_size);
            bzero(buf, buf_size);
        }
        else break;
    }while(1);

    ret = recv(fd->fd, buf, buf_size, 0);
    dlog(LOG_DEBUG, "%s.%d read %d buf_size %d", __func__, __LINE__, ret, buf_size);
    capwap_read_cb(cl, buf, ret);

    SAFE_FREE(buf);

}

static size_t client_ustream_write(struct client *cl, char *buf, size_t len)
{
    char *newbuf = (char *)malloc(len+8);
    *((unsigned int *)newbuf) = htonl(IDENTIFY);
    *((unsigned int *)(newbuf+4)) = htonl(len);
    memcpy(newbuf+8, buf, len);

    ustream_write(cl->us, newbuf, len+8, true);

    SAFE_FREE(newbuf);
    return 0;
}
static size_t client_udp_write(struct client *cl, char *buf, size_t len)
{
    return write(cl->fd.fd, buf, len);
}

static void client_ustream_read_cb(struct ustream *us, int bytes)
{
    struct client *cl = container_of(us, struct client, sfd.stream);
    int len = 0;
    char *buf = NULL;
    uint32_t Identify, Length;
    do {
        buf = ustream_get_read_buf(us, &len);

        if ( len < 8 ) break;

        Identify = ntohl(*((unsigned int *)buf));
        Length = ntohl(*((unsigned int *)(buf+4)));

        if ( Identify != IDENTIFY ) {
            ustream_consume(us, len);
            break;
        }

        if ( Length+8 > len ) break;

        capwap_read_cb(cl, buf+8, Length);
        ustream_consume(us, Length+8);
    }while(buf);

}
static void client_ustream_write_cb(struct ustream *us, int bytes)
{
    dlog(LOG_DEBUG,"write : %d", bytes);
}
static void client_ustream_state_cb(struct ustream *us)
{
    struct client *cl = container_of(us, struct client, sfd.stream);
	// struct ustream *s = cl->us;

    /*
	if (!s->write_error && cl->state != CLIENT_STATE_CLEANUP) {
		if (cl->state == CLIENT_STATE_DATA)
			return;

		if (!s->eof || s->w.data_bytes)
			return;
	}
    */

	tcp_close_client(cl);
}

int tcp_accept_client(int fd, struct listener *l)
{
    struct client *cl = NULL;
    struct sockaddr_in cliaddr;
    socklen_t clen = sizeof(cliaddr);
    int csock;

    cl = (struct client *)malloc(sizeof(struct client));
    bzero(cl, sizeof(*cl));

    csock = accept(fd, (struct sockaddr *)&cliaddr, &clen);
    if (0 > csock) {
        dlog(LOG_DEBUG,"[%s.%d] accept error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    {
        char ipaddr[32] = {0};
        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipaddr, sizeof(ipaddr));
        dlog(LOG_DEBUG, "%s.%d new client from %s port %d",
               __func__, __LINE__, ipaddr, ntohs(cliaddr.sin_port));
    }

    l->n_clients ++;
    n_clients++;

    memcpy(&cl->peer_addr, &cliaddr, sizeof(cliaddr));
    cl->listener = l;
    cl->us = &cl->sfd.stream;
    cl->us->notify_read = client_ustream_read_cb;
    cl->us->notify_write = client_ustream_write_cb;
    cl->us->notify_state = client_ustream_state_cb;
    cl->us->string_data = false;
    cl->write_cb = client_ustream_write;
    cl->close_client_cb = tcp_close_client;
    client_timeout_init(cl);

    ustream_fd_init(&cl->sfd, csock);

    clients.push_back(cl);

    return 0;
}

int udp_accept_client(int fd, struct listener *l)
{
    int new_sd = -1;
    int ret = 0;
    int buf_size = 1024;
    int reuse = 1;
    char peek_buf[2] = {0};
    char *buf = NULL;
    struct sockaddr_in peer_addr;
    socklen_t cli_len = sizeof(peer_addr);


    ret = recvfrom(fd, peek_buf, 2, MSG_PEEK, (struct sockaddr *)&peer_addr, &cli_len);
    if (ret < 0)
    {
        dlog(LOG_ERR, "%s.%d recvfrom error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    if ((new_sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        dlog(LOG_ERR, "%s.%d socket error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }
    ret = setsockopt(new_sd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse));
    if (ret) {
        dlog(LOG_ERR, "%s.%d setsockopt error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    ret = bind(new_sd, (struct sockaddr *) &l->addr, sizeof(struct sockaddr_in));
    if (ret){
        dlog(LOG_ERR, "%s.%d bind error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    peer_addr.sin_family = AF_INET;

    if (connect(new_sd, (struct sockaddr *) &peer_addr, sizeof(struct sockaddr_in)) == -1) {
        dlog(LOG_ERR, "%s.%d connect error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    struct client *cl = (struct client *)malloc(sizeof(struct client));
    if (NULL == cl)
    {
        dlog(LOG_ERR, "%s.%d malloc error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    bzero(cl, sizeof(struct client));

    l->n_clients++;
    n_clients++;

    memcpy(&cl->peer_addr, &peer_addr, sizeof(struct sockaddr_in));
    cl->listener = l;
    cl->fd.fd = new_sd;
    cl->fd.cb = client_udp_read_cb;
    cl->write_cb = client_udp_write;
    cl->close_client_cb = udp_close_client;
    client_timeout_init(cl);

    uloop_fd_add(&cl->fd, ULOOP_READ);

    clients.push_back(cl);

    // 从原套接字中读取第一个报文进行处理
    buf = (char *)malloc(buf_size);
    bzero(buf, buf_size);

    do {
        ret = recvfrom(fd, buf, buf_size, MSG_PEEK, (struct sockaddr *)&peer_addr, &cli_len);
        if (ret == buf_size) {
            buf_size *= 2;
            buf = (char *)realloc(buf, buf_size);
            bzero(buf, buf_size);
        }
        else break;
    }while(1);

    ret = recvfrom(fd, buf, buf_size, 0, (struct sockaddr *)&peer_addr, &cli_len);
    dlog(LOG_DEBUG, "%s.%d read %d buf_size %d ", __func__, __LINE__,  ret, buf_size);
    capwap_read_cb(cl, buf, ret);

    {
        char ipaddr[32] = {0};
        inet_ntop(AF_INET, &peer_addr.sin_addr.s_addr, ipaddr, sizeof(ipaddr));
        dlog(LOG_DEBUG, "%s.%d new client from %s port %d",
               __func__, __LINE__, ipaddr, ntohs(peer_addr.sin_port));
    }

    SAFE_FREE(buf);
    return 0;
}
int tcp_close_client(struct client *cl)
{

    dlog(LOG_DEBUG, "%s.%d", __func__, __LINE__);

    capwap_state_change_cb(cl);

    cl->listener->n_clients--;
    n_clients --;
    ustream_free(&cl->sfd.stream);
    close(cl->sfd.fd.fd);
    clients.remove(cl);
    SAFE_FREE(cl);

    //uh_unblock_listeners(); //通知listener终端数减少，重新监听
    return 0;
}
int udp_close_client(struct client *cl)
{
    dlog(LOG_DEBUG, "%s.%d", __func__, __LINE__);

    capwap_state_change_cb(cl);

    cl->listener->n_clients--;
    n_clients --;
    uloop_fd_delete(&cl->fd);
    close(cl->fd.fd);
    clients.remove(cl);
    SAFE_FREE(cl);

    return 0;
}
void tcp_listener_cb(struct uloop_fd *fd, unsigned int events)
{
	struct listener *l = container_of(fd, struct listener, fd);

	while (1) {
		if (!tcp_accept_client(fd->fd, l))
			break;
	}

    /* if (conf.max_connections && n_clients >= conf.max_connections)
		uh_block_listener(l);
    */
}
void udp_listener_cb(struct uloop_fd *fd, unsigned int events)
{
	struct listener *l = container_of(fd, struct listener, fd);

    while (1) {
        if (!udp_accept_client(fd->fd, l))
            break;
    }
}

/**
   初始化TCP上线套接字
   添加listener到监听列表
 */
int init_tcp_server()
{
    int yes = 1;
    struct sockaddr_in serv;
    struct listener *l = NULL;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if(0 > sock)
    {
        dlog(LOG_DEBUG,"[%s.%d] socket error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
        dlog(LOG_DEBUG,"[%s.%d] setsockopt error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(LISTEN_PORT);

    if (bind(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        dlog(LOG_DEBUG,"[%s.%d] bind error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }
    if (listen(sock, LIMIT_CLIENTS) < 0) {
        dlog(LOG_DEBUG,"[%s.%d] listen error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    l = (struct listener *)malloc(sizeof(struct listener));
    if (NULL == l) {
        dlog(LOG_DEBUG,"[%s.%d] malloc error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    bzero(l, sizeof(*l));

    memcpy(&l->addr, &serv, sizeof(struct sockaddr_in));
    l->fd.fd = sock;
    l->fd.cb = tcp_listener_cb;
    l->proto = LISTENER_PROTO_TCP;

    uloop_fd_add(&l->fd, ULOOP_READ);

    listeners.push_back(l);

    return 0;
}
int init_udp_server()
{
    struct sockaddr_in serv;
    struct listener *l = NULL;
    int yes = 1;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        dlog(LOG_ERR, "%s.%d socket error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
        dlog(LOG_DEBUG,"[%s.%d] setsockopt error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    /*
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes))) {
        dlog(LOG_DEBUG,"[%s.%d] setsockopt error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }
    */

    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(LISTEN_PORT);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0)
    {
        dlog(LOG_ERR, "%s.%d bind error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    l = (struct listener *)malloc(sizeof(struct listener));
    if (NULL == l)
    {
        dlog(LOG_ERR, "%s.%d malloc error %s", __func__, __LINE__, strerror(errno));
        return -1;
    }

    bzero(l, sizeof(struct listener));

    memcpy(&l->addr, &serv, sizeof(struct sockaddr_in));
    l->fd.fd = sock;
    l->fd.cb = udp_listener_cb;
    l->proto = LISTENER_PROTO_UDP;

    uloop_fd_add(&l->fd, ULOOP_READ);

    listeners.push_back(l);

    return 0;
}
/**
   AC 服务初始化
   --tcp服务监听
   --udp服务监听
   --添加到服务列表中
   --添加到uloop列表
   --设置回调函数
 */
int init_server()
{
    init_tcp_server();
    init_udp_server();

    return 0;
}
