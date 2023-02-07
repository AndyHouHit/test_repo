/*************************************************************************
@File Name: tmp.c
@Desc: 
@Author: Andy-wei.hou
@Mail: Andy-wei.hou@outlook.com 
@Created Time: Tue Jun 15 13:56:23 2021
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

int main() {
	int *b[10];

	int *c = b[0];
	printf("point c is %p, a is %d\n", c, a());
}
