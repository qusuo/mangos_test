#ifndef JXS_LOGGER_H_
#define JXS_LOGGER_H_
//author:hsj
//日志wrapter

#include "../include/basic_type_def.h"
#include "../include/value_def.h"
#include "log_def.h"
#include <string>


enum Logger_Level
{
	LOGGER_ERROR,
	LOGGER_WARNING,
};

class Logger
{
public:
	Logger();
	~Logger();
	bool Init(const char* log_file, bool to_console);
	void WriteLog(const char* format, ...);
	void WriteLog(Int32 log_level, const char* format, ...);
	void WriteLogFlush(const char* format, ...);
	void WriteLogFlush(Int32 log_level, const char* format, ...);
	void WriteLogWithTime(const char* format, ...);

private:
	const char* LogTime();


private:
	std::string m_file_dir;
	bool m_to_console;
	FILE* m_file;

	static char m_buff[MAX_4K_BUFFER_LEN];

};

#endif