#include "mail_struct.h"

MailStruct::MailStruct()
	: mail_id(0)
	, mail_type(0)
	, mail_state(0)
	, is_fetch(0)
	, is_lock(0)
	, create_time(0)
	, sender_role_id(0)
	, receiver_role_id(0)
	, content_data_len(0)
{
	memset(sender_name, 0, sizeof sender_name);
	memset(title, 0, sizeof title);
	memset(content, 0, sizeof content);
}

MailStruct::~MailStruct()
{

}

bool MailStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = se.PutInt64(mail_id)
		&& se.PutInt8(mail_type)
		&& se.PutInt8(sub_type)
		&& se.PutInt8(mail_state)
		&& se.PutInt8(is_fetch)
		&& se.PutInt8(is_lock)
		&& se.PutInt64(create_time)
		&& se.PutInt8Vector(sender_name, sizeof(sender_name))
		&& se.PutInt64(sender_role_id)
		&& se.PutInt64(receiver_role_id)
		&& se.PutInt8Vector(title, sizeof(title))
		&& se.PutInt32(content_data_len);

	if (content_data_len > 0)
	{
		res &= se.PutInt8s(content, content_data_len);
	}

	return res;
}

bool MailStruct::Deserialize(jxsstr::Deserializer& ds)
{
	Int16 tmp = 0;
	bool res = ds.GetInt64(mail_id)
		&& ds.GetInt8(mail_type)
		&& ds.GetInt8(sub_type)
		&& ds.GetInt8(mail_state)
		&& ds.GetInt8(is_fetch)
		&& ds.GetInt8(is_lock)
		&& ds.GetInt64(create_time)
		&& ds.GetInt8Vector(sender_name, sizeof(sender_name), tmp)
		&& ds.GetInt64(sender_role_id)
		&& ds.GetInt64(receiver_role_id)
		&& ds.GetInt8Vector(title, sizeof(title), tmp)
		&& ds.GetInt32(content_data_len);

	if (content_data_len > 0)
	{
		res &= ds.GetInt8s(content, content_data_len);
	}

	return res;
}

void MailStruct::Reset()
{
	mail_id = 0;
	mail_type = 0;
	sub_type = 0;
	mail_state = 0;
	is_fetch = 0;
	is_lock = 0;
	create_time = 0;
	memset(sender_name, 0, sizeof(sender_name));
	sender_role_id = 0;
	receiver_role_id = 0;
	memset(title, 0, sizeof(title));
	content_data_len = 0;
	memset(content, 0, sizeof(content));
}