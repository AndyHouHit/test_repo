/*************************************************************************
@File Name: main.cpp
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: 2023年05月25日 星期四 21时59分46秒
@Log: 
************************************************************************/

#include<iostream>
#include "libsfx.h"
using namespace std;

typedef int64_t (*pFUN1)(const std::string);
typedef int64_t (*pFUN2)(void);


int main()
{
    cout << "Hello" << endl;

    uint64_t arr[40] = { 0 };

    sfx_namespace::getFuncArray(arr, 3);

    for (int i = 0 ; i < 3; i++)
        cout << "0x" << hex << arr[i] <<endl;

    pFUN1 pfun1 = (pFUN1)arr[1];    
    if (pfun1)
        pfun1("/dev/nvme0");
    pFUN2 pfun2 = (pFUN2)arr[2];
    if (pfun2)
        pfun2();

    // cout<< "size:" << sizeof(sfx_namespace::funArray) <<endl;
    return 0;
}
