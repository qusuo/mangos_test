#ifndef JXS_MONITORMANAGER_H_
#define JXS_MONITORMANAGER_H_
//author:hsj
//监视管理器

#include "monitor_records.h"
#include <map>
typedef std::map<std::string, EffeciencyRecorder> EffRecorders;
typedef EffRecorders::iterator EffRecordersItr;

class MonitorMgr
{
public:	
	~MonitorMgr();
	static MonitorMgr& Singleton();

	void AddEffRecord(const std::string& name, Int64 elapse_time);
	void Print();

	EffRecordersItr GetBegin() { return m_eff_recorders.begin(); }
	EffRecordersItr GetEnd() { return m_eff_recorders.end(); }

private:
	MonitorMgr();

private:
	

private:
	EffRecorders m_eff_recorders;
};

#define g_monitor_mgr MonitorMgr::Singleton()



#endif