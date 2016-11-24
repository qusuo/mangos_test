#include "db_config.h"
#include "../../external/json/jsonapi.h"

DBConfig::DBConfig()
	:m_port(0)
	, m_port_bak(0)
	, m_server_id(0)
	, m_server_id_bak(0)
{

}

DBConfig::~DBConfig()
{

}

DBConfig& DBConfig::Singleton()
{
	static DBConfig instance;
	return instance;
}


bool DBConfig::Init(const char* file_path)
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

bool DBConfig::LoadFromFile(const char* file_path)
{
	Json::Value root;
	std::string err;
	bool res = jxsjson::LoadFromFile(file_path, root, err);
	if (!res)
	{
		printf("LoadFromFile[%s] error: %s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "ip", m_ip))
	{
		printf("LoadFromFile[%s]  ip error  code %s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "port", m_port))
	{
		printf("LoadFromFile[%s]  port error  code %s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "user", m_user))
	{
		printf("LoadFromFile[%s]  user error  code %s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "password", m_password))
	{
		printf("LoadFromFile[%s]  password error  code %s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "db_name", m_dbname))
	{
		printf("LoadFromFile[%s]  db_name error  code %s \n", file_path, err.c_str());
		return false;
	}

	if (!jxsjson::GetSubKeyValue(root, "server_id", m_server_id))
	{
		printf("LoadFromFile[%s]  server_id error  code %s \n", file_path, err.c_str());
		return false;
	}

	return true;
}


bool DBConfig::PreLoad()
{
	m_file_path_bak = m_file_path;
	m_ip_bak = m_ip;
	m_port_bak = m_port;
	m_user_bak = m_user;
	m_password_bak = m_password;
	m_dbname_bak = m_dbname;
	m_server_id_bak = m_server_id;
	return LoadFromFile(m_file_path.c_str());
}

bool DBConfig::ConformPreLoad()
{
	return true;
}

bool DBConfig::RollBackPreLoad()
{
	m_file_path = m_file_path_bak;
	m_ip = m_ip_bak;
	m_port = m_port_bak;
	m_user = m_user_bak;
	m_password = m_password_bak;
	m_dbname = m_dbname_bak;
	m_server_id = m_server_id_bak;
	return true;
}


