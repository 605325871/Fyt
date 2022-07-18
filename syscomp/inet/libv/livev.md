框架

# 1,创建event_base
>#include <evevnt2/event.h>
\
struct event_base* event_base_new(void)
struct event_base* base = NULL;
base = event_base_new;


# 2,创建事件
>常规事件 event-->event_new()
buffevent->bufferevent_socket_new()

# 3,将事件添加到base上
>int event_add(struct event*ev,const struct timeval*tv)

# 4,循环监听事件满足
>int event_base_dispatch(struct event_base* base)
    event_base_dispatch(base)

# 5,释放 event_base
>    event_base_free(*base*)

***
# 创建事件
struct event *event_new(struct event_base *base, int fd, short events, event_callback_fn callback, void *callback_arg)

* base   创建的sturct event_base* base  变量 类似创建epoll 的红黑树pfd
* fd     捆绑到事件上的文件描述符
* what    对应的监听事件 读写或异常 （r, w,e)
   * EV_READ 一次读事件
   * EV_WRITE 一次写事件
   * EV_PRERSIST 持续出发，结合event_base_dispatch函数使用，生效
  
*cb 满足事件所调用的函数
返回值 成功创建的event
 
 typedef void(*event_callback_fn)(int  fd,short,void*)
## 添加事件
int event_add(struct event *ev, const struct timeval *timeout)
* ev event_new() 函数的返回值
* timeout 时间

## 删除事件
int event_del(struct event *ev) 
## 销毁事件
int event_free() 

****
# bufferent
buffevent 有两个缓冲区，也是队列实现，读走没，先进先出

读 有数据，会触发读缓冲的对应的回调函数 ->使用buffread-> 读取数据
写 使用buffevent_write（）->向缓冲区中写数据->该缓冲区有数据会自动写出，->写完，回调函数被调用（ ）

创建buffevent:
 >  struct bufferevent * bufferevent_socket_new(struct event_base *base, int fd, int options)

base : event_base;
fd: 封装到buffevent内的fd
options： BEV_OPT_CLOSE_ON_FREE //释放时关闭底层传输窗口，这将关闭底层套接字，释放底层buffevent

返回：成功的buffevent事件对象

给buffevent设置回调函数
    void bufferevent_setcb(struct bufferevent *bufev, bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb, void *cbarg)
* buffevent_new返回值
* readcb 读缓冲设置回调 {bufferevent_read()}
* writcb 写缓冲设置回调  
* eventcb  设置状态回调
* cbarg 上述回调函数使用的参数
  
  typedef void(*buffevent_data_cb)(struct buffevent * bev,void *arg)

   typedef void(*buffevent_read_cb)(struct buffevent * bev,void *arg)

   buffevent_read(struct bufferevent* bev, void *arg)

# 启动关闭bufferevent缓冲区
 默认写是enable 读是unable
void bufferevent_enable(struct bufferevent* buffev,short events)


客户端
 socket() connect()

int bufferevent_socket_connect(struct bufferevent *bufev, const struct sockaddr *addr, int socklen)


