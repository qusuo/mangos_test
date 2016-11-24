#ifndef JXS_GAMENETREGISTER_H_
#define JXS_GAMENETREGISTER_H_
#include "module-interface.h"
#include "module_manager-interface.h"

namespace jxsnet
{
	struct NetParam;
}

namespace jxsgame
{
	bool RegisterStopper(const wchar_t* wnd_class_name, const wchar_t* wnd_name);
	bool RegisterNet(jxsnet::NetParam& net_param);
	bool RegisterModule(const char* module_name, jxsmodule::Module* module);
	jxsmodule::Module* QueryModule(const char* module_name);
	void RunGame();
	void StopGame();
}

#endif