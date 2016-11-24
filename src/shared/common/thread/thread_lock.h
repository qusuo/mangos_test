#ifndef JXS_THREADLOCK_H_
#define JXS_THREADLOCK_H_
//ahthor: hsj
//

#ifdef _WIN32
#include "../include/win32_header_def.h"
#define LockHandle CRITICAL_SECTION 
#else
#include <pthread.h> 
#define LockHandle pthread_mutex_t 
#endif


namespace jxsthread
{

//线程锁，windows下用临界区，linux下用互斥量实现
class ThreadLock
{
public:
	ThreadLock()
	{
#ifdef _WIN32
		InitializeCriticalSection(&m_handle);
#else
		pthread_mutexattr_t attr;
		pthread_mutexattr_init( &attr );
		pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE_NP );
		pthread_mutex_init( &m_handle, &attr );
#endif
	}

	~ThreadLock()
	{
#ifdef _WIN32
		DeleteCriticalSection(&m_handle);
#else
		pthread_mutex_destroy(&m_handle);
#endif
	}

	void Lock()
	{
		//AutoEfficiencyMonitor em("Lock");
#ifdef _WIN32
		EnterCriticalSection(&m_handle);
#else
		pthread_mutex_lock(&m_handle);
#endif
	}

	void UnLock()
	{
		//AutoEfficiencyMonitor em("UnLock");
#ifdef _WIN32
		LeaveCriticalSection(&m_handle);
#else
		pthread_mutex_unlock(&m_handle);
#endif
	}

private:
	LockHandle m_handle;
};


class AutoThreadLock
{
public:
	explicit AutoThreadLock(ThreadLock& thread_lock)
		:m_lock(thread_lock)
	{
		m_lock.Lock();
	}
	~AutoThreadLock()
	{
		m_lock.UnLock();
	}

private:
	ThreadLock& m_lock;
};


}

#endif