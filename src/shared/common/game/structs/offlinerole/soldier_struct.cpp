#include "soldier_struct.h"

SoldierStruct::SoldierStruct()
{
	Reset();
}

SoldierStruct::~SoldierStruct()
{

}

bool SoldierStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret = se.PutInt16(inner_unit_num);
	for (Int16 i = 0; i < inner_unit_num; ++i)
	{
		ret &= se.PutInt32(inner_units[i].soldier_id)
			&& se.PutInt32(inner_units[i].soldier_num);
	}

	ret &= se.PutInt16(injured_unit_num);
	for (Int16 i = 0; i < injured_unit_num; ++i)
	{
		ret &= se.PutInt32(injured_units[i].soldier_id)
			&& se.PutInt32(injured_units[i].soldier_num);
	}

	return ret;
}

bool SoldierStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	ret = ds.GetInt16(inner_unit_num);
	for (Int16 i = 0; i < inner_unit_num; ++i)
	{
		ret &= ds.GetInt32(inner_units[i].soldier_id)
			&& ds.GetInt32(inner_units[i].soldier_num);
	}

	ret &= ds.GetInt16(injured_unit_num);
	for (Int16 i = 0; i < injured_unit_num; ++i)
	{
		ret &= ds.GetInt32(injured_units[i].soldier_id)
			&& ds.GetInt32(injured_units[i].soldier_num);
	}

	return ret;
}

void SoldierStruct::Reset()
{
	inner_unit_num = 0;
	injured_unit_num = 0;
	memset(inner_units, 0, sizeof(inner_units));
	memset(injured_units, 0, sizeof(injured_units));
}