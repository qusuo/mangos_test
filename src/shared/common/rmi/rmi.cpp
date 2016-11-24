#include "rmi-interface.h"
#include "../time/time_api.h"

RmiCallBackObj::RmiCallBackObj()
{
	m_create_time = jxstime::PISTime();
}

RmiCallBackObj::~RmiCallBackObj()
{
	m_create_time = 0;
}

bool RmiCallBackObj::IsExpired()
{
	const Int32 EXPEIRED_TIME_LEN = 10; //10秒
	if (jxstime::PISTime() - m_create_time > EXPEIRED_TIME_LEN)
	{
		return true;
	}
	else
	{
		return false;
	}
}