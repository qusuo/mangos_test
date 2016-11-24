#include "role_struct.h"
#include "../../include/data_version_def.h"

RoleStruct::RoleStruct()
{
	Reset();
}

RoleStruct::~RoleStruct()
{

}

bool RoleStruct::Serialize(jxsstr::Serializer& se)
{
	return logic_attr.Serialize(se)
		&& role_base.Serialize(se)
		&& chapter.Serialize(se)
		&& items.Serialize(se)
		&& activity.Serialize(se)
		&& general.Serialize(se)
		&& task.Serialize(se)
		&& role_login.Serialize(se);
}

bool RoleStruct::Deserialize(jxsstr::Deserializer& ds)
{
	Int32 data_version = DATA_VERSION_INVALID;
	if (!ds.ViewInt32(data_version))
	{
		return false;
	}
	return logic_attr.Deserialize(ds)
		&& role_base.Deserialize(ds, data_version)
		&& chapter.Deserialize(ds, data_version)
		&& items.Deserialize(ds, data_version)
		&& activity.Deserialize(ds, data_version)
		&& general.Deserialize(ds, data_version)
		&& task.Deserialize(ds, data_version)
		&& role_login.Deserialize(ds, data_version);
}

void RoleStruct::Reset()
{
	role_base.Reset();
	chapter.Reset();
	items.Reset();
	logic_attr.Reset();
	activity.Reset();
	general.Reset();
	task.Reset();
	role_login.Reset();
}