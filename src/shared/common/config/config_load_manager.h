#ifndef JXS_CONFIGLOADMANAGER_H_
#define JXS_CONFIGLOADMANAGER_H_
//配置管理加载管理器
#include "config-interface.h"
#include <vector>
#include <map>
#include <string>

class ConfigLoadMgr
{
public:
	ConfigLoadMgr();
	~ConfigLoadMgr();

	bool Init();
	bool HotLoad();
	//添加配置应该按顺序来
	bool AddConfig(const char* cfg_name, const char* cfg_path, jxs::Config* cfg);
	void RemoveConfig(const char* cfg_name);

private:
	jxs::Config* GetConfig(const char* cfg_name);

private:
	struct ConfigParam
	{
		ConfigParam()
			:config(NULL)
		{

		}
		std::string config_name;
		std::string config_path;
		jxs::Config* config;
	};

private:
	typedef std::vector<ConfigParam*> ConfigList;
	typedef std::map<std::string, ConfigParam*> ConfigNameMap;

	ConfigList m_config_list;
	ConfigNameMap m_name_map;
};

#endif