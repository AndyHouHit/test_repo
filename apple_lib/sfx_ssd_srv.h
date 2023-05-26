/*************************************************************************
@File Name: sfx_ssd_srv.h
@Desc: 
@Author: Andy-wei.hou
@Mail: wei.hou@scaleflux.com 
@Created Time: 2023年05月26日 星期五 23时18分34秒
@Log: 
************************************************************************/
#ifndef __SFX_SSD_SRV_H__
#define __SFX_SSD_SRV_H__

int start_sfx_ssd_srv(void);
int sfx_ssd_srv_alive(void);
int64_t sfx_ssd_send_req(void *req_msg, int req_len, void *resp_msg);

#endif
