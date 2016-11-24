#ifndef JXS_MODULEMANAGERINTERFACE_H_
#define JXS_MODULEMANAGERINTERFACE_H_
//author:hsj
//进程循环

#include "../include/basic_type_def.h"

namespace jxsmodule
{
class Module;
class ModuleManager
{
public:
	ModuleManager(){}
	virtual ~ModuleManager(){}
	virtual bool RegisterModule(const char* name, Module *module)=0;
	virtual Module* QueryModule(const char* name)=0;
	virtual void Run() = 0;
};
}

#endif