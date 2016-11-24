#ifndef JXS_UTF8STRCONFIG_H_
#define JXS_UTF8STRCONFIG_H_
//一些utf8字符

//服务器配置
#include "../../config/config-interface.h"
#include "../../include/value_def.h"
#include "../../include/basic_type_def.h"
#include "../include/server_def.h"


class Utf8StrConfig : public jxs::Config
{
public:
	virtual ~Utf8StrConfig();
	virtual bool Init(const char* file_path);
	virtual bool PreLoad();
	virtual bool ConformPreLoad();
	virtual bool RollBackPreLoad();
	static Utf8StrConfig& Singleton();

	const std::string& GetOreBaseName(Int32 resource_type);
	const std::string& GetOreSprintfStr() { return m_ore_sprintf_str; }
	const std::string& GetMailSenderName() { return m_mail_sender_name; }
	const std::string& GetAttackTitle() { return m_attack_title; }
	const std::string& GetBeAttackTitle() { return m_be_attack_title; }
	const std::string& GetSpyTitle() { return m_spy_title; }

private:
	Utf8StrConfig();

	bool LoadFromFile(const char* file_path);

private:
	std::string m_file_path;
	std::string m_ore_base_name[RESOURCE_TYPE_COUNT];
	std::string m_ore_sprintf_str;
	std::string m_mail_sender_name;
	std::string m_attack_title;
	std::string m_be_attack_title;
	std::string m_spy_title;

	std::string m_ore_base_name_bak[RESOURCE_TYPE_COUNT];
	std::string m_ore_sprintf_str_bak;
	std::string m_mail_sender_name_bak;
	std::string m_attack_title_bak;
	std::string m_be_attack_title_bak;
	std::string m_spy_title_bak;
};

#define g_utf8_str_config Utf8StrConfig::Singleton()


#endif