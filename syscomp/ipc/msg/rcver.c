#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "proto.h"

int main()
{
    key_t key;
    int msgid;
    struct msg_st rbuf;
    key = ftok(keypath, keyproj);
    if (key < 0)
    {
        perror("ftok");
        exit(-1);
    }
    msgid = msgget(key, IPC_CREAT | 0600); //
    if (msgid < 0)
    {
        perror("msgget");
        exit(-1);
    }

    while (1)
    {
        if (msgrcv(msgid, &rbuf, sizeof(rbuf) - sizeof(long), 0, 0) < 0)
        {
            perror("msgcv");
            exit(1);
        }
        printf(" name = %s\n", rbuf.name);
        printf(" name = %d\n", rbuf.math);
        printf(" name = %d\n", rbuf.chinese);
    }
    msgctl(msgid, IPC_RMID, NULL);
    exit(0);
}