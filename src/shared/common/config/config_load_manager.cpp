#include "config_load_manager.h"
#include "../include/header_def.h"
ConfigLoadMgr::ConfigLoadMgr()
{

}

ConfigLoadMgr::~ConfigLoadMgr()
{
	ConfigNameMap::iterator itr_end = m_name_map.end();
	for (ConfigNameMap::iterator itr = m_name_map.begin(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}
}

bool ConfigLoadMgr::Init()
{
	bool load_res = true;
	ConfigList::iterator itr_pre = m_config_list.begin();
	ConfigList::iterator itr_end = m_config_list.end();
	for (; itr_pre != itr_end && load_res; ++itr_pre)
	{
		ConfigParam* cfg_param = *itr_pre;
		if (NULL != cfg_param && NULL != cfg_param->config)
		{
			//printf("%s \n", cfg_param->config_path.c_str());
			load_res &= cfg_param->config->Init(cfg_param->config_path.c_str());
			if (false == load_res)
			{
				printf("%s false \n", cfg_param->config_path.c_str());
				return false;
			}

		}
	}
	return load_res;
}

bool ConfigLoadMgr::HotLoad()
{
	bool pre_load_res = true;
	ConfigList::iterator itr_pre = m_config_list.begin();
	ConfigList::iterator itr_end = m_config_list.end();
	for (; itr_pre != itr_end && pre_load_res; ++itr_pre)
	{
		ConfigParam* cfg_param = *itr_pre;
		if (NULL != cfg_param && NULL != cfg_param->config)
		{
			pre_load_res &= cfg_param->config->PreLoad();
		}
	}

	if (pre_load_res)
	{
		for (ConfigList::iterator itr = m_config_list.begin(); itr != itr_end && pre_load_res; ++itr)
		{
			ConfigParam* cfg_param = *itr_pre;
			if (NULL != cfg_param && NULL != cfg_param->config)
			{
				pre_load_res &= cfg_param->config->ConformPreLoad();
			}
		}
	}
	else
	{
		for (ConfigList::iterator itr = m_config_list.begin(); itr != itr_pre && pre_load_res; ++itr)
		{
			ConfigParam* cfg_param = *itr_pre;
			if (NULL != cfg_param && NULL != cfg_param->config)
			{
				pre_load_res &= cfg_param->config->RollBackPreLoad();
			}
		}
	}

	return pre_load_res;
}

bool ConfigLoadMgr::AddConfig(const char* cfg_name, const char* cfg_path, jxs::Config* cfg)
{
	if (NULL == cfg_name || NULL == cfg_path || NULL == cfg)
	{
		return false;
	}

	if (NULL != GetConfig(cfg_name))
	{
		return false;
	}

	ConfigParam* tmp_param = new ConfigParam();
	tmp_param->config_name = cfg_name;
	tmp_param->config_path = cfg_path;
	tmp_param->config = cfg;

	m_name_map[cfg_name] = tmp_param;
	m_config_list.push_back(tmp_param);
	return true;
}

void ConfigLoadMgr::RemoveConfig(const char* cfg_name)
{
	if (NULL == cfg_name)
	{
		return;
	}

	ConfigNameMap::iterator itr_m = m_name_map.find(cfg_name);
	for (ConfigList::iterator itr_l = m_config_list.begin(); itr_l != m_config_list.end();)
	{
		if (*itr_l == itr_m->second)
		{
			ConfigList::iterator  tmp_itr = itr_l;
			++itr_l;
			m_config_list.erase(tmp_itr);
		}
		else
		{
			++itr_l;
		}
	}

	if (itr_m != m_name_map.end())
	{
		delete itr_m->second;
		m_name_map.erase(itr_m);
	}
}

jxs::Config* ConfigLoadMgr::GetConfig(const char* cfg_name)
{
	if (NULL == cfg_name)
	{
		return NULL;
	}

	ConfigNameMap::iterator itr = m_name_map.find(cfg_name);
	if (itr == m_name_map.end())
	{
		return NULL;
	}
	else
	{
		return itr->second->config;
	}
}
