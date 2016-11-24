#include "army_struct.h"

ArmyStruct::ArmyStruct()
{
	Reset();
}

ArmyStruct::~ArmyStruct()
{

}

bool ArmyStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = true;
	res = se.PutInt8(army_num);
	for (Int32 i = 0; i < army_num; ++i)
	{
		res &= se.PutInt8(armies[i].pos)
			&& SerializeArmyBuffData(armies[i].army_data, se);
	}
	res &= SerializeArmyBuffData(city_army, se)
		&& SerializeArmyBuffData(arena_army, se);
	return res;
}

bool ArmyStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool res = true;
	res = ds.GetInt8(army_num);
	for (Int32 i = 0; i < army_num; ++i)
	{
		res &= ds.GetInt8(armies[i].pos)
			&& DeserializeArmyBuffData(armies[i].army_data, ds, data_version);
	}

	res &= DeserializeArmyBuffData(city_army, ds, data_version)
		&& DeserializeArmyBuffData(city_army, ds, data_version);
	return res;
}

void ArmyStruct::Reset()
{
	army_num = 0;
	for (Int32 i = 0; i < MAX_ROLE_ARMY_NUM; ++i)
	{
		armies[i].pos = 0;
		armies[i].army_data.Reset();
	}
	city_army.Reset();
	arena_army.Reset();
}

bool ArmyStruct::SerializeArmyBuffData(ArmyBuffData& data, jxsstr::Serializer& se)
{
	bool res = true;
	for (Int32 soldier_pos = 0; soldier_pos < FORMATION_GRID_NUM; ++soldier_pos)
	{
		res &= se.PutInt32(data.soldiers[soldier_pos].soldier_id)
			&& se.PutInt32(data.soldiers[soldier_pos].soldier_num);
	}
	for (Int32 store_index = 0; store_index < GENERAL_TYPE_COUNT; ++store_index)
	{
		res &= se.PutInt32(data.generals[store_index].general_id)
			&& se.PutInt32(data.generals[store_index].store_index)
			&& se.PutInt32(data.generals[store_index].level)
			&& se.PutInt32(data.generals[store_index].grade)
			&& se.PutInt32Vector(data.generals[store_index].attr, GENERAL_ATTR_TYPE_COUNT)
			&& se.PutInt32Vector(data.generals[store_index].proficiency, SOLDIER_TYPE_COUNT)
			&& se.PutInt32(data.generals[store_index].general_skill_id)
			&& se.PutInt32(data.generals[store_index].soldier_skill_id)
			&& se.PutInt32(data.generals[store_index].spe_skill_id);
	}
	return res;
}

bool ArmyStruct::DeserializeArmyBuffData(ArmyBuffData& data, jxsstr::Deserializer& ds, Int32 data_version)
{
	bool res = true;
	for (Int32 soldier_pos = 0; soldier_pos < FORMATION_GRID_NUM; ++soldier_pos)
	{
		res &= ds.GetInt32(data.soldiers[soldier_pos].soldier_id)
			&& ds.GetInt32(data.soldiers[soldier_pos].soldier_num);
	}
	for (Int32 store_index = 0; store_index < GENERAL_TYPE_COUNT; ++store_index)
	{
		Int16 tmp_pos;
		res &= ds.GetInt32(data.generals[store_index].general_id)
			&& ds.GetInt32(data.generals[store_index].store_index)
			&& ds.GetInt32(data.generals[store_index].level)
			&& ds.GetInt32(data.generals[store_index].grade)
			&& ds.GetInt32Vector(data.generals[store_index].attr, GENERAL_ATTR_TYPE_COUNT, tmp_pos)
			&& ds.GetInt32Vector(data.generals[store_index].proficiency, SOLDIER_TYPE_COUNT, tmp_pos)
			&& ds.GetInt32(data.generals[store_index].general_skill_id)
			&& ds.GetInt32(data.generals[store_index].soldier_skill_id)
			&& ds.GetInt32(data.generals[store_index].spe_skill_id);
	}
	return res;
}
