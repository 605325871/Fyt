#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include "proto.h"
// #define keypath "/etc/services"
// #define keyproj 'g'

int main()
{
    key_t key;
    int msgid;
    struct msg_st sbuf;
    key = ftok(keypath,keyproj); //获取key
    if (key < 0)
    {
        perror("ftok");
        exit(-1);
    }
    msgid = msgget(key, 0); //初始化消息队列
    if (msgid < 0)
    {
        perror("msgget");
        exit(-1);
    }
    sbuf.mtype = 1;
    strcpy(sbuf.name, "zhangsan");
    sbuf.math = rand() % 100;
    sbuf.chinese = rand() % 100;

    if (msgsnd(msgid, &sbuf, sizeof(sbuf) - sizeof(long), 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    puts(" ok !");
    exit(0);
}