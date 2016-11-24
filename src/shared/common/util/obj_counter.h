#ifndef JXS_OBJCOUNTER_H_
#define JXS_OBJCOUNTER_H_
//简易计数器
#include "../include/basic_type_def.h"

class ObjCounter
{
public:
	ObjCounter();
	~ObjCounter();

	void IncreaseObj(const char* obj_name);
	void DecreaseObj(const char* obj_name);
	void PrintReport();

private:
	typedef std::map<std::string, Int64> CounterMap;
	CounterMap m_counters;
};


#endif