#ifndef JXS_MAILCONFIG_H_
#define JXS_MAILCONFIG_H_
/*
* 邮件配表
*/
#include "../../../common/config/config-interface.h"
#include "../../../common/game/include/battle_def.h"
#include "../../../common/game/include/game_value_def.h"
#include "../../../common/game/structs/item_data.h"

struct MailConfig
{
	MailConfig()
		:msg_type(0)
		, sender_type(0)
		, item_param_num(0)
	{
	}
	~MailConfig(){}
	Int32 msg_type;
	Int32 sender_type;									//发件人类型 1=系统，2=玩家名，3=军团
	std::string title;
	std::string content;
	Int16 item_param_num;								//道具列表长度
	ItemParam item_params[MAX_MAIL_ITEM_PARAM_NUM];		//道具列表
};

class MailConfigMgr : public jxs::Config
{
public:
	virtual ~MailConfigMgr();

	static MailConfigMgr& Singleton();

	virtual bool Init(const char* file_path);
	virtual bool PreLoad();
	virtual bool ConformPreLoad();
	virtual bool RollBackPreLoad();

	const MailConfig* GetMailConfig(Int32 msg_type);	// 


private:

	typedef std::map<Int32, MailConfig*> MailCfgMap;
	MailConfigMgr();
	bool LoadFromFile(const char* file_path);
	void ClearMails(MailCfgMap& mails);

private:
	MailCfgMap m_mails;
	MailCfgMap m_mails_bak;
	std::string m_file_path;
};

#define g_mail_cfg_mgr MailConfigMgr::Singleton()


#endif