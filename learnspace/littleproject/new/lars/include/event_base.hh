#pragma once

class event_loop;
typedef void io_callback(event_loop *loop, int fd, void *args);
//封装io事件
struct io_event
{
    io_event()
    {
        mask = 0;
        write_callback = nullptr;
        read_callback = nullptr;
        rcb_args = nullptr;
        wcb_args = nullptr;
    }

    //事件的读写属性
    int mask; // EPOLLIN  and   EPOLLOUT

    //读事件触发的回调函数
    io_callback *read_callback;
    //写事件触发的回调函数
    io_callback *write_callback;
    //读事件触发的回调函数的形参
    void *rcb_args;
    //写事件触发回调函数的形参
    void *wcb_args;
};
