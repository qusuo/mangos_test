#include "alliance_member_struct.h"

bool AllianceMemberStruct::Serialize(jxsstr::Serializer& se)
{
	return se.PutInt32(logic_data_len) && se.PutInt8s(logic_data, logic_data_len);
}

bool AllianceMemberStruct::Deserialize(jxsstr::Deserializer& ds)
{
	bool res = ds.GetInt32(logic_data_len);
	if (logic_data_len > 0)
	{
		res &= ds.GetInt8s(logic_data, logic_data_len);
	}

	return res;
}

void AllianceMemberStruct::Reset()
{
	role_id = 0;
	alliance_id = 0;
	post = 0;
	logic_data_len = 0;
}

