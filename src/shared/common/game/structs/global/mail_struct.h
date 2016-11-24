#ifndef JXS_MAILSTRUCT_H_
#define JXS_MAILSTRUCT_H_
//author:hsj
//гЪМўЪ§Он

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../include/mail_def.h"
#include "../../include/game_value_def.h"

struct MailStruct
{
	MailStruct();
	~MailStruct();

	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	Int64 mail_id;
	Int8 mail_type;
	Int8 sub_type;
	Int8 mail_state;
	Int8 is_fetch;
	Int8 is_lock;
	Int64 create_time;
	RoleName sender_name;
	RoleID sender_role_id;
	RoleID receiver_role_id;
	RoleName receiver_name;
	char title[MAX_MAIL_TITLE_LEN];
	Int32 content_data_len;
	char content[MAIL_CONTENT_DATA_LEN];
};



#endif