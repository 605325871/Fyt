#pragma once

//封装io事件
struct io_event
{
    io_event();

    //事件的读写属性
    int mask; // EPOLLIN  and   EPOLLOUT
    //读事件触发的回调函数
};
