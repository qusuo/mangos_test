#ifndef JXS_MODULEINTERFACE_H_
#define JXS_MODULEINTERFACE_H_
//author:hsj
//受进程循环驱动的模块

#include "../include/basic_type_def.h"

namespace jxsmodule
{
//class ModuleManager;
class Module
{
public:
	Module()/*:m_owner(NULL)*/{}
	virtual ~Module(){};

	virtual Int32 Init()=0;
	virtual Int32 Start()=0;
	virtual Int32 Update()=0;
	virtual Int32 UnInit()=0;

private:
	//ModuleManager* m_owner;
};
}

#endif