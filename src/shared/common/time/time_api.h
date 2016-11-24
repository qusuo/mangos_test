#ifndef JXS_TIMEAPI_H_
#define JXS_TIMEAPI_H_
#include "../include/basic_type_def.h"
#include "day_id.h"
//author:hsj
//包装一些时间操作

namespace jxstime
{
	//调时间 只对秒钟以上的时间起作用
	void SetTimeGap(Int32 time_gap);
	Int32 GetTimeGap();

	//精确到秒，是当前时间和1970年1月1日零点时间的差
	Int64 PISTime();
	//毫秒
	Int64 PIMSTime();
	//微秒,操作系统启动到现在流失的微秒
	Int64 PIUSTime();
	//微秒比较
	Int64 PIUSTimeDiff(Int64 old_ustime, Int64 new_ustime);

	//一年的不同的id，区间[0-365]
	Int32 PIDayOfYear();
	DayID PIDayId();

	//获取时间信息, 如果time_secs为0， 则取当前时间 （周日是0）
	bool GetLocalTime(Int64 time_secs, Int16 *year, Int8 *month, Int8 *day, Int8 *hour, Int8 * minute, Int8 *second, Int8 *dayofweek);
	Int64 MakeTime(Int16 year, Int8 month, Int8 day, Int8 hour, Int8 minute, Int8 secod);//获取特定时间的时候戳

	/*常用操作*/
	//取某个时间点当周周n的0点时间戳
	Int64 GetZeroHourTimeOfWeakDay(Int64 time_stamp, Int8 weak_day);
	//取时间格式字符串YYYY/MM/DD/hh:mm:ss
	const char* GetLocalTimeFormat(Int64 time_stamp);

}

#endif