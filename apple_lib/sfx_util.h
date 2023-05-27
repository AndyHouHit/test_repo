/*************************************************************************
@File Name: sfx_util.h
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: 2023年05月27日 星期六 00时16分42秒
@Log: 
************************************************************************/
#ifndef __SFX_UTIL_H__
#define __SFX_UTIL_H__

#define DEBUG_SRV
#define DEBUG

#define pr_info printf

#ifdef DEBUG
#define pr_dbg  printf
#else
#define pr_dbg(fmt, args...)                 \
do											\
{ 									  		\
}while(0)
#endif


#ifdef DEBUG_SRV
#define pr_dbg_srv  printf
#else
#define pr_dbg_srv(fmt, args...)            \
do											\
{ 									  		\
}while(0)
#endif

inline void hexdump(const char *disp, void *buf, int len)
{
    int i = 0;
    unsigned char *tmp = (unsigned char *)buf;
    pr_dbg("%s[\n", disp ? disp : "");
    for (i = 0; i < len; i++) {
        pr_dbg(" 0x%2X", *(tmp+i));
        if (i % 16 == 0 && i > 0) {
            pr_dbg("\n");
        }
    }
    pr_dbg("]\n");
}

#endif
