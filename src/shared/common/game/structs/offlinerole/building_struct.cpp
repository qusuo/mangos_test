#include "building_struct.h"

BuildingStruct::BuildingStruct()
{
	Reset();
}

BuildingStruct::~BuildingStruct()
{

}

bool BuildingStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret = se.PutInt16(building_num);
	for (Int16 i = 0; i < building_num; ++i)
	{
		ret &= se.PutInt16(buildings[i].building_id)
			&& se.PutInt16(buildings[i].level)
			&& se.PutInt16(buildings[i].resource_building_type)
			&& se.PutInt16(buildings[i].reserved);
	}
	ret &= se.PutInt16(city_pos.x)
		&& se.PutInt16(city_pos.y);
	return ret;
}

bool BuildingStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	ret = ds.GetInt16(building_num);
	for (Int16 i = 0; i < building_num; ++i)
	{
		ret &= ds.GetInt16(buildings[i].building_id)
			&& ds.GetInt16(buildings[i].level)
			&& ds.GetInt16(buildings[i].resource_building_type)
			&& ds.GetInt16(buildings[i].reserved);
	}
	ret &= ds.GetInt16(city_pos.x)
		&& ds.GetInt16(city_pos.y);

	return ret;
}

void BuildingStruct::Reset()
{
	building_num = 0;
	memset(buildings, 0, sizeof(buildings));
	city_pos.x = INVALID_POS;
	city_pos.y = INVALID_POS;

}