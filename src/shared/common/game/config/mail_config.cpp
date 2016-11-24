#include "mail_config.h"
#include "../../../common/external/json/jsonapi.h"
#include "../../../common/config/config_api.h"
#include "../../../common/bit/bit_api.h"
#include "../../../common/game/include/mail_def.h"
#include "../../../common/game/config/game_config_api.h"

MailConfigMgr::MailConfigMgr()
{

}

MailConfigMgr::~MailConfigMgr()
{
	ClearMails(m_mails);
	ClearMails(m_mails_bak);
}

MailConfigMgr& MailConfigMgr::Singleton()
{
	static MailConfigMgr instance;
	return instance;
}

bool MailConfigMgr::Init(const char* file_path)
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

bool MailConfigMgr::PreLoad()
{
	m_mails_bak.clear();
	MailCfgMap::iterator itr = m_mails.begin();
	MailCfgMap::iterator itr_end = m_mails.end();
	for (; itr != itr_end; ++itr)
	{
		m_mails_bak[itr->first] = itr->second;
	}

	m_mails.clear();
	return LoadFromFile(m_file_path.c_str());
}

bool MailConfigMgr::ConformPreLoad()
{
	ClearMails(m_mails_bak);
	return true;
}

bool MailConfigMgr::RollBackPreLoad()
{
	ClearMails(m_mails);
	MailCfgMap::iterator itr = m_mails_bak.begin();
	MailCfgMap::iterator itr_end = m_mails_bak.end();
	for (; itr != itr_end; ++itr)
	{
		m_mails[itr->first] = itr->second;
	}
	m_mails_bak.clear();
	return true;
}

const MailConfig* MailConfigMgr::GetMailConfig(Int32 msg_type)
{
	MailCfgMap::iterator itr = m_mails.find(msg_type);
	if (itr != m_mails.end())
	{
		return itr->second;
	}
	else
	{
		return NULL;
	}
}

void MailConfigMgr::ClearMails(MailCfgMap& mails)
{
	MailCfgMap::iterator itr = mails.begin();
	MailCfgMap::iterator itr_end = mails.end();
	for (; itr != itr_end; ++itr)
	{
		delete itr->second;
	}
	mails.clear();
}

bool MailConfigMgr::LoadFromFile(const char* file_path)
{
	Json::Value root;
	std::string err;
	bool res = jxsjson::LoadFromFile(file_path, root, err);
	if (!res)
	{
		printf("LoadFromFile[%s] error: %s \n", file_path, err.c_str());
		return false;
	}

	MailConfig tmp_cfg;
	for (Json::Value::UInt cfg_index = 0; cfg_index < root.size(); ++cfg_index)
	{
		Json::Value mail_value = root[cfg_index];
		if (!jxsjson::GetSubKeyValue(mail_value, "mail_type", tmp_cfg.msg_type)
			|| tmp_cfg.msg_type <= MAIL_MSG_TYPE_INVALID
			|| tmp_cfg.msg_type >= MAIL_MSG_TYPE_END
			)
		{
			printf("LoadFromFile[%s] error, mail_type:%d \n", file_path, tmp_cfg.msg_type);
			return false;
		}

		if (!jxsjson::GetSubKeyValue(mail_value, "mail_top", tmp_cfg.title))
		{
			printf("LoadFromFile[%s] mail_top error, type:%d \n", file_path, tmp_cfg.msg_type);
			return false;
		}

		if (!jxsjson::GetSubKeyValue(mail_value, "mail_text", tmp_cfg.content))
		{
			printf("LoadFromFile[%s] mail_text error, type:%d \n", file_path, tmp_cfg.msg_type);
			return false;
		}

		if (!jxsjson::GetSubKeyValue(mail_value, "sender_id", tmp_cfg.sender_type))
		{
			printf("LoadFromFile[%s] sender_id error, type:%d \n", file_path, tmp_cfg.msg_type);
			return false;
		}

		std::string tmp_str;
		if (!jxsjson::GetSubKeyValue(mail_value, "resource_id1", tmp_str))
		{
			printf("LoadFromFile[%s] resource_id1 error, type:%d \n", file_path, tmp_cfg.msg_type);
			return false;
		}

	//	tmp_cfg.item_param_num = gameconfigapi::ParseItemParams(tmp_str, tmp_cfg.item_params, MAX_MAIL_ITEM_PARAM_NUM);

		MailConfig* new_mail = new MailConfig(tmp_cfg);
		if (NULL == new_mail)
		{
			printf("LoadFromFile[%s] error, new mail fail\n", file_path);
			delete new_mail;
			return false;
		}
		if (NULL != GetMailConfig(tmp_cfg.msg_type))
		{
			printf("LoadFromFile[%s] error, type:%d is allready exist \n", file_path, tmp_cfg.msg_type);
			delete new_mail;
			return false;
		}

		m_mails[tmp_cfg.msg_type] = new_mail;
	}
	return true;
}

