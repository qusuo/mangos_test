#include "logapi.h"
#include "../include/header_def.h"
#include "../thread/thread_lock.h"
#include "../time/time_api.h"

LogApi::LogApi()
	:m_sys_log(NULL)
{

}

LogApi::~LogApi()
{
	if (NULL != m_sys_log)
	{
		delete m_sys_log;
	}
}

LogApi& LogApi::Singleton()
{
	static LogApi instance;
	return instance;
}

void LogApi::PrintfFlush(const char* format, ...)
{
	//jxsthread::AutoThreadLock auto_lock(log_locker);
	char buffer[1000 * 4] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, args);
	va_end(args);
	printf("%s\n", buffer);
	fflush(stdout);
	fflush(stderr);
}

const char* LogApi::QuickLogInfo(const char* format, ...)
{
	static char buffer[1024] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, args);
	va_end(args);
	printf(buffer);
	return buffer;
}

bool LogApi::InitSysLog(const char* dir, const char* fla_name)
{
	if (NULL != m_sys_log)
	{
		return false;
	}

	m_sys_log = new Logger();

	Int64 cur_time = jxstime::PISTime();
	char file_name_buff[MAX_256_CHAR_LEN] = { 0 };
	bool res = true;
	sprintf(file_name_buff, "%s/%s_%lld.log", dir, fla_name, cur_time);
	res &= m_sys_log->Init(file_name_buff, true);
	return res;
}

