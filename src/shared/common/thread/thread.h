#ifndef JXS_THREAD_H_
#define JXS_THREAD_H_
#include "../include/basic_type_def.h"
#include "thread_def.h"
//author:hsj
//用于线程池的线程

namespace jxsthread
{
class Thread
{
public:
	Thread();
	virtual ~Thread();

	//这里会创建系统线程
	void Run();
	void Stop();
	//void Pause();

	bool IsRunning() { return m_thread_handle != 0; }

	//线程实际处理函数
	virtual void ThreadAction() = 0;

private:
	//被系统线程调用的函数
static void ThreadProcess(void* thread);	

protected:
	ThreadHandle m_thread_handle;
	bool m_is_stop;
};



}

#endif