#include <iostream>
using namespace std;

#include <list>

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
        DBI::exec(INIT_WP_LIST_TABLE, &err);
        DBI::Close();
    }
    DBI::Open(file);
    return 0;
}

int init_db_data()
{
    DBI::Insert(GROUP_LIST, DB_STRING_GROUP_NAME, "'default'");
    DBI::Insert(RADIO_2G_LIST, DB_STRING_RADIO_2G_STRATEGY_NAME, "'default'");
    DBI::Insert(RADIO_5G_LIST, DB_STRING_RADIO_5G_STRATEGY_NAME, "'default'");
    DBI::Insert(WP_LIST, DB_STRING_WIRELESS_POSITION_NAME, "'default'");
    return 0;
}

int main(int argc, char **argv)
{
    //daemon(0,0);
    dopenlog("capwap_server", LOG_CONS | LOG_PID, 0);
    init_config();

    init_db(CAPWAP_SERVER_DB);
    init_db_data();

    server_run();

    dcloselog();

    DBI::Close();
    return 0;
}
