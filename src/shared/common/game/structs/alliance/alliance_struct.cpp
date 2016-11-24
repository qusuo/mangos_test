#include "alliance_struct.h"

bool AllianceStruct::Serialize(jxsstr::Serializer& se)
{
	return se.PutInt64(alliance_id)
		&& se.PutInt32(level)
		&& se.PutInt64(alliance_leader_id)
		&& se.PutInt8Vector(alliance_name, sizeof(alliance_name))
		&& se.PutInt32(logic_data_len)
		&& se.PutInt8s(logic_data, logic_data_len);
}

bool AllianceStruct::Deserialize(jxsstr::Deserializer& ds)
{
	Int16 tmp = 0;
	bool res = ds.GetInt64(alliance_id)
		&& ds.GetInt32(level)
		&& ds.GetInt64(alliance_leader_id)
		&& ds.GetInt8Vector(alliance_name, sizeof(alliance_name), tmp)
		&& ds.GetInt32(logic_data_len);

	if (logic_data_len > 0)
	{
		res &= ds.GetInt8s(logic_data, logic_data_len);
	}

	return res;
}

void AllianceStruct::Reset()
{
	alliance_id = 0;
	level = 0;
	alliance_leader_id = 0;
}

