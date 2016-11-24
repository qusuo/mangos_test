#ifndef JXS_MODULEINTERFACE_H_
#define JXS_MODULEINTERFACE_H_
//author:hsj
//受进程循环驱动的模块

#include "Platform/Define.h"

namespace jxsmodule
{
//class ModuleManager;
class Module
{
public:
	Module()/*:m_owner(NULL)*/{}
	virtual ~Module(){};

	virtual int32 Init()=0;
	virtual int32 Start()=0;
	virtual int32 Update()=0;
	virtual int32 UnInit()=0;

private:
	//ModuleManager* m_owner;
};

}

#endif