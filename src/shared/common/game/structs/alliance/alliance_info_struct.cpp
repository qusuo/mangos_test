#include "alliance_info_struct.h"

bool AllianceInfoStruct::Serialize(jxsstr::Serializer &se)
{
	bool res = se.PutInt64(alliance_id)
		&& se.PutInt8s(logic_data, logic_data_len);

	return res;
}

bool AllianceInfoStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt64(alliance_id)
		&& ds.GetInt32(logic_data_len);

	if (logic_data_len > 0)
	{
		res &= ds.GetInt8s(logic_data, logic_data_len);
	}

	return res;
}

void AllianceInfoStruct::Resset()
{
	alliance_id = 0;
	logic_data_len = 0;
}