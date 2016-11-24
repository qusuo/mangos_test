#ifndef JXS_VALUEDEF_H_
#define JXS_VALUEDEF_H_

#include "Platform/Define.h"

//author:
//对一些数值常量的定义

//64位整型数值最大值
#define MAX_INT64_NUMBER ((int64)(~((uint64)0)>>1))
//32位整型数值最大值
#define MAX_INT32_NUMBER ((int32)(~((uint32)0)>>1))
//16位整型数值最大值
#define MAX_INT16_NUMBER 0x7FFF

//定义常用缓冲区大小
#define MAX_32M_BUFFER_LEN  32*1024*1024
#define MAX_6M_BUFFER_LEN  6*1024*1024
#define MAX_4M_BUFFER_LEN  4*1024*1024
#define MAX_2M_BUFFER_LEN  2*1024*1024
#define MAX_1M_BUFFER_LEN  1024*1024

#define MAX_1024K_BUFFER_LEN 1024*1024
#define MAX_512K_BUFFER_LEN	512*1024
#define MAX_128K_BUFFER_LEN	128*1024
#define MAX_64K_BUFFER_LEN	64*1024
#define MAX_32K_BUFFER_LEN	32*1024
#define MAX_24K_BUFFER_LEN 24*1024
#define MAX_16K_BUFFER_LEN	16*1024
#define MAX_12K_BUFFER_LEN 12*1024
#define MAX_9K_BUFFER_LEN 9*1024

#define MAX_8K_BUFFER_LEN  8192
#define MAX_4K_BUFFER_LEN  4096
#define MAX_2K_BUFFER_LEN  2048
#define MAX_1K_BUFFER_LEN  1024

#define MAX_32_CHAR_LEN 32
#define MAX_64_CHAR_LEN 64
#define MAX_128_CHAR_LEN 128
#define MAX_256_CHAR_LEN 256
#define MAX_512_CHAR_LEN 512

#define INVALID_POS -1
#define INVALID_INDEX 0
#define SUCCEED_RES 0
#define FAIL_RES -1

#endif