#include "logger.h"
#include "../time/time_api.h"

char Logger::m_buff[];

Logger::Logger()
	:m_to_console(false)
	, m_file(NULL)
{
}

Logger::~Logger()
{
	if (NULL != m_file)
	{
		fclose(m_file);
	}
}

bool Logger::Init(const char* log_file, bool to_console)
{
	if (NULL != m_file || NULL == log_file)
	{
		return false;
	}

	m_file = fopen(log_file, "w");
	if (NULL == m_file)
	{
		printf("open file[%s] fail \n", log_file);
		return false;
	}

	m_file_dir = log_file;
	m_to_console = to_console;
	return true;
}

void Logger::WriteLog(const char* format, ...)
{
	if (NULL == m_file)
	{
		return;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(m_buff, sizeof(m_buff) - 1, format, args);
	va_end(args);

	fprintf(m_file, "%s", m_buff);
	if (m_to_console)
	{
		printf("%s", m_buff);
	}
}

void Logger::WriteLog(Int32 log_level, const char* format, ...)
{
	if (NULL == m_file)
	{
		return;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(m_buff, sizeof(m_buff) - 1, format, args);
	va_end(args);

	fprintf(m_file, "%s\n", m_buff);
	if (m_to_console)
	{
		printf("%s\n", m_buff);
	}
}

void Logger::WriteLogFlush(const char* format, ...)
{
	if (NULL == m_file)
	{
		return;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(m_buff, sizeof(m_buff) - 1, format, args);
	va_end(args);

	//Int32 len = strlen(m_buff);
	//m_buff[len] = '\n';


	//fwrite(m_buff, 1, len + 1, m_file);
	fprintf(m_file, "%s\n", m_buff);
	if (m_to_console)
	{
		printf("%s\n", m_buff);
	}

	fflush(m_file);
}

void Logger::WriteLogFlush(Int32 log_level, const char* format, ...)
{
	if (NULL == m_file)
	{
		return;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(m_buff, sizeof(m_buff) - 1, format, args);
	va_end(args);

	fprintf(m_file, "%s\n", m_buff);
	if (m_to_console)
	{
		printf("%s\n", m_buff);
	}

	fflush(m_file);
}

void Logger::WriteLogWithTime(const char* format, ...)
{
	if (NULL == m_file)
	{
		return;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(m_buff, sizeof(m_buff) - 1, format, args);
	va_end(args);

#ifdef _WIN32
	fprintf(m_file, "[%s]:%s \n", LogTime(), m_buff);
#else
	fprintf(m_file, "[%s]:%s \n", LogTime(), m_buff);
#endif

	if (m_to_console)
	{
		printf("%s\n", m_buff);
	}
}

const char* Logger::LogTime()
{
	Int16 year = 0;
	Int8 month = 0;
	Int8 day = 0;
	Int8 hour = 0;
	Int8 minute = 0;
	Int8 second = 0;
	Int8 dayofweek = 0;

	jxstime::GetLocalTime(0, &year, &month, &day, &hour, &minute, &second, &dayofweek);
	static char szDay[25];
	Snprintf(szDay, 25, "%4d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
	return szDay;
}


