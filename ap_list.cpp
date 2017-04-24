#include <iostream>
using namespace std;

#include <string.h>
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

#include "kvlist.h"
#include "ap_list.h"
#include "dstring.h"
#include "log.h"
#include "capwap_state.h"

#define SAFE_ARRCOPY(dst, src) \
    strncpy((dst), (src), sizeof((dst)))

aplist aps;

struct ap_dev *find_ap_dev(string mac)
{
    aplist::iterator it = aps.end();
    if ((it = aps.find(mac)) != aps.end())
    {
        return it->second;
    }
    return NULL;
}
int add_ap_dev(struct ap_dev *ap)
{
    string mac = ap->hw_addr;
    aps[mac] = ap;
    return 0;
}
int del_ap_dev(struct ap_dev *ap)
{
    string mac = ap->hw_addr;
    aps.erase(mac);
    return 0;
}
int del_ap_dev(string &mac)
{
    aps.erase(mac);
    return 0;
}

int ap_dev_set_hw_addr(struct ap_dev *ap, const char *hw_addr)
{
    SAFE_ARRCOPY(ap->hw_addr, hw_addr);
    return 0;
}
int ap_dev_set_serial_number(struct ap_dev *ap, const char *serial_number)
{
    SAFE_ARRCOPY(ap->serial_number, serial_number);
    return 0;
}
int ap_dev_set_position(struct ap_dev *ap, const char *position)
{
    SAFE_ARRCOPY(ap->position, position);
    return 0;
}
int ap_dev_set_wan_ip(struct ap_dev *ap, const char *wan_ip)
{
    SAFE_ARRCOPY(ap->wan_ip, wan_ip);
    return 0;
}
int ap_dev_set_lan_ip(struct ap_dev *ap, const char *lan_ip)
{
    SAFE_ARRCOPY(ap->lan_ip, lan_ip);
    return 0;
}
int ap_dev_set_state(struct ap_dev *ap, int state)
{
    ap->state = state;
    return 0;
}
int ap_dev_set_group_name(struct ap_dev *ap, const char *group_name)
{
    SAFE_ARRCOPY(ap->group_name, group_name);
    return 0;
}
int ap_dev_set_dev_model(struct ap_dev *ap, const char *dev_model)
{
    SAFE_ARRCOPY(ap->dev_model, dev_model);
    return 0;
}
int ap_dev_set_hardware_version(struct ap_dev *ap, const char *hardware_version)
{
    SAFE_ARRCOPY(ap->hardware_version, hardware_version);
    return 0;
}
int ap_dev_set_software_version(struct ap_dev *ap, const char *software_version)
{
    SAFE_ARRCOPY(ap->software_version, software_version);
    return 0;
}
int ap_dev_set_company_id(struct ap_dev *ap, const char *company_id)
{
    SAFE_ARRCOPY(ap->company_id, company_id);
    return 0;
}
int ap_dev_set_dst_white_list(struct ap_dev *ap, const char *dst_white_list)
{
    SAFE_ARRCOPY(ap->dst_white_list, dst_white_list);
    return 0;
}
int ap_dev_set_dst_black_list(struct ap_dev *ap, const char *dst_black_list)
{
    SAFE_ARRCOPY(ap->dst_black_list, dst_black_list);
    return 0;
}
int ap_dev_set_src_white_list(struct ap_dev *ap, const char *src_white_list)
{
    SAFE_ARRCOPY(ap->src_white_list, src_white_list);
    return 0;
}
int ap_dev_set_upgrade_cnt(struct ap_dev *ap, int upgrade_cnt)
{
    ap->upgrade_cnt = upgrade_cnt;
    return 0;
}
int ap_dev_set_online_time(struct ap_dev *ap, long long online_time)
{
    ap->online_time = online_time;
    return 0;
}
int ap_dev_set_leave_time(struct ap_dev *ap, long long leave_time)
{
    ap->leave_time = leave_time;
    return 0;
}
int ap_dev_set_create_time(struct ap_dev *ap, long long create_time)
{
    ap->create_time = create_time;
    return 0;
}

int ap_dev_load_from_db(struct ap_dev *ap, vector<string> &row)
{
    ap_dev_set_hw_addr(ap, row[0].c_str());
    ap_dev_set_serial_number(ap, row[1].c_str());
    ap_dev_set_position(ap, row[2].c_str());
    ap_dev_set_wan_ip(ap, row[3].c_str());
    ap_dev_set_lan_ip(ap, row[4].c_str());
    // ap_dev_set_state(ap, toInt(row[5])); // 状态不读取
    ap_dev_set_group_name(ap, row[6].c_str());
    ap_dev_set_dev_model(ap, row[7].c_str());
    ap_dev_set_hardware_version(ap, row[8].c_str());
    ap_dev_set_software_version(ap, row[9].c_str());
    ap_dev_set_company_id(ap, row[10].c_str());
    ap_dev_set_dst_white_list(ap, row[11].c_str());
    ap_dev_set_dst_black_list(ap, row[12].c_str());
    ap_dev_set_src_white_list(ap, row[13].c_str());
    ap_dev_set_upgrade_cnt(ap, toInt(row[14]));
    ap_dev_set_online_time(ap, toInt64(row[15]));
    ap_dev_set_leave_time(ap, toInt64(row[16]));
    ap_dev_set_create_time(ap, toInt64(row[17]));

    return 0;
}


int ap_dev_set_radios(struct ap_dev *ap, kvlist &kv)
{
    for (int i=0; i<MAX_RADIOS; ++i)
    {
        // 注意：下标并不是radio_id,radio_id一般从1开始
        if (IsSet(kv, STRING_RADIO_ID + toString(i)))
        {
            int radio_id = toInt(GetValue(kv, STRING_RADIO_ID + toString(i)));

            ap->max_radios = radio_id;
            ap->radios[i].radio_id = radio_id;
            ap->radios[i].radio_type = toInt(GetValue(kv, TO_STR(STRING_RADIO_RADIO_TYPE) + toString(i)));

            dlog(LOG_DEBUG, "MAX_RADIOS %d RADIO_ID %d RADIO_TYPE %d",
                 ap->max_radios, ap->radios[i].radio_id, ap->radios[i].radio_type);
        }
        else
        {
            break;
        }
    }
    return 0;
}

string ap_list_str()
{
    aplist::iterator it = aps.begin();
    string str = "";
    int i = 0;
    for (; it != aps.end(); it++)
    {
        str.append(toString(i++) + string(" ") + it->first + string(" ") + it->second->lan_ip +
                   string(" ") + capwap_state_string[it->second->state] + ";");
    }
    str.append("count : " + toString(i));

    return str;
}
int ap_list_run_count()
{
    aplist::iterator it = aps.begin();
    int i = 0;
    for (; it != aps.end(); it++)
    {
        if (it->second->state == CAPWAP_STATE_RUN)
            i++;
    }

    return i;
}
int ap_list_all_count()
{
    return aps.size();
}

bool isOnline(struct ap_dev *ap)
{
    return ap && (CAPWAP_STATE_RUN == ap->state);
}
