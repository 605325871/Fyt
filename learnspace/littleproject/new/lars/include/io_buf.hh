#pragma once
#include <iostream>
#include<assert.h>
#include<memory.h>
class io_buf
{
public:
    io_buf(int size);
    ~io_buf();

    void pop(int len);            // len 表示已经处理的数据长度
    void adjust();                //将已经处理数据清空将未处理的数据移到buf的首地址，length有效长度减少
    void clear();                 //清空数据
    void copy(const io_buf *rhs); //将其他的io_buf对象拷贝到拷贝到自己中

public:
    int _capcity; // 容量
    int _length;  // 未处理数据的总有效长度
    int _head;    //当前未处理有效的头部索引
    char *data;    //当前buf的内存首地址
    io_buf *next; //存在多个io_buf采用链表的形式进行管理
};

//取数据 io_buf.data+head,lenth
// io_buf.dat,3
// io_buf.pop(3)
// io_buf.adjust