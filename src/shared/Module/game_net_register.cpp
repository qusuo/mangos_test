#include "game_net_register.h"

jxsgame::jxsgame()
{

}

jxsgame::~jxsgame()
{

}

bool jxsgame::RegisterStopper(const wchar_t* wnd_class_name, const wchar_t* wnd_name)
{
	return false;
}

bool jxsgame::RegisterNet(jxsnet::NetParam& net_param)
{
	return false;
}

bool jxsgame::RegisterModule(const char* module_name, jxsmodule::Module* module)
{
	return false;
}

jxsmodule::Module* jxsgame::QueryModule(const char* module_name)
{
	return NULL;
}

void jxsgame::RunGame()
{

}

void jxsgame::StopGame()
{

}
