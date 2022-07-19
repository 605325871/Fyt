#include "../include/reator_bufer.hh"
enum MEM_CAP
{
    m4k = 4096,
    m16k = 16384,
    m64k = 65535,
    m1M = 1048576
};

reator_bufer::reator_bufer()
{
    _buf = NULL;
}
reator_bufer::~reator_bufer()
{
    this->clear();
}

int reator_bufer::legth() //得到当前还有多少有效数据
{
    if (_buf == NULL)
        return 0;
    else
    {
        return _buf->_length;
    }
}

void reator_bufer::pop(int len) //已经处理了多少buf
{
    assert(_buf != NULL && len <= _buf->_length);
    _buf->pop(len);

    if (_buf->_length == 0)
    {
        buf_pool::instance()->revert(_buf);
        _buf =NULL;
    }
}
void reator_bufer::clear()
{
    if (_buf != nullptr)
    {
        buf_pool::instance()->revert(_buf);
        _buf =nullptr;
    }
}

//////
int input_buffer::read_data(int fd)
{
    int need_read; //看需要多少数据是可以读的
    //一次性读出所有数据，需要给fd 设置为fionread 得到有多少数据是需要读取的

    if (ioctl(fd, FIONREAD, &need_read) == -1)
    {
        std::cerr << "ioctl error" << std::endl;
        return -1;
    }

    if (_buf == nullptr)
    {
        _buf = buf_pool::instance()->alloc_buf(need_read);
        if (_buf == nullptr)
        {
            std::cerr << "no idle buf for alloc" << std::endl;
            exit(-1);
        }
    }
    else
    {
        assert(_buf->_head == 0);

        if (_buf->_capcity - _buf->_length < (int)need_read) //容量-有用的数据=空闲空间《==需要读去的数据
        {
            //不够需要从新申请
            io_buf *newbuf = buf_pool::instance()->alloc_buf(need_read + _buf->_length);
            if (newbuf == NULL)
            {
                std::cerr << "no idle buf for alloc" << std::endl;
                exit(-1);
            }

            newbuf->copy(_buf); //将之前_buff的数据拷贝到新申请的内存中去

            buf_pool::instance()->revert(_buf); //将之前的_buf回收
            _buf = newbuf;
        }
    }

    // 读取数据
    int alread_read = 0;
    do
    {
        // 读取的数据拼接到之前的数据后
        if (need_read == 0)
        {
            // 可能是read阻塞读数据的模式 对方未写入数据
            alread_read = read(fd, _buf->data + _buf->_length, m4k);
        }
        else
        {
            alread_read = read(fd, _buf->data + _buf->_length, need_read);
        }
    } while (alread_read == -1 &&
             errno == EINTR); // systemCall引起的中断 继续读取 防止可重入导致读取中途失败

    if (alread_read > 0)
    {
        if (need_read != 0)
        {
            assert(alread_read == need_read);
        }
        _buf->_length += alread_read;
    }
    return alread_read;
}
//获取当前的数据的方法
const char *input_buffer::data()
{
    return _buf != NULL ? _buf->data += _buf->_head : NULL;
}
//重置缓冲区
void input_buffer::adjust()
{
    if (_buf != NULL)
        _buf->adjust();
}
//////

int ouput_buffer ::send_data(const char *data, int datalen)
{
    if (_buf == nullptr)
    {
        //如果io_buf为空,从内存池申请
        _buf = buf_pool::instance()->alloc_buf(datalen);
        if (_buf == nullptr)
        {
            fprintf(stderr, "no idle buf for alloc\n");
            return -1;
        }
    }
    else
    {
        //如果io_buf可用，判断是否够存
        assert(_buf->_head == 0);
        if (_buf->_capcity - _buf->_length < datalen)
        {
            //不够存，从内存池申请
            io_buf *new_buf = buf_pool::instance()->alloc_buf(datalen + _buf->_length);
            if (new_buf == nullptr)
            {
                fprintf(stderr, "no ilde buf for alloc\n");
                return -1;
            }
            //将之前的_buf的数据考到新申请的buf中
            new_buf->copy(_buf);
            //将之前的_buf放回内存池中
            buf_pool::instance()->revert(_buf);
            //新申请的buf成为当前io_buf
            _buf = new_buf;
        }
    }

    // 将data数据拷贝到io_buf中 拼接到后面
    memcpy(_buf->data + _buf->_length, data, datalen);
    _buf->_length += datalen;

    return 0;
}
//将buf中的数据写到一个fd里面
int ouput_buffer ::write2fd(int fd)
{
    assert(_buf != nullptr && _buf->_head == 0);
    int alredy_write = 0;

    do
    {
        alredy_write = write(fd, _buf->data, _buf->_length);
    } while (alredy_write == -1 && errno == EINTR);

    if (alredy_write > 0)
    {
        //已经将所有数据发回去
        _buf->pop(alredy_write);
        _buf->adjust();
    }
    // 如果fd非阻塞 可能得到EAGAIN错误
    if (alredy_write == -1 && errno == EAGAIN)
    {
        alredy_write = 0; // 不是错误 仅仅返回0 便是当前暂时不可以继续写
    }

    return alredy_write;
}
