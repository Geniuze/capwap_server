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

#include <uci.h>

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
    struct uci_context *ctx = NULL;
    struct uci_element *e = NULL;
    struct uci_package *p = NULL;
    int ret = 0;

    ctx = uci_alloc_context();
    if (NULL == ctx)
    {
        dlog(LOG_ERR, "%s.%d {uci_alloc_context error}", __FUNC__, __LINE__);
        return -1;
    }
    ret = uci_load(ctx, CAPWAP_CONFIG_FILE, &p);
    if (0 != ret)
    {
        dlog(LOG_ERR, "%s.%d {uci_load error ret = %d}", __FUNC__, __LINE__, ret);
        return -1;
    }

    uci_foreach_element(&p->sections, e)
    {
        struct uci_section *s = uci_to_section(e);
        struct uci_element *ep = NULL;
        string sname = s->e.name;
        if (sname == CAPWAP_ECHO_CONFIG)
        {
            uci_foreach_element(&s->options, ep)
            {
                struct uci_option *o = uci_to_option(ep);
                string oname = o->e.name;
                dlog(LOG_DEBUG, "%s.%d {oname = %s}", __FUNC__, __LINE__, oname.c_str());
                if (oname == CAPWAP_ECHO_CONFIG_INTERVAL)
                {
                    echo_interval = toInt((o->type == UCI_TYPE_STRING)?o->v.string:"10");
                }
                if (oname == CAPWAP_ECHO_CONFIG_COUNT)
                {
                    echo_count = toInt((o->type == UCI_TYPE_STRING)?o->v.string:"3");
                }
            }
        }
        if (sname == CAPWAP_TS_CONFIG)
        {
            uci_foreach_element(&s->options, ep)
            {
                struct uci_option *o = uci_to_option(ep);
                string oname = o->e.name;
                dlog(LOG_DEBUG, "%s.%d {oname = %s}", __FUNC__, __LINE__, oname.c_str());
                if (oname == CAPWAP_TS_CONFIG_ENABLE)
                {
                    ts_enable = toInt((o->type == UCI_TYPE_STRING)?o->v.string:"0");
                }
                if (oname == CAPWAP_TS_CONFIG_INTERVAL)
                {
                    ts_interval = toInt((o->type == UCI_TYPE_STRING)?o->v.string:"30");
                }
            }
        }
        if (sname == CAPWAP_UPGRADE_CONFIG)
        {
            uci_foreach_element(&s->options, ep)
            {
                struct uci_option *o = uci_to_option(ep);
                string oname = o->e.name;
                dlog(LOG_DEBUG, "%s.%d {oname = %s}", __FUNC__, __LINE__, oname.c_str());
                if (oname == CAPWAP_UPGRADE_CONFIG_ENABLE)
                {
                    upgrade_enable = toInt((o->type == UCI_TYPE_STRING)?o->v.string:"0");
                }
            }
        }
        if (sname == CAPWAP_UPGRADE_SERVER)
        {
            uci_foreach_element(&s->options, ep)
            {
                struct uci_option *o = uci_to_option(ep);
                string oname = o->e.name;
                dlog(LOG_DEBUG, "%s.%d {oname = %s}", __FUNC__, __LINE__, oname.c_str());
                if (oname == CAPWAP_UPGRADE_SERVER_TYPE)
                {
                    upgrade_server_type = (o->type == UCI_TYPE_STRING)?o->v.string:"ftp";
                }
                if (oname == CAPWAP_UPGRADE_SERVER_ADDR)
                {
                    upgrade_server_addr = (o->type == UCI_TYPE_STRING)?o->v.string:"";
                }
                if (oname == CAPWAP_UPGRADE_SERVER_USERNAME)
                {
                    upgrade_server_username = (o->type == UCI_TYPE_STRING)?o->v.string:"anonymous";
                }
                if (oname == CAPWAP_UPGRADE_SERVER_PASSWORD)
                {
                    upgrade_server_password = (o->type == UCI_TYPE_STRING)?o->v.string:"anonymous";
                }
                if (oname == CAPWAP_UPGRADE_SERVER_PATH)
                {
                    upgrade_server_path = (o->type == UCI_TYPE_STRING)?o->v.string:"";
                }
            }
        }
    }

    dlog(LOG_DEBUG, "%s.%d {echo_interval = %d, echo_timeout = %d}",
         __FUNC__, __LINE__, echo_interval, echo_count);
    dlog(LOG_DEBUG, "%s.%d {ts_enable = %d, ts_interval = %d}",
         __FUNC__, __LINE__, ts_enable, ts_interval);
    dlog(LOG_DEBUG, "%s.%d {upgrade_enable = %d}",
         __FUNC__, __LINE__, upgrade_enable);
    dlog(LOG_DEBUG, "%s.%d {upgrade_server_type = %s, upgrade_server_addr = %s,"
         "upgrade_server_username = %s, upgrade_server_password = %s,"
         "upgrade_server_path = %s}",
         __FUNC__, __LINE__,
         upgrade_server_type.c_str(), upgrade_server_addr.c_str(),
         upgrade_server_username.c_str(), upgrade_server_password.c_str(),
         upgrade_server_path.c_str());

    uci_unload(ctx, p);
    uci_free_context(ctx);

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
        DBI::exec(INIT_UPGRADE_STRATEGY_LIST_TABLE, &err);

        DBI::Insert(TO_STR(GROUP_LIST), STRING_NAME, "'default'");
        DBI::Insert(TO_STR(RADIO_2G_LIST), STRING_NAME, "'default'");
        DBI::Insert(TO_STR(RADIO_2G_LIST), STRING_NAME, "'default22'");
        DBI::Insert(TO_STR(RADIO_5G_LIST), STRING_NAME, "'default'");
        DBI::Insert(TO_STR(WP_LIST), STRING_NAME, "'default'");
        DBI::Insert(TO_STR(WLAN_LIST), STRING_INDEX","STRING_NAME","TO_STR(STRING_WLAN_ESSID)","
                    TO_STR(STRING_WLAN_WLAN_ID)","TO_STR(STRING_WLAN_AUTH_TYPE)","TO_STR(STRING_WLAN_PORTAL_URL),
                    "1,'default','default_ssid',1,0,''");
        DBI::Insert(TO_STR(WLAN_SECURE_LIST), STRING_NAME, "'open'");
        DBI::Insert(TO_STR(NTP_SERVER_LIST), STRING_NAME, "'default'");

        DBI::Close();
    }
    DBI::Open(file);
    return 0;
}

int init_db_data()
{
    string key_value;
    key_value.append(STRING_STATE"=" + toString(CAPWAP_STATE_QUIT));

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
