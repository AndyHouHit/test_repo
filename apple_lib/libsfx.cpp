/*************************************************************************
@File Name: libsfx.cpp
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: 2023年05月25日 星期四 21时39分12秒
@Log: 
************************************************************************/

#include <string.h>
#include "libsfx.h"
#include "sfx_ssd_srv.h"

using namespace std;

uint64_t sfxFunArray[] = { 
    (uint64_t) sfx_namespace::getFuncArray,
    (uint64_t) sfx_namespace::startHealthCheck,

    (uint64_t) sfx_namespace::fun2,
};


bool sfx_namespace::getFuncArray(uint64_t array[], uint32_t size)
{
    char buf[] = "SFX09100";
    memcpy(&array[0], buf, 8);
    uint32_t sfxFunArrSize = sizeof(sfxFunArray)/sizeof(uint64_t);

    for (int i = 1; i < min(sfxFunArrSize, size); i++) {
        array[i] = sfxFunArray[i];
    }

    /*create sfx_srv service thread if not alive*/
    if (!sfx_ssd_srv_alive()) {
        if (start_sfx_ssd_srv()) {
            return false;
        }
    }
    return true;
}

int64_t sfx_namespace::startHealthCheck(const std::string &devNode)
{
    cout << __func__ << "("  << devNode << ")"<< ":0x"<< hex << (uint64_t)startHealthCheck << endl;
    return 0;
}

int64_t sfx_namespace::fun2(void)
{
    cout <<__func__ << "        :0x"<< hex <<(uint64_t)fun2 << endl;
    return 0;
}
