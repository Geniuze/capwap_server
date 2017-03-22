#ifndef _BUSINESS_H_
#define _BUSINESS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum
{
    CAPWAP_BUSINESS_DISCOVERY, // AP上线业务处理类型
    CAPWAP_BUSINESS_JOIN, // AP加入业务处理类型
    CAPWAP_BUSINESS_CONFIGURE, //AP请求配置业务处理
    CAPWAP_BUSINESS_DATA_CHECK, // AP 数据检查状态业务处理
    CAPWAP_BUSINESS_DATA_TRANSFER, // AP 数据上传业务处理
    CAPWAP_BUSINESS_MAX, //业务处理最大值
};
enum
{
    BUSINESS_SUCCESS,
    BUSINESS_FAIL,
};

class CBusiness
{
private:
    uint32_t business_type;
    string src_info;
public:
    CBusiness()
    {
        business_type = CAPWAP_BUSINESS_MAX;
        src_info = "";
    }

    CBusiness(uint32_t type, string &src)
    {
        business_type = type;
        src_info.assign(src);
    }
    ~CBusiness(){}

    int Process(struct ap_dev *ap);
    int business_discovery_process(struct ap_dev *ap);
    int business_join_process(struct ap_dev *ap);
    int business_configure_process(struct ap_dev *ap);
    int business_data_check_process(struct ap_dev *ap);
    int business_data_transfer_process(struct ap_dev *ap);

    void set_business_type(int type)
    {
        business_type = type;
    }
    void set_business_string(string &src)
    {
        src_info.assign(src);
    }
};

#endif
