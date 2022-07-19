#pragma once
#include "io_buf.hh"
#include "buf_pool.hh"
#include <assert.h>
#include <sys/ioctl.h>
 #include <unistd.h>
 
class reator_bufer
{
public:
    io_buf *_buf;

public:
    reator_bufer(/* args */);
    ~reator_bufer();

    int legth(); //得到当前还有多少有效数据

    void pop(int len); //已经处理了多少buf
    void clear();
};

class input_buffer : public reator_bufer
{
public:
    //一个fd中读取数据到reator—buf中
    int read_data(int fd);
    //获取当前的数据的方法
    const char *data();
    //重置缓冲区
    void adjust();
};

class ouput_buffer : public reator_bufer
{
public:
    //将一个数据发送到自身buff中
    int send_data(const char *data, int datalen);
    //将buf中的数据写到一个fd里面
    int write2fd(int fd);
};