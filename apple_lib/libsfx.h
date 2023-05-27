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

#ifndef _COMMON_DEFINITIONS_
#define	 _COMMON_DEFINITIONS_

// Generally,there are 2x categories for Error Code.
// The below are for APIs invocation. They are about APIs task operations.
//
/*ERROR CODE DEFINATION*/
// Please Note: beyond ENCODE_ERROR(0x1000)	are	reserved for vendor specific.
#define	MAX_BIT                     0x8000000000000000
#define	ENCODE_ERROR(a)             (MAX_BIT    |   (a))
#define TASK_OPERATE_SUCCESS        0

//already called API for the task, need to wait until it is finished or aborted.
#define TASK_ALREADY_STARTED        ENCODE_ERROR(1)

//invalid parameter for API
#define TASK_INVALID_PARAMETER      ENCODE_ERROR(2)

//unsupported for optional APIs
#define	 TASK_UNSUPPORTED           ENCODE_ERROR(3)

//out of resource, like memory and etc
#define	 TASK_OUT_OF_RESOURCES      ENCODE_ERROR(4)
#define	 TASK_ABORT_FAIL		    ENCODE_ERROR(5)
#define	 TASK_CANNOT_ABORT			ENCODE_ERROR(6)
#define	 TASK_DRIVE_NOT_READY	    ENCODE_ERROR(7)
#define	 TASK_DRIVE_NOT_FOUND       ENCODE_ERROR(8)
#define	 TASK_DRIVE_NO_RESPONSE     ENCODE_ERROR(9)

//snapshot file	not	matched for getWAF()
#define	 FILE_NOT_MATCHED           ENCODE_ERROR(10)



#endif /*_COMMON_DEFINITIONS_*/



#endif


