#ifndef JXS_EFFICIENCYMONITOR_H_
#define JXS_EFFICIENCYMONITOR_H_
//author:hsj
//运行效率监视器

#include "../include/basic_type_def.h"
#include <string>

class EfficiencyMonitor
{
public:
	EfficiencyMonitor(const char* monitor_name);
	~EfficiencyMonitor();

private:
	std::string m_name;
	Int64 m_start_time;
	//Int64 m_end_time;
};



#endif