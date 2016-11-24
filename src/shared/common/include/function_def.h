#ifndef JXS_FUNCTIONDEF_H_
#define JXS_FUNCTIONDEF_H_
//author:hsj
//对一些常用宏方法的定义

#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Realm(a, min_v, max_v) (Min( (Max((a), (min_v))), (max_v)))

#ifdef JXS_USE_DEBUG
#include <assert.h>
#define JXS_ASSERT(as) assert(as)
#else
#define JXS_ASSERT(as) 0
#endif

#ifdef _WIN32

#ifndef BZERO
#define BZERO(addr,len) ZeroMemory(addr, len) 
#endif

#else

#ifndef BZERO
#define BZERO(addr,len) bzero(addr, len) 
#endif


#endif // !_WIN32



#endif