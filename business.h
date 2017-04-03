#ifndef _BUSINESS_H_
#define _BUSINESS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "buffer.h"

enum
{
    CAPWAP_BUSINESS_DISCOVERY,         // AP发现业务处理类型
    CAPWAP_BUSINESS_JOIN,              // AP加入业务处理类型
    CAPWAP_BUSINESS_CONFIGURE,         // AP请求配置业务处理
    CAPWAP_BUSINESS_DATA_CHECK,        // AP 数据检查状态业务处理
    CAPWAP_BUSINESS_DATA_TRANSFER,     // AP 数据上传业务处理
    CAPWAP_BUSINESS_ECHO,              // 心跳业务处理
    CAPWAP_BUSINESS_INIT_CONFIG,       // AP 上线初始化业务
    CAPWAP_BUSINESS_AP_CONFIG_RSP,     // AP_CONFIG_RSP报文处理业务
    CAPWAP_BUSINESS_CONFIG_UPDATE_RSP, // config update rsp报文片处理
    CAPWAP_BUSINESS_WLAN_CONFIG_RSP,   // WLAN CONFIG RSP报文处理业务
    CAPWAP_BUSINESS_WTP_EVENT_REQ,     // wtp request业务处理（包括许多小业务的处理，终端上线、下线、多终端上线、流量上报等等）
    CAPWAP_BUSINESS_AP_INFO_REQ,       // ap上报ac地址，cloud地址以及运行模式
    CAPWAP_BUSINESS_AP_LEAVE,          // AP 离线业务处理
    CAPWAP_BUSINESS_NOTIFY_STATUS,     // 下发终端的认证状态
    CAPWAP_BUSINESS_MAX,               // 业务处理最大值
};
enum
{
    BUSINESS_SUCCESS,
    BUSINESS_FAIL,
};

#define AUTH_TYPE_FREEAUTH   0x00
#define AUTH_TYPE_PORTALAUTH 0x01
#define AUTH_TYPE_WXAUTH     0x02
#define AUTH_TYPE_WXLWIFI    0x04

#define BUSINESS_TIMEOUT_INTERVAL 1  // 业务处理存在1ms的间隔
extern struct uloop_timeout business_timeout;

class CCapwapHeader;
class CBusiness
{
private:
    uint32_t business_type;
    string src_info;
    CCapwapHeader *src_packet;
    struct ap_dev *ap;
public:

    CBusiness();
    ~CBusiness();

    int Process();
    int business_discovery_process();
    int business_join_process();
    int business_configure_process();
    int business_data_check_process();
    int business_data_transfer_process();
    int business_echo_process();
    int business_init_config_process();
    int business_init_ap_config();
    int business_init_wlan_config();
    int business_ap_config_rsp();
    int business_wlan_config_rsp();
    int business_wtp_event_req();
    int business_add_station();
    int business_multi_add_station();
    int business_del_station();
    int business_ap_info_req();
    int business_ap_leave();
    int business_notify_status();
    int business_multi_process_station();
    int business_config_update_rsp();
    int business_user_info();

    void set_business_type(int type)
    {
        business_type = type;
    }
    void set_business_string(string &src)
    {
        src_info.assign(src);
    }
    void set_business_packet(CCapwapHeader *cwheader)
    {
        src_packet = cwheader;
    }
    void set_business_ap_dev(struct ap_dev *ap_dev)
    {
        ap = ap_dev;
    }
    uint32_t type()
    {
        return business_type;
    }
    string str()
    {
        return src_info;
    }
    CCapwapHeader *packet()
    {
        return src_packet;
    }
    struct ap_dev* ap_dev()
    {
        return ap;
    }
};

CBusiness *create_business();

void business_timeout_cb(struct uloop_timeout *timeout);

int add_business(CBusiness *pbusiness);
int flush_business();
int flush_business(struct ap_dev *ap);

#endif
