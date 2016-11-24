#include "obj_counter.h"
#include "../log/logapi.h"

ObjCounter::ObjCounter()
{

}

ObjCounter::~ObjCounter()
{

}

void ObjCounter::IncreaseObj(const char* obj_name)
{
	CounterMap::iterator itr = m_counters.find(obj_name);
	if (itr == m_counters.end())
	{
		m_counters[obj_name] = 1;
	}
	else
	{
		++itr->second;
	}
}

void ObjCounter::DecreaseObj(const char* obj_name)
{
	CounterMap::iterator itr = m_counters.find(obj_name);
	if (itr == m_counters.end())
	{
		DEBUG_LOG_D("decrease unexist obj[%s]", obj_name);
		return;
	}
	else
	{
		if (itr->second <= 0)
		{
			DEBUG_LOG_D("decrease empty obj[%s]", obj_name);
			return;
		}
		else
		{
			--(itr->second);
		}	
	}
}

void ObjCounter::PrintReport()
{
	CounterMap::iterator itr = m_counters.begin();
	CounterMap::iterator itr_end = m_counters.end();
	for (; itr != itr_end; ++itr)
	{
		if (itr->second > 0)
		{
			DEBUG_LOG("obj_name:%s, obj_num:%lld", itr->first.c_str(), itr->second);
		}
	}
}
