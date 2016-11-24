#ifndef JXS_DBCONFIG_H_
#define JXS_DBCONFIG_H_

//数据库配置
#include "../../config/config-interface.h"
#include "../../include/value_def.h"
#include "../../include/basic_type_def.h"

class DBConfig : public jxs::Config
{
public:
	virtual ~DBConfig();
	virtual bool Init(const char* file_path);
	virtual bool PreLoad();
	virtual bool ConformPreLoad();
	virtual bool RollBackPreLoad();
	static DBConfig& Singleton();

	const char* GetIp() const { return m_ip.c_str(); }
	Int32 GetPort() const { return m_port; }
	const char* GetUser() const { return m_user.c_str(); }
	const char* GetPassword() const { return m_password.c_str(); }
	const char* GetDBName() const { return m_dbname.c_str(); }
	//Int32 GetServerId() const { return m_server_id; }
	//Int32 GetPlatId() const { return m_plat_id; }

private:
	DBConfig();
	bool LoadFromFile(const char* file_path);

private:
	std::string m_file_path;
	std::string m_ip;
	Int32 m_port;
	std::string m_user;
	std::string m_password;
	std::string m_dbname;
	Int32 m_server_id;
	Int32 m_plat_id;

	std::string m_file_path_bak;
	std::string m_ip_bak;
	Int32 m_port_bak;
	std::string m_user_bak;
	std::string m_password_bak;
	std::string m_dbname_bak;
	Int32 m_server_id_bak;
};

#define g_db_config DBConfig::Singleton()

#endif