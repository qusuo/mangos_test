#include "alliance_tech_hall_struct.h"

bool AllianceTechHallStruct::Serialize(jxsstr::Serializer &se)
{
	return se.PutInt64(alliance_id)
		&& se.PutInt8s(logic_data, logic_data_len);
}

bool AllianceTechHallStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt64(alliance_id)
		&& ds.GetInt32(logic_data_len);

	if (logic_data_len > 0)
	{
		res &= ds.GetInt8s(logic_data, logic_data_len);
	}

	return res;
}

void AllianceTechHallStruct::Reset()
{
	alliance_id = 0;
	logic_data_len = 0;
}