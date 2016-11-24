#include "auto_efficiency_monitor.h"
#include "../time/time_api.h"
#include "monitor_manager.h"

AutoEfficiencyMonitor::AutoEfficiencyMonitor(const char* monitor_name)
{
	m_name = monitor_name;
	m_start_time = jxstime::PIMSTime();
}

AutoEfficiencyMonitor::~AutoEfficiencyMonitor()
{
	Int64 diff_time = jxstime::PIMSTime() - m_start_time;
	g_monitor_mgr.AddEffRecord(m_name, diff_time);
}
