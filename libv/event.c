#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef _WIN32
#include <netinet/in.h>
#ifdef _XOPEN_SOURCE_EXTENDED
#include <arpa/inet.h>
#endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

int main()
{
    struct event_base *base = event_base_new();

    const char **buf;
    buf = event_get_supported_methods();
    for (int i = 0; buf[i]!=NULL; ++i)
        printf("%s\n", buf[i]);
}
