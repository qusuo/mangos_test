#include "offline_role_struct.h"

OfflineRoleStruct::OfflineRoleStruct()
{
	Reset();
}

OfflineRoleStruct::~OfflineRoleStruct()
{

}

bool OfflineRoleStruct::Serialize(jxsstr::Serializer& se)
{
	return se.PutInt64(save_time)
		&& attr_base.Serialize(se)
		&& attr_add.Serialize(se)
		&& building.Serialize(se)
		&& soldier.Serialize(se)
		&& role_comm.Serialize(se)
		&& armies.Serialize(se);
}

bool OfflineRoleStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	return ds.GetInt64(save_time)
		&& attr_base.Deserialize(ds, data_version)
		&& attr_add.Deserialize(ds, data_version)
		&& building.Deserialize(ds, data_version)
		&& soldier.Deserialize(ds, data_version)
		&& role_comm.Deserialize(ds, data_version)
		&& armies.Deserialize(ds, data_version);
}

void OfflineRoleStruct::Reset()
{
	save_time = 0;
	attr_base.Reset();
	attr_add.Reset();
	building.Reset();
	soldier.Reset();
	role_comm.Reset();
	armies.Reset();
}