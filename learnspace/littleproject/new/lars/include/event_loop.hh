#pragma once
#include <map>
#include <set>
#include <iostream>
#include "event_base.hh"
#include <sys/epoll.h>

#define MAX_EVENT 1024
typedef std::map<int, io_event> io_event_map;
typedef io_event_map::iterator io_event_it;

// set value fd 表示当前正在监听的fd集合
typedef std::set<int> listen_fd_set;

class event_loop
{
private:
    int _epfd; //创建epoll树

    //当前的event——loop 监控的fd和其对应的事件
    io_event_map _io_evs;

    //当前event—loop都有那些fd正在监听 epoll_wait()正在等待那些fd触发的状态
    //作用是将当前的服务器可以主动的给客户端发送消息
    //当前的fd集合中表示在线的fd
    listen_fd_set listen_fd;

    //每次epoll——wait 被激活的事件集合
    struct epoll_event _fired_evs[MAX_EVENT];

public:
    event_loop(); //创建epoll树
    ~event_loop();

    //循环监听epoll事件并处理
    void event_process();

    //添加一个事件到event_loop中
    void add_epoll_event(int fd, io_callback *proc, int mask, void *args);
    //删除一个事件到event_loop中
    void del_epoll_event(int fd);
    //删除io事件的触发条件
    void deltiaojian_epoll_event(int fd, int mask);
};