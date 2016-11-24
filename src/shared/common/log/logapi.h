#ifndef JXS_LOGAPI_H_
#define JXS_LOGAPI_H_
//author:hsj
#include "log_def.h"
#include "logger.h"
#include "../time/time_api.h"

class LogApi
{
public:
	~LogApi();
	static LogApi& Singleton();

	/**工具方法**/
	void PrintfFlush(const char* format, ...);
	//注意只支持1024长度
	const char* QuickLogInfo(const char* format, ...); 

	//目录结束处不带"/"
	bool InitSysLog(const char* dir, const char* fla_name);
	Logger* GetSysLog() { return m_sys_log; }

private:
	LogApi();
	Logger* m_sys_log;

};

#define g_log_api LogApi::Singleton()
#define SYS_LOG(format,...) g_log_api.GetSysLog()->WriteLogWithTime("[%s %d]"format,__FILE__, __LINE__, ##__VA_ARGS__);
#define DEBUG_LOG(format,...) g_log_api.PrintfFlush("[%s]: "format, jxstime::GetLocalTimeFormat(0), ##__VA_ARGS__);
#define DEBUG_LOG_D(format,...) g_log_api.PrintfFlush("[%s %s %d]: "format, jxstime::GetLocalTimeFormat(0), __FILE__, __LINE__, ##__VA_ARGS__);

#endif