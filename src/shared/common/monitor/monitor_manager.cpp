#include "monitor_manager.h"

MonitorMgr::MonitorMgr()
{
}

MonitorMgr::~MonitorMgr()
{
}

void MonitorMgr::AddEffRecord(const std::string& name, Int64 elapse_time)
{
	EffRecorders::iterator itr = m_eff_recorders.find(name);
	if (itr != m_eff_recorders.end())
	{
		itr->second.total_time_len += elapse_time;
		++(itr->second.execute_times);
		if (elapse_time < itr->second.min_time_len)
		{
			itr->second.min_time_len = elapse_time;
		}
		else if (elapse_time > itr->second.max_time_len)
		{
			itr->second.max_time_len = elapse_time;
		}
	}
	else
	{
		EffeciencyRecorder tmp_recorder;
		tmp_recorder.recorder_name = name;
		tmp_recorder.total_time_len = elapse_time;
		tmp_recorder.execute_times = 1;
		tmp_recorder.min_time_len = elapse_time;
		tmp_recorder.max_time_len = elapse_time;
		m_eff_recorders[name] = tmp_recorder;
	}
}

MonitorMgr& MonitorMgr::Singleton()
{
	static MonitorMgr instance;
	return instance;
}

void MonitorMgr::Print()
{
	printf("============ report  monitor records ============= \n");
	EffRecorders::iterator itr_end = m_eff_recorders.end();
	for (EffRecorders::iterator itr = m_eff_recorders.begin(); itr != itr_end; ++itr)
	{
		printf("name[%s], total_time_len[%lld], execute_times[%lld] \n", 
			itr->second.recorder_name.c_str(), itr->second.total_time_len, itr->second.execute_times);
	}
}
