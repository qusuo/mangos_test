#include "alliance_shop_struct.h"

bool AllianceShopStruct::Serialize(jxsstr::Serializer &se)
{
	bool res = se.PutInt64(alliance_id)
		&& se.PutInt8s(logic_data, logic_data_len);

	return res;
}

bool AllianceShopStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt64(alliance_id)
		&& ds.GetInt32(logic_data_len);

	if (logic_data_len > 0)
	{
		res &= ds.GetInt8s(logic_data, logic_data_len);
	}

	return res;
}

void AllianceShopStruct::Reset()
{
	alliance_id = 0;
	logic_data_len = 0;
}