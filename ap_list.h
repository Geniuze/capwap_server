#ifndef _AP_LIST_H_
#define _AP_LIST_H_

extern "C" {
#include "libubus.h"
#include "libubox/blobmsg.h"
#include "libubox/blobmsg_json.h"
#include "libubox/ustream.h"
}

#include "dbi.h"
#include "kvlist.h"
#include "server.h"

#define HW_ADDR_LEN 6

#define INFO_LEN32 32
#define INFO_LEN64 64
#define INFO_LEN128 128
#define MAX_RADIOS 8

#define CAPWAP_SERVER_DB "/etc/config/cwserver.db"

#define RADIO_TYPE_11B 0x01
#define RADIO_TYPE_11A 0x02
#define RADIO_TYPE_11G 0x04
#define RADIO_TYPE_11N 0x08

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

struct ap_dev *find_ap_dev(string mac);
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

bool isOnline(struct ap_dev *ap);

#endif
