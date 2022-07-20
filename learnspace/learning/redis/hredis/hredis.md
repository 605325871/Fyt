#include <hiredis/hiredis.h>

typedef struct redisContext
{
    int err;          /* 错误码  成功返回0*/
    char errstr[128]; /* String representation of error when applicable y应该是返回的错误原因*/
    int fd;
    int flags;
    char *obuf;          /* Write buffer */
    redisReader *reader; /* Protocol reader */

    enum redisConnectionType connection_type;
    struct timeval *timeout;

    struct
    {
        char *host;
        char *source_addr;
        int port;
    } tcp;

    struct
    {
        char *path;
    } unix_sock;

    /* For non-blocking connect */
    struct sockadr *saddr;
    size_t addrlen;
} redisContext;


typedef struct redisReply {
    int type; /* REDIS_REPLY_* */
    long long integer; /* The integer when type is REDIS_REPLY_INTEGER */
    size_t len; /* Length of string */
    char *str; /* Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
    size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
    struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */
} redisReply;



// 该函数用来连接redis数据库，参数为ip地址和端口号，默认端口6379.该函数返回一个redisContext对象
redisContext *redisConnect(const char *ip, int port);

// 该函数执行redis命令，返回redisReply对象
void *redisCommand(redisContext *c, const char *format, ...);

// 释放redisCommand执行后返回的RedisReply对象
void freeReplyObject(void *reply);

// 断开redisConnect所产生的连接
void redisFree(redisContext *c);

// redisReply对象结构如下：
typedef struct redisReply
{
    int type;                    // 返回结果类型
    long long integer;           // 返回类型为整型的时候的返回值
    size_t len;                  // 字符串的长度
    char *str;                   // 返回错误类型或者字符串类型的字符串
    size_t elements;             // 返回数组类型时，元素的数量
    struct redisReply **element; // 元素结果集合
} redisReply;

// 返回类型有一下几种：
REDIS_REPLY_STRING 1      //字符串
    REDIS_REPLY_ARRAY 2   //数组，多个reply，通过element数组以及elements数组大小访问
    REDIS_REPLY_INTEGER 3 //整型
    REDIS_REPLY_NIL 4     //空，没有数据
    REDIS_REPLY_STATUS 5  //状态，str字符串以及len
    REDIS_REPLY_ERROR 6   //错误，同STATUS
