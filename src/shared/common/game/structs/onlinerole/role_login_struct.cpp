#include "role_login_struct.h"

RoleLoginStruct::RoleLoginStruct()
{
	Reset();
}

RoleLoginStruct::~RoleLoginStruct()
{

}

bool RoleLoginStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret &= se.PutInt32(online_time)
		&& se.PutInt64(logout_time)
		&& se.PutInt32(anti_addiction)
		&& se.PutInt64(forbid_time)
		&& se.PutInt8Vector(client_sys, sizeof(client_sys))
		&& se.PutInt8Vector(device_code, sizeof(device_code))
		&& se.PutInt64(create_time);
	return ret;

}

bool RoleLoginStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	Int16 tmp = 0;
	ret &= ds.GetInt32(online_time)
		&& ds.GetInt64(logout_time)
		&& ds.GetInt32(anti_addiction)
		&& ds.GetInt64(forbid_time)
		&& ds.GetInt8Vector(client_sys, sizeof(client_sys), tmp)
		&& ds.GetInt8Vector(device_code, sizeof(device_code), tmp)
		&& ds.GetInt64(create_time);

	return ret;
}

void RoleLoginStruct::Reset()
{
	online_time = 0;
	logout_time = 0;
	anti_addiction = 0;
	forbid_time = 0;
	memset(client_sys, 0, sizeof(client_sys));
	memset(device_code, 0, sizeof(device_code));
	create_time = 0;
}