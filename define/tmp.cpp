/*************************************************************************
@File Name: tmp.c
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: Wed Feb 15 16:35:01 2023
@Log: 
************************************************************************/

#include<stdio.h>

/** #define MSG "hello, world" */
#define TOSTRING(x) #x


int main(int argc, char *argv[])
{
#if OPN == MSG
    printf("%d, OPN %s\n", OPN, TOSTRING(OPN));
#else
    printf("%d\n", MSG);
    printf("%s\n", TOSTRING(MSG));
#endif

}

