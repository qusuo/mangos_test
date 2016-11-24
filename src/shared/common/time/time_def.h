#ifndef JSX_TIMEDEF_H_
#define JSX_TIMEDEF_H_

//author: hsj
//对操作系统的一些时间操作的定义

#ifdef _WIN32
#include "../include/win32_header_def.h"
#else
#include<sys/time.h>
#endif

#ifdef _WIN32
#define SSleep(s) Sleep(s * 1000)
#define MSleep(ms) Sleep(ms)     //毫秒
#define USleep(us) Sleep(us) //微秒(windows下暂时用毫秒）
#else
#define SSleep(s) sleep(s)
#define MSleep(ms) usleep(ms * 1000)
#define USleep(us) usleep(us) //微秒
#endif



#endif