#include "efficiency_monitor.h"
#include "../time/time_api.h"
#include "monitor_manager.h"

EfficiencyMonitor::EfficiencyMonitor(const char* monitor_name)
{
	m_name = monitor_name;
	m_start_time = jxstime::PIMSTime();
}

EfficiencyMonitor::~EfficiencyMonitor()
{
	Int64 diff_time = jxstime::PIMSTime() - m_start_time;
	g_monitor_mgr.AddEffRecord(m_name, diff_time);
}
