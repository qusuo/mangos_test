#include "utf8_str_config.h"
#include "../../external/json/jsonapi.h"

Utf8StrConfig::Utf8StrConfig()
{

}

Utf8StrConfig::~Utf8StrConfig()
{

}

Utf8StrConfig& Utf8StrConfig::Singleton()
{
	static Utf8StrConfig instance;
	return instance;
}

bool Utf8StrConfig::Init(const char* file_path)
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

bool Utf8StrConfig::LoadFromFile(const char* file_path)
{
	Json::Value root;
	std::string err;
	bool res = jxsjson::LoadFromFile(file_path, root, err);
	if (!res)
	{
		printf("LoadFromFile[%s] error\n", file_path);
		return false;
	}

	Json::Value tmp_value;
	if (!jxsjson::GetSubKeyValue(root, "ore_base_name", tmp_value))
	{
		printf("LoadFromFile[%s]  ore_base_name error \n", file_path);
		return false;
	}

	for (Json::Value::UInt cfg_index = 0; cfg_index < tmp_value.size() && cfg_index < RESOURCE_TYPE_END; ++cfg_index)
	{
		m_ore_base_name[cfg_index] = tmp_value[cfg_index].asString();
	}

	if (!jxsjson::GetSubKeyValue(root, "ore_sprintf_str", m_ore_sprintf_str))
	{
		printf("LoadFromFile[%s]  ore_base_name error:%s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "mail_sender", m_mail_sender_name))
	{
		printf("LoadFromFile[%s]  mail_sender error:%s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "attack_title", m_attack_title))
	{
		printf("LoadFromFile[%s]  attack_title error:%s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "be_attack_title", m_be_attack_title))
	{
		printf("LoadFromFile[%s]  be_attack_title error:%s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "spy_title", m_spy_title))
	{
		printf("LoadFromFile[%s]  spy_title error:%s \n", file_path, err.c_str());
		return false;
	}
	return true;
}

bool Utf8StrConfig::PreLoad()
{
	for (Int32 i = 0; i < RESOURCE_TYPE_COUNT; ++i)
	{
		m_ore_base_name_bak[i] = m_ore_base_name[i];
	}
	m_ore_sprintf_str_bak = m_ore_sprintf_str;
	m_mail_sender_name_bak = m_mail_sender_name;
	m_attack_title_bak = m_attack_title;
	m_be_attack_title_bak = m_be_attack_title;
	m_spy_title_bak = m_spy_title;

	return LoadFromFile(m_file_path.c_str());
}

bool Utf8StrConfig::ConformPreLoad()
{
	for (Int32 i = 0; i < RESOURCE_TYPE_COUNT; ++i)
	{
		m_ore_base_name_bak[i].clear();
	}

	m_ore_sprintf_str_bak.clear();

	return true;
}

bool Utf8StrConfig::RollBackPreLoad()
{
	for (Int32 i = 0; i < RESOURCE_TYPE_COUNT; ++i)
	{
		m_ore_base_name[i] = m_ore_base_name_bak[i];
	}
	m_ore_sprintf_str = m_ore_sprintf_str_bak;
	m_mail_sender_name = m_mail_sender_name_bak;
	m_attack_title = m_attack_title_bak;
	m_be_attack_title = m_be_attack_title_bak;
	m_spy_title = m_spy_title_bak;

	return true;
}

const std::string& Utf8StrConfig::GetOreBaseName(Int32 resource_type)
{
	if (resource_type <= RESOURCE_TYPE_INVALID || resource_type >= RESOURCE_TYPE_END)
	{
		static std::string tmp_ret = "";
		return tmp_ret;
	}
	else
	{
		return m_ore_base_name[resource_type - 1];
	}
}