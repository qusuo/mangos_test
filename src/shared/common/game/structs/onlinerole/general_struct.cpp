#include "general_struct.h"


bool GeneralStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = se.PutInt16(general_num);
	for (Int32 i = 0; i < general_num; ++i)
	{
		res &= se.PutInt8(general_items[i].db_opt)
			&& se.PutInt32(general_items[i].general_id)
			&& se.PutInt16(general_items[i].grid_pos)
			&& se.PutInt8Vector(general_items[i].logic_data, sizeof(general_items[i].logic_data));
	}

	return res;
}

bool GeneralStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	Int16 tmp = 0;
	bool res = ds.GetInt16(general_num);
	for (Int32 i = 0; i < general_num; ++i)
	{
		res &= ds.GetInt8(general_items[i].db_opt)
			&& ds.GetInt32(general_items[i].general_id)
			&& ds.GetInt16(general_items[i].grid_pos)
			&& ds.GetInt8Vector(general_items[i].logic_data, sizeof(general_items[i].logic_data), tmp);
	}

	return res;
}

void GeneralStruct::Reset()
{
	general_num = 0;
	memset(general_items, 0, sizeof(general_items));
}

GeneralStruct::GeneralStruct()
{
	Reset();
}

GeneralStruct::~GeneralStruct()
{

}
