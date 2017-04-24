#include <iostream>
using namespace std;

#include <list>
#include <vector>

#include <sqlite3.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>


extern "C" {
#include "libubus.h"
#include "libubox/blobmsg.h"
#include "libubox/blobmsg_json.h"
#include "libubox/ustream.h"
}

#include "server.h"
#include "ubus.h"
#include "dbi.h"
#include "ap_list.h"
#include "log.h"
#include "business.h"
#include "capwap_state.h"
#include "dstring.h"
#include "config_manager.h"

/**
   初始化配置
   --最大AP数
   --license 相关配置
 */
int init_config()
{
    return 0;
}

int init_db(const char *file)
{
    struct stat st;
    char *err = NULL;

    if (stat(file, &st) != 0)
    {
        DBI::Open(file);
        DBI::exec(INIT_AP_LIST_TABLE, &err);
        DBI::exec(INIT_GROUP_LIST_TABLE, &err);
        DBI::exec(INIT_RADIO_2G_LIST_TABLE, &err);
        DBI::exec(INIT_RADIO_5G_LIST_TABLE, &err);
        DBI::exec(INIT_WLAN_LIST_TABLE, &err);
        DBI::exec(INIT_WLAN_SECURE_LIST_TABLE, &err);
        DBI::exec(INIT_WP_LIST_TABLE, &err);
        DBI::exec(INIT_LAN_PORTAL_LIST_TABLE, &err);
        DBI::exec(INIT_RFG_LIST_TABLE, &err);
        DBI::exec(INIT_RATE_SET_LIST_TABLE, &err);
        DBI::exec(INIT_NTP_SERVER_LIST_TABLE, &err);
        DBI::exec(INIT_PORTAL_CUSTOM_LIST_TABLE, &err);
        DBI::Close();
    }
    DBI::Open(file);
    return 0;
}

int init_db_data()
{
    string key_value;
    key_value.append(STRING_STATE"=" + toString(CAPWAP_STATE_QUIT));

    DBI::Insert(TO_STR(GROUP_LIST), STRING_NAME, "'default'");
    DBI::Insert(TO_STR(RADIO_2G_LIST), STRING_NAME, "'default'");
    DBI::Insert(TO_STR(RADIO_2G_LIST), STRING_NAME, "'default22'");
    DBI::Insert(TO_STR(RADIO_5G_LIST), STRING_NAME, "'default'");
    DBI::Insert(TO_STR(WP_LIST), STRING_NAME, "'default'");
    DBI::Insert(TO_STR(WLAN_LIST), STRING_INDEX","STRING_NAME","TO_STR(STRING_WLAN_ESSID)","
                TO_STR(STRING_WLAN_WLAN_ID)","TO_STR(STRING_WLAN_AUTH_TYPE)","TO_STR(STRING_WLAN_PORTAL_URL),
                "1,'default','default_ssid',1,0,'http://10.0.3.228/htmls/portal/default.html'");
    DBI::Insert(TO_STR(WLAN_LIST), STRING_INDEX","STRING_NAME","TO_STR(STRING_WLAN_ESSID)","
                TO_STR(STRING_WLAN_WLAN_ID)","TO_STR(STRING_WLAN_AUTH_TYPE)","TO_STR(STRING_WLAN_PORTAL_URL),
                "2,'default22','default_ssid22',1,1,'http://10.0.3.228/htmls/portal/default.html'");
    DBI::Insert(TO_STR(WLAN_LIST), STRING_INDEX","STRING_NAME","TO_STR(STRING_WLAN_ESSID)","
                TO_STR(STRING_WLAN_WLAN_ID)","TO_STR(STRING_WLAN_AUTH_TYPE)","TO_STR(STRING_WLAN_PORTAL_URL),
                "3,'default22','default_ssid222',2,0,'http://10.0.3.228/htmls/portal/default.html'");
    DBI::Insert(TO_STR(WLAN_SECURE_LIST), STRING_NAME, "'open'");
    DBI::Insert(TO_STR(NTP_SERVER_LIST), STRING_NAME, "'default'");
    DBI::Insert(TO_STR(PORTAL_CUSTOM_LIST), STRING_NAME","TO_STR(STRING_PORTAL_CUSTOM_DATA),
                "'default','{\"portal_custom_count\":\"1\",\"portal_custom_content\":[{\"portal_custom_key\":\"extern\",\"portal_custom_alias\":\"aaaaa\",\"portal_custom_value\":\"bbbbb\"}]}'");

    DBI::Update(TO_STR(AP_LIST), key_value.c_str(), NULL);
    return 0;
}

int init_business_timeout()
{
    business_timeout.cb = business_timeout_cb;
    uloop_timeout_set(&business_timeout, BUSINESS_TIMEOUT_INTERVAL);

    return 0;
}

int main(int argc, char **argv)
{
    //daemon(0,0);
    dopenlog("capwap_server", LOG_CONS | LOG_PID, 0);
    init_config();

    init_db(CAPWAP_SERVER_DB);
    init_db_data();

    init_business_timeout();

    server_run();

    dcloselog();

    DBI::Close();
    return 0;
}
