#ifndef JXS_THREADDEF_H_
#define JXS_THREADDEF_H_
//author:hsj
//对线程的一些定义
#include "../include/header_def.h"

#ifdef _WIN32
#include "../include/win32_header_def.h"
typedef HANDLE ThreadHandle;
//typedef DWORD (ThreadFun) (void*);
#else
#include<pthread.h>
typedef pthread_t ThreadHandle;
#endif

typedef void (ThreadFun)(void*);

namespace jxsthread
{
inline ThreadHandle TCreateThread(ThreadFun* fun, void* param)
{
#ifdef _WIN32
	return ::CreateThread(NULL, 0, (DWORD (_stdcall *)(void*))fun, param, 0, NULL);
#else
	ThreadHandle h = {0};
	//if (0 != pthread_create(&h, NULL, (void* (*)(void*))fun, param))
	if (0 != pthread_create(&h, NULL, (void* (*)(void*))fun, param))
	{
		printf("creat thread error \n");
		return pthread_t(0);
	}
	return h;
#endif
}

inline int TCloseThread(ThreadHandle handle)
{
#ifdef _WIN32
	return ::TerminateThread(handle, 0);
#else
	return pthread_detach(handle);
#endif
}		

}




#endif