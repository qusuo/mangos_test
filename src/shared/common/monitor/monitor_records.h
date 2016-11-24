#ifndef JXS_MONITORRECORDERS_H_
#define JXS_MONITORRECORDERS_H_
//author:hsj
//监视记录

#include "../include/basic_type_def.h"
#include <string>

struct EffeciencyRecorder
{
	EffeciencyRecorder()
		:total_time_len(0)
		, execute_times(0)
		, max_time_len(0)
		, min_time_len(0)
	{

	}
	std::string recorder_name;
	Int64 total_time_len;	//总时长
	Int64 execute_times;	//执行次数
	Int64 max_time_len;		//最大时长
	Int64 min_time_len;		//最小时长
};



#endif