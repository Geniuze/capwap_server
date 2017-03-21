#include <iostream>
using namespace std;

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "interface.h"
#include "log.h"


string GetInterfaceAddress(const char *interface)
{
    struct ifreq ifr;
    struct sockaddr_in *addr = NULL;
    string ip_addr = "";
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( fd < 0 )
    {
        dlog(LOG_DEBUG, "socket error : %s", strerror(errno));
        return "";
    }
    bzero(&ifr, sizeof(ifr));
    strcpy(ifr.ifr_name, interface);
    addr = (struct sockaddr_in *)&ifr.ifr_addr;
    addr->sin_family = AF_INET;


    if (0 == ioctl(fd, SIOCGIFADDR, &ifr))
    {
        ip_addr = inet_ntoa(addr->sin_addr);
    }
    close(fd);

    return ip_addr;
}
string GetInterfaceAddress(string &interface)
{
    return GetInterfaceAddress(interface.c_str());
}
