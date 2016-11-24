#ifndef JXS_LOGDEF_H_
#define JXS_LOGDEF_H_
//author:hsj
#include <stdio.h>

#define LOG_STRINGIFY(x) #x
#define LOG_TOSTRING(x) LOG_STRINGIFY(x)	
#define LOG_ADDR "["__FILE__": "LOG_TOSTRING(__LINE__)"]:"

#endif