/*************************************************************************
@File Name: libsfx.h
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: 2023年05月25日 星期四 21时42分47秒
@Log: 
************************************************************************/
#ifndef __LIB_SFX_X_H__
#define __LIB_SFX_X_H__
#include<iostream>
namespace sfx_namespace {
    bool getFuncArray(uint64_t array[], uint32_t size);
    int64_t startHealthCheck(const std::string &devNode);
    int64_t fun2(void);
}

#endif


