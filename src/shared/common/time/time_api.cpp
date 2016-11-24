#include "time_api.h"

#ifdef _WIN32
#include <time.h>
#include <Windows.h>
#else
#include <sys/time.h>
#endif


namespace jxstime
{
	Int32 g_time_stamp_gap = 0;

	void SetTimeGap(Int32 time_gap)
	{
		g_time_stamp_gap = time_gap;
	}

	Int32 GetTimeGap()
	{
		return g_time_stamp_gap;
	}

	//精确到秒，是当前时间和1970年1月1日零点时间的差
	Int64 PISTime()
	{
		time_t lt;
		lt =time(NULL);
		return lt + g_time_stamp_gap;
	}

	//毫秒
	Int64 PIMSTime()
	{
#ifdef _WIN32
		SYSTEMTIME systime;
		GetSystemTime(&systime);
		time_t second = time(NULL);
		return (second * 1000 + systime.wMilliseconds);
#else
		struct timeval tv;
		struct timezone tz;
		gettimeofday( &tv, &tz );
		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
	}

	//微秒,操作系统启动到现在流失的微秒
	Int64 PIUSTime()
	{
#ifdef _WIN32
		return GetTickCount();//fix(这是错的，应该用cpc时钟频率去做)
#else
		struct timeval tv;
		struct timezone tz;
		gettimeofday( &tv, &tz );
		return tv.tv_sec * 1000000 + tv.tv_usec;
#endif
	}

	//微秒比较
	Int64 PIUSTimeDiff(Int64 old_ustime, Int64 new_ustime)
	{
		// getMSTime() have limited data range and this is case when it overflow in this tick
		if (old_ustime > new_ustime) 
		{
			return (0xFFFFFFFF - old_ustime) + new_ustime;
		}
		else
		{
			return new_ustime - old_ustime;
		}
	}

	//一年的不同的id，区间[0-365]
	Int32 PIDayOfYear()
	{
		Int32 dayid = 0;

#ifdef _WIN32
		time_t cur_time = PISTime();
		//time(&cur_time);  
		tm cur_tm;
		localtime_s(&cur_tm, &cur_time);
		dayid = cur_tm.tm_yday;
#else
		time_t lt = PISTime();
		//lt =time(NULL);
		struct tm* ct = localtime(&lt);
		dayid = ct->tm_yday;
#endif
		return dayid;
	}

	DayID PIDayId()
	{
		Int64 cur_time = PISTime();
		return DayID(cur_time);
	}

	//获取时间信息
	bool GetLocalTime(Int64 time_secs, Int16 *year, Int8 *month, Int8 *day, Int8 *hour, Int8 * minute, Int8 *second, Int8 *dayofweek)
	{
		time_t cur_time = time_secs > 0 ? time_secs : PISTime();
#ifdef _WIN32
		tm lt;
		localtime_s(&lt, &cur_time);
		year == NULL ? 0 : *year = lt.tm_year + 1900;
		month == NULL ? 0 : *month = lt.tm_mon + 1;
		day == NULL ? 0 : *day = lt.tm_mday;
		hour == NULL ? 0 : *hour = lt.tm_hour;
		minute == NULL ? 0 : *minute = lt.tm_min;
		second == NULL ? 0 : *second = lt.tm_sec;
		dayofweek == NULL ? 0 : *dayofweek = lt.tm_wday;
#else
		struct tm* lt = localtime(&cur_time);
		year == NULL ? 0 : *year = lt->tm_year + 1900;
		month == NULL ? 0 : *month = lt->tm_mon + 1;
		day == NULL ? 0 : *day = lt->tm_mday;
		hour == NULL ? 0 : *hour = lt->tm_hour;
		minute == NULL ? 0 : *minute = lt->tm_min;
		second == NULL ? 0 : *second = lt->tm_sec;
		dayofweek == NULL ? 0 : *dayofweek = lt->tm_wday;
#endif
		return true;
	}

	//获取特定时间的时候戳
	Int64 MakeTime(Int16 year, Int8 month, Int8 day, Int8 hour, Int8 minute, Int8 secod)
	{
		//取得从1970/01/01到目标日期的秒数
		struct tm t;
		t.tm_year	= year - 1900;
		t.tm_mon	= month - 1;
		t.tm_mday	= day;
		t.tm_hour	= hour;
		t.tm_min	= minute;
		t.tm_sec	= secod;

		return mktime(&t);
	}

	Int64 GetZeroHourTimeOfWeakDay(Int64 time_stamp, Int8 weak_day)
	{
		const Int32 DAY_SECOND_NUM = 3600 * 24;
		Int16 cur_year = 0;
		Int8 cur_month = 0;
		Int8 cur_day = 0;
		Int8 cur_weak_day = -1;
		if (!GetLocalTime(time_stamp, &cur_year, &cur_month, &cur_day, NULL, NULL, NULL, &cur_weak_day))
		{
			return 0;
		}
		Int8 diff = cur_weak_day - weak_day;

		Int64 tmp_time = time_stamp + diff * DAY_SECOND_NUM;
		if (!GetLocalTime(tmp_time, &cur_year, &cur_month, &cur_day, NULL, NULL, NULL, NULL))
		{
			return 0;
		}
		return MakeTime(cur_year, cur_month, cur_day, 0, 0, 0);
	}

	const char* GetLocalTimeFormat(Int64 time_stamp)
	{
		Int16 year = 0;
		Int8 month = 0;
		Int8 day = 0;
		Int8 hour = 0;
		Int8 minit = 0;
		Int8 second = 0;
		GetLocalTime(time_stamp, &year, &month, &day, &hour, &minit, &second, NULL);
		static char buff[128] = { 0 };
		sprintf(buff, "%d/%02d/%02d/%02d:%02d:%02d", year, month, day, hour, minit, second);

		return buff;
	}

}//end namespace jxsmem
