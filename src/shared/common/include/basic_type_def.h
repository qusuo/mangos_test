#ifndef JXS_BASICTYPEDEF_H_
#define JXS_BASICTYPEDEF_H_
//author hsj
#include "header_def.h"

#if _WIN32

typedef __int64            Int64;
typedef __int32            Int32;
typedef __int16            Int16;
typedef __int8             Int8;
typedef unsigned __int64   uInt64;
typedef unsigned __int32   uInt32;
typedef unsigned __int16   uInt16;
typedef unsigned __int8    uInt8;

#else  
#if LINNUX

typedef __int64_t   Int64;
typedef __int32_t   Int32;
typedef __int16_t   Int16;
typedef __int8_t    Int8;
typedef __uint64_t  uInt64;
typedef __uint32_t  uInt32;
typedef __uint16_t  uInt16;
typedef __uint8_t   uInt8;
	
#else //android

typedef long long			Int64;
typedef int					Int32;
typedef short				Int16;
typedef char				Int8;
typedef unsigned long long  uInt64;
typedef unsigned int		uInt32;
typedef unsigned short		uInt16;
typedef unsigned char		uInt8;

#endif

#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef INVALID_POS
#define INVALID_POS -1
#endif

#ifdef _WIN32
#ifdef SHEX_DLL_EXPORTS
#define SHEX_DLL_API __declspec(dllexport)
#else
#define SHEX_DLL_API __declspec(dllimport)
#endif
#else
#define SHEX_DLL_API
#endif

#ifdef _WIN32
#include "win32_header_def.h"
#define HSOCKET SOCKET
#define SOCKET_LEN int
#define CXHANDLE HANDLE
#define TTHREAD HANDLE
#else
#define HSOCKET Int32
#define SOCKET_LEN socklen_t
#define CXHANDLE int
#define TTHREAD pthread_t
#endif


#ifdef _WIN32
#define Snprintf	_snprintf_s
#else
#define Snprintf	snprintf
#endif

#endif