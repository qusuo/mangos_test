#include "server_base_config.h"
#include "../../external/json/jsonapi.h"
#include "../../config/config_api.h"
#include "../../time/time_api.h"

ServerBaseConfig::ServerBaseConfig()
	:m_open_server_time(0)
	, m_server_id(0)
	, m_plat_id(0)
	, m_is_anti_addiction_opened(0)
	, m_anti_addiction_cut_time_len(0)
	, m_anti_addiction_out_time_len(0)
	, m_open_server_time_bak(0)
	, m_server_id_bak(0)
	, m_plat_id_bak(0)
	, m_is_anti_addiction_opened_bak(0)
	, m_anti_addiction_cut_time_len_bak(0)
	, m_anti_addiction_out_time_len_bak(0)
{
}

ServerBaseConfig::~ServerBaseConfig()
{

}

ServerBaseConfig& ServerBaseConfig::Singleton()
{
	static ServerBaseConfig instance;
	return instance;
}

bool ServerBaseConfig::Init(const char* file_path)
{
	if (!LoadFromFile(file_path))
	{
		return false;
	}
	else
	{
		m_file_path = file_path;
		return true;
	}
}

bool ServerBaseConfig::LoadFromFile(const char* file_path)
{
	Json::Value root;
	std::string err;
	bool res = jxsjson::LoadFromFile(file_path, root, err);
	if (!res)
	{
		printf("LoadFromFile[%s] error: %s \n", file_path, err.c_str());
		return false;
	}

	Json::Value tmp_value;
	std::string time_str;
	if (!jxsjson::GetSubKeyValue(root, "open_server_time", time_str))
	{
		printf("LoadFromFile[%s]  open_server_time:%s error  \n", file_path, time_str.c_str());
		return false;
	}
	m_open_server_time = configapi::ParseFormatTime(time_str);
	if (m_open_server_time <= 0)
	{
		printf("LoadFromFile[%s]  open_server_time:%s error  \n", file_path, time_str.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "server_id", m_server_id)
		|| m_open_server_time <= 0)
	{
		printf("LoadFromFile[%s]  server_id:%d error  \n", file_path, m_server_id);
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "plat_id", m_plat_id)
		|| m_open_server_time <= 0)
	{
		printf("LoadFromFile[%s]  plat_id:%d error  \n", file_path, m_plat_id);
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "is_anti_addiction_opened", m_is_anti_addiction_opened)
		|| m_is_anti_addiction_opened < 0)
	{
		printf("LoadFromFile[%s]  is_anti_addiction_opened:%d error  \n", file_path, m_is_anti_addiction_opened);
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "anti_addiction_cut_time_len", m_anti_addiction_cut_time_len)
		|| m_anti_addiction_cut_time_len <= 0)
	{
		printf("LoadFromFile[%s]  anti_addiction_cut_time_len:%d error  \n", file_path, m_anti_addiction_cut_time_len);
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "anti_addiction_out_time_len", m_anti_addiction_out_time_len)
		|| m_anti_addiction_out_time_len <= 0)
	{
		printf("LoadFromFile[%s]  anti_addiction_out_time_len:%d error  \n", file_path, m_anti_addiction_out_time_len);
		return false;
	}

	return true;
}

bool ServerBaseConfig::PreLoad()
{
	m_open_server_time_bak = m_open_server_time;
	m_server_id_bak = m_server_id;
	m_plat_id_bak = m_plat_id;
	m_is_anti_addiction_opened_bak = m_is_anti_addiction_opened;
	m_anti_addiction_cut_time_len_bak = m_anti_addiction_cut_time_len;
	m_anti_addiction_out_time_len_bak = m_anti_addiction_out_time_len;

	return LoadFromFile(m_file_path.c_str());
}

bool ServerBaseConfig::ConformPreLoad()
{
	return true;
}

bool ServerBaseConfig::RollBackPreLoad()
{
	m_open_server_time = m_open_server_time_bak;
	m_server_id = m_server_id_bak;
	m_plat_id = m_plat_id_bak;
	m_is_anti_addiction_opened = m_is_anti_addiction_opened_bak;
	m_anti_addiction_cut_time_len = m_anti_addiction_cut_time_len_bak;
	m_anti_addiction_out_time_len = m_anti_addiction_out_time_len_bak;
	return true;
}

Int32 ServerBaseConfig::GetOpenServerEscapeDay()
{
	 return Int32((jxstime::PISTime() - GetOpenServerTime()) / (24 * 60 * 60));
}


