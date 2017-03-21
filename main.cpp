#include <iostream>
using namespace std;

#include <list>

#include <sqlite3.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <unistd.h>

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

int main(int argc, char **argv)
{
    //daemon(0,0);
    dopenlog("capwap_server", LOG_CONS | LOG_PID, 0);
    init_config();

    init_db(CAPWAP_SERVER_DB, INIT_AP_LIST_TABLE);

    server_run();

    dcloselog();

    DBI::Close();
    return 0;
}
