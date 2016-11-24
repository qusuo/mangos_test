#include "activity_struct.h"

ActivityStruct::ActivityStruct()
{
	Reset();
}

ActivityStruct::~ActivityStruct()
{

}

bool ActivityStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = true;
	res &= se.PutInt16(act_num);
	for (Int32 i = 0; i < act_num; ++i)
	{
		res &= se.PutInt32(act_items[i].act_id)
			&& se.PutInt16(act_items[i].act_data_len);
		if (act_items[i].act_data_len > 0)
		{
			res &= se.PutInt8Vector(act_items[i].act_data, act_items[i].act_data_len);
		}
	}

	return res;
}

bool ActivityStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool res = true;
	Int16 tmp = 0;
	res &= ds.GetInt16(act_num);
	for (Int32 i = 0; i < act_num; ++i)
	{
		res &= ds.GetInt32(act_items[i].act_id)
			&& ds.GetInt16(act_items[i].act_data_len);
		if (act_items[i].act_data_len > 0)
		{
			res &= ds.GetInt8Vector(act_items[i].act_data, act_items[i].act_data_len, tmp);
		}
	}

	return res;
}

void ActivityStruct::Reset()
{
	act_num = 0;
	memset(act_items, 0, sizeof(act_items));
}

