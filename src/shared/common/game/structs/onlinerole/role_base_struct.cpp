#include "role_base_struct.h"

RoleBaseStruct::RoleBaseStruct()
{
	Reset();
}

RoleBaseStruct::~RoleBaseStruct()
{

}

bool RoleBaseStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret &= se.PutInt64(role_id)
		&& se.PutInt8Vector(account, sizeof(account))
		&& se.PutInt8Vector(role_name, sizeof(role_name))	
		&& se.PutInt16(role_level)
		&& se.PutInt16(vip_level)
		&& se.PutInt32(gold)
		&& se.PutInt64(last_save_time);
	return ret;

}

bool RoleBaseStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	Int16 tmp = 0;
	ret &= ds.GetInt64(role_id)
		&& ds.GetInt8Vector(account, sizeof(account), tmp)
		&& ds.GetInt8Vector(role_name, sizeof(role_name), tmp)
		&& ds.GetInt16(role_level)
		&& ds.GetInt16(vip_level)
		&& ds.GetInt32(gold)
		&& ds.GetInt64(last_save_time);
	return ret;
}

void RoleBaseStruct::Reset()
{
	role_id = 0;
	memset(account, 0, sizeof(account));
	memset(role_name, 0, sizeof(role_name));
	role_level = 0;
	vip_level = 0;
	gold = 0;
	last_save_time = 0;
}