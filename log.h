#ifndef __LOG_H__
#define __LOG_H__


#include <syslog.h>
#include <errno.h>
#include <stdio.h>

#define dopenlog(ident, option, facility)       \
    openlog(ident, option, facility)

#define dcloselog() \
    closelog()

#define dlog(pri, ...) \
    do {                                        \
        printf(__VA_ARGS__);                    \
        printf("\n");                           \
    }while(0)


#endif
