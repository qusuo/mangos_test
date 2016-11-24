#ifndef JXS_MAILLISTSTRUCT_H_
#define JXS_MAILLISTSTRUCT_H_
//author:hsj
//邮件简报列表数据

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../include/game_value_def.h"

struct MailListStruct
{
	struct MailInfo
	{
		MailInfo()
			:mail_id(0)
			, create_time(0)
			, mail_type(0)
			, sub_type(0)
			, mail_state(0)
			, lock_state(0)
			, fetch_state(0)
		{
			memset(sender_name, 0, sizeof(sender_name));
			memset(title, 0, sizeof(title));
		}
		Int64 mail_id;
		Int64 create_time;
		Int8 mail_type;
		Int8 sub_type;
		Int8 mail_state;
		Int8 lock_state;
		Int8 fetch_state;
		RoleName sender_name;
		char title[MAX_MAIL_TITLE_LEN];
	};

	MailListStruct();
	~MailListStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();
	Int16 mail_num;
	MailInfo mails[MAX_MAIL_NUM_PER_TYPE];
};

#endif