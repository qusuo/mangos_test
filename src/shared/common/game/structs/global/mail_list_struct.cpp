#include "mail_list_struct.h"

MailListStruct::MailListStruct()
{
	Reset();
}

MailListStruct::~MailListStruct()
{

}

bool MailListStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = se.PutInt16(mail_num);
	for (Int16 i = 0; i < mail_num; ++i)
	{
		res &= se.PutInt64(mails[i].mail_id)
			&& se.PutInt64(mails[i].create_time)
			&& se.PutInt8(mails[i].mail_type)
			&& se.PutInt8(mails[i].sub_type)
			&& se.PutInt8(mails[i].mail_state)
			&& se.PutInt8(mails[i].lock_state)
			&& se.PutInt8(mails[i].fetch_state)		
			&& se.PutInt8s(mails[i].sender_name, sizeof(mails[i].sender_name))
			&& se.PutInt8s(mails[i].title, sizeof(mails[i].title));
	}
	return res;
}

bool MailListStruct::Deserialize(jxsstr::Deserializer& ds)
{
	bool res = ds.GetInt16(mail_num);
	for (Int16 i = 0; i < mail_num; ++i)
	{
		res &= ds.GetInt64(mails[i].mail_id)
			&& ds.GetInt64(mails[i].create_time)
			&& ds.GetInt8(mails[i].mail_type)
			&& ds.GetInt8(mails[i].sub_type)
			&& ds.GetInt8(mails[i].mail_state)
			&& ds.GetInt8(mails[i].lock_state)
			&& ds.GetInt8(mails[i].fetch_state)			
			&& ds.GetInt8s(mails[i].sender_name, sizeof(mails[i].sender_name))
			&& ds.GetInt8s(mails[i].title, sizeof(mails[i].title));
	}
	return res;
}

void MailListStruct::Reset()
{
	mail_num = 0;
	for (Int32 i = 0; i < MAX_ROLE_ARMY_NUM; ++i)
	{
		memset(&mails[i], 0, sizeof(mails[i]));
	}
}