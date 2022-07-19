#include "../include/io_buf.hh"

io_buf::io_buf(int size)
{
    _capcity = size;
    _length = 0;
    _head = 0;
    next = nullptr;

    data = new char[size];
    assert(data != nullptr);
}

void io_buf::pop(int len)
{
    _length -= len;
    _head += len;
} // len 表示已经处理的数据长度
void io_buf::adjust() //将已经处理数据清空将未处理的数据移到buf的首地址，length有效长度减少
{
    if (_head != 0)
    {
        // legnth为0说明 全部数据已经处理完毕
        if (_length != 0)
        {
            memmove(data, data + _head, _length);
        }
        _head = 0;
    }
}
void io_buf::copy(const io_buf *rhs) //将其他的io_buf对象拷贝到拷贝到自己中
{
    memcpy(data, rhs->data + rhs->_head, rhs->_length);
    _head = 0;
    _length = rhs->_length;
}
void io_buf::clear() //清空数据
{
    _length = _head = 0;
}