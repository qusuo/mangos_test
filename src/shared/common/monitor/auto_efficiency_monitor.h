#ifndef JXS_EFFICIENCYMONITOR_H_
#define JXS_EFFICIENCYMONITOR_H_
//author:hsj
//运行效率监视器

#include "../include/basic_type_def.h"
#include <string>

class AutoEfficiencyMonitor
{
public:
	AutoEfficiencyMonitor(const char* monitor_name);
	~AutoEfficiencyMonitor();

private:
	std::string m_name;
	Int64 m_start_time;
	//Int64 m_end_time;
};



#endif