#ifndef _BUSINESS_H_
#define _BUSINESS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "buffer.h"
#include "kvlist.h"
#include "config_manager.h"

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
    CAPWAP_BUSINESS_MODIFY,            // 页面下发修改业务
    CAPWAP_BUSINESS_ADD_WLAN,         // 页面下发添加wlan业务
    CAPWAP_BUSINESS_DEL_WLAN,         // 页面下发删除wlan业务

    CAPWAP_BUSINESS_GET_AP_LIST,       // 获取AP列表业务
    CAPWAP_BUSINESS_MAX,               // 业务处理最大值
};
enum
{
    BUSINESS_SUCCESS,
    BUSINESS_FAIL,
};

enum
{
    ORDER_BY_DEVMODEL,
    ORDER_BY_APMAC,
    ORDER_BY_LANIP,
    ORDER_BY_WANIP,
    ORDER_BY_SOFTWARE_VERSION,
    ORDER_BY_GROUP_NAME,
    ORDER_BY_UP_RATE,
    ORDER_BY_DOWN_RATE,
    ORDER_BY_STATE,
    ORDER_BY_ONLINE_TIME,
    ORDER_BY_LEAVE_TIME,
    ORDER_BY_OFFLINE_DURATION,
};

enum
{
    COND_KEY_WANIP,
    COND_KEY_LANIP,
    COND_KEY_APMAC,
    COND_KEY_DEVMD,
    COND_KEY_SOFTW,
    COND_KEY_LOCATION,
};

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
    kvlist ret_info;
    struct ConfigItem ci;

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
    int business_modify();
    int business_add_wlan();
    int business_del_wlan();

    void set_business_type(int type)
    {
        business_type = type;
    }
    void set_business_string(string src)
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
    void set_business_config_item(struct ConfigItem &ci)
    {
        this->ci = ci;
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
    kvlist ret()
    {
        return ret_info;
    }
    struct ConfigItem get_business_config_item()
    {
        return ci;
    }
};

CBusiness *create_business();

void business_timeout_cb(struct uloop_timeout *timeout);

int add_business(CBusiness *pbusiness);
int flush_business();
int flush_business(struct ap_dev *ap);

#endif
