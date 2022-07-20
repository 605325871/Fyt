#include "../include/event_loop.hh"

event_loop::event_loop()
{
    //创建epoll句柄
    _epfd = epoll_create(1);
    if (_epfd == -1)
    {
        std::cerr << "epoll create err" << std::endl;
        exit(-1);
    }
} //创建epoll树
event_loop::~event_loop()
{
}
//循环监听epoll事件并处理epoll_wait()
void event_loop::event_process()
{
    io_event_it ev_it;
    while (true)
    {
        int nfds = epoll_wait(_epfd, _fired_evs, MAX_EVENT, -1);

        for (int i = 0; i < nfds; ++i)
        {
            ev_it = _io_evs.find(_fired_evs[i].data.fd);

            //取出对应的事件
            io_event *ev = &(ev_it->second);

            if (_fired_evs[i].events & EPOLLIN)
            {
                //读事件调用读回调函数
                void *args = ev->rcb_args;
                ev->read_callback(this, _fired_evs[i].data.fd, args); //读业务需要自己注册
            }
            else if (_fired_evs[i].events & EPOLLOUT)
            {
                void *args = ev->wcb_args;
                ev->write_callback(this, _fired_evs[i].data.fd, args);
            }
            else if (_fired_evs[i].events & EPOLLOUT & (EPOLLHUP | EPOLLERR))
            {

                //水平触发未处理会触犯hup事件，需要正常处理读写，如果当前事件，既没有读也没有写需要删除
                if (ev->read_callback != NULL)
                {
                    //读事件调用读回调函数
                    void *args = ev->rcb_args;
                    ev->read_callback(this, _fired_evs[i].data.fd, args); //读业务需要自己注册
                }
                else if (ev->write_callback != NULL)
                {
                    void *args = ev->wcb_args;
                    ev->write_callback(this, _fired_evs[i].data.fd, args);
                }
                else
                {
                    fprintf(stderr, "fd,%d get error ,delet frome epoll ", _fired_evs[i].data.fd);
                    this->del_epoll_event(_fired_evs[i].data.fd);
                }
            }
        }
    }
}

//添加一个事件到event_loop中
void event_loop::add_epoll_event(int fd, io_callback *proc, int mask, void *args)
{
    int final_mask;
    int op;
    // 1,找到当前的事件集合fd是否有事件
    io_event_it it = _io_evs.find(fd);
    if (it == _io_evs.end())
    {
        //如果没有以add的方式添加
        op = EPOLL_CTL_ADD;
        final_mask = mask;
    }
    else
    {
        //如果有以mod的方式添
        op = EPOLL_CTL_MOD;
        final_mask = it->second.mask | mask;
    }

    // 2将fd和io——callback绑定到map里面
    if (mask & EPOLLIN)
    {
        //该事件为读事件
        // fd索引到一个io_event结构体
        _io_evs[fd].read_callback = proc;
        _io_evs[fd].rcb_args = args;
    }
    else if (mask & EPOLLOUT)
    {
        _io_evs[fd].write_callback = proc;
        _io_evs[fd].wcb_args = args;
    }
    else
    {
        op = EPOLL_CTL_MOD;
        final_mask = it->second.mask | mask; //妹看懂这步
    }
    // 3,将当前的事件加入到原生的epoll中

    struct epoll_event event;
    event.events = final_mask;
    event.data.fd = fd;
    if (epoll_ctl(_epfd, op, fd, &event) == -1)
    {
        std::cerr << "epoll create err 66" << std::endl;
        return;
    }

    // 4将当前fd添加到listenfdset中
    listen_fd.insert(fd);
}
//删除一个事件到event_loop中
void event_loop::del_epoll_event(int fd)
{
    //将事件map中删除
    _io_evs.erase(fd);

    //将事件从listenfd中删除
    listen_fd.erase(fd);

    //将fd对应的事件从epoll树中摘除、
    epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
}
//删除io事件的触发条件
void event_loop::deltiaojian_epoll_event(int fd, int mask)
{
    io_event_it it = _io_evs.find(fd);
    if (it == _io_evs.end())
    {
        return;
    }
    // o_mask 0011
    // mask  0001 ~mask = 1110
    //            o_mask= 0011
    //                    0010

    //此时it就是需要删除的条件
    int o_mask = it->second.mask;
    o_mask = mask & (~mask);

    if (o_mask == 0) //说明已经没有事件需要删除
    {
        this->del_epoll_event(fd);
    }
    else
    {
        struct epoll_event ev;
        ev.events = o_mask;
        ev.data.fd = fd;
        epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &ev);
    }
}