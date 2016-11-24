#ifndef JXS_NETWORKDEF_H_
#define JXS_NETWORKDEF_H_
//author:hsj
//关于网络的一些定义
#include "../include/header_def.h"


#ifdef _WIN32
#include "../include/win32_header_def.h"
#define NET_INVALID_SOCKET INVALID_SOCKET
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
typedef SOCKET SocketHandle;
typedef HANDLE CommonHandle;
typedef int SocketLen;

#else

#define NET_INVALID_SOCKET -1
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
typedef int SocketHandle;
typedef int CommonHandle;
typedef socklen_t SocketLen;

#endif

const int MAX_IP_STR_LEN = 32;


#endif