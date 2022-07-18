#pragma once
#define keypath "/etc/services"
#define keyproj 'g'

#define namesize 32
struct msg_st
{
    long mtype;
    char name[namesize];
    int math;
    int chinese;
};