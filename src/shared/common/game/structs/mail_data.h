#ifndef JXS_MAILDATA_H_
#define JXS_MAILDATA_H_

#include "../include/data_len_def.h"
#include "../../stream/serializer.h"
#include "../../stream/deserializer.h"
#include "../include/game_value_def.h"
#include "../../game/structs/army_format_data.h"
#include "../../game/structs/global/mail_content_struct.h"

struct MailDesc
{
	MailDesc()
		:mail_type(0)
		, mail_state(0)
		, sender_role_id(0)
		, receiver_role_id(0)
	{
		memset(sender_name, 0, sizeof(sender_name));
		memset(title, 0, sizeof(title));
	}
	Int8 mail_type;
	Int8 mail_state;
	RoleName sender_name;
	RoleID sender_role_id;
	RoleID receiver_role_id;
	char title[MAX_MAIL_TITLE_LEN];
};

// 战斗报告
//const Int32 MAX_BATTLE_ACTION_NUM = 150;
//struct MailBattleReport
//{
//	MailBattleReport() { Reset(); }
//	~MailBattleReport() {}
//
//	bool Serialize(jxsstr::Serializer& se)
//	{
//		return content.Serialize(se);
//	}
//	bool Deserialize(jxsstr::Deserializer& ds)
//	{
//		return content.Deserialize(ds);
//	}
//
//	void Reset()
//	{
//		content.Reset();
//	}
//
//	MailBattleContent content;
//};
// 邮件战报 end

// 侦查玩家报告 begin
struct MailSpyRoleReport
{
	MailSpyRoleReport()
	{
		Reset();
	}

	~MailSpyRoleReport()
	{}

	bool Serialize(jxsstr::Serializer& se)
	{
		return content.Serialize(se);
	}
	bool Deserialize(jxsstr::Deserializer& ds)
	{
		return content.Deserialize(ds);
	}
	void Reset()
	{
		content.Reset();
	}

	MailSpyRoleContent content;
};
// 侦查玩家报告 end

// 侦查矿点报告 begin
struct MailSpyOreReport
{
	MailSpyOreReport()
	{
		Reset();
	}
	~MailSpyOreReport();

	bool Serialize(jxsstr::Serializer& se)
	{
		return content.Serialize(se);
	}
	bool Deserialize(jxsstr::Deserializer& ds)
	{
		return content.Deserialize(ds);
	}
	void Reset()
	{
		content.Reset();
	}
	
	MailSpyOreContent content;
};
// 侦查矿点报告 end

// 玩家邮件 begin
struct MailUserMsg
{
	MailUserMsg()
		:user_msg_type(0)
		, msg_len(0)
	{
		memset(msg, 0, sizeof(msg));
	}

	Int32 user_msg_type;
	Int32 msg_len;
	char msg[MAX_MAIL_USER_MSG_LEN];
	MailMsgContent appendix;
};
// 玩家邮件 end

#endif