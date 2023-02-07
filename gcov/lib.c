/*************************************************************************
@File Name: lib.c
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: Tue Feb  7 18:11:36 2023
@Log: 
************************************************************************/

#include<stdio.h>

int x = 3; 

int a() {
    switch (x) {
    case 3:
    case 4:
        x++;
    default:
    return a() +1;

    case 5: break;
}
    return x;
}


