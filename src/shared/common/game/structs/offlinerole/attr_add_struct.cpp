#include "attr_add_struct.h"

AttrAddStruct::AttrAddStruct()
{
	Reset();
}

AttrAddStruct::~AttrAddStruct()
{

}

bool AttrAddStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret &= se.PutInt16(skill_add_num);
	if (skill_add_num > 0 && skill_add_num <= SOLDIER_TYPE_COUNT)
	{
		for (Int32 i = 0; i < skill_add_num; ++i)
		{
			ret &= se.PutInt32(skill_add[i].soldier_type)
				&& SerializeAddParams(skill_add[i].attr_add, se);
		}
	}

	ret &= se.PutInt16(wartech_add_num);
	if (wartech_add_num > 0 && wartech_add_num <= MAX_SOLDIER_ID_NUM)
	{
		for (Int32 i = 0; i < wartech_add_num; ++i)
		{
			ret &= se.PutInt32(wartech_add[i].soldier_id)
				&& SerializeAddParams(wartech_add[i].attr_add, se);
		}		
	}

	ret &= se.PutInt16(equip_add_num);
	if (equip_add_num > 0 && equip_add_num <= FORMATION_GRID_NUM)
	{
		for (Int32 i = 0; i < equip_add_num; ++i)
		{
			ret &= se.PutInt32(equip_add[i].equip_pos)
				&& SerializeAddParams(equip_add[i].attr_add, se);
		}
	}

	ret &= se.PutInt16(ordnance_add_num);
	if (ordnance_add_num > 0 && ordnance_add_num <= SOLDIER_TYPE_COUNT)
	{
		for (Int32 i = 0; i < ordnance_add_num; ++i)
		{
			ret &= se.PutInt32(ordnance_add[i].soldier_type)
				&& SerializeAddParams(ordnance_add[i].attr_add, se);
		}
	}

	return ret;
}

bool AttrAddStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	ret &= ds.GetInt16(skill_add_num);
	if (skill_add_num > 0 && skill_add_num <= SOLDIER_TYPE_COUNT)
	{
		for (Int32 i = 0; i < skill_add_num; ++i)
		{
			ret &= ds.GetInt32(skill_add[i].soldier_type)
				&& DeserializeAddParams(skill_add[i].attr_add, ds, data_version);
		}
	}

	ret &= ds.GetInt16(wartech_add_num);
	if (wartech_add_num > 0 && wartech_add_num <= MAX_SOLDIER_ID_NUM)
	{
		for (Int32 i = 0; i < wartech_add_num; ++i)
		{
			ret &= ds.GetInt32(wartech_add[i].soldier_id)
				&& DeserializeAddParams(wartech_add[i].attr_add, ds, data_version);
		}
	}

	ret &= ds.GetInt16(equip_add_num);
	if (equip_add_num > 0 && equip_add_num <= FORMATION_GRID_NUM)
	{
		for (Int32 i = 0; i < equip_add_num; ++i)
		{
			ret &= ds.GetInt32(equip_add[i].equip_pos)
				&& DeserializeAddParams(equip_add[i].attr_add, ds, data_version);
		}
	}

	ret &= ds.GetInt16(ordnance_add_num);
	if (ordnance_add_num > 0 && ordnance_add_num <= SOLDIER_TYPE_COUNT)
	{
		for (Int32 i = 0; i < ordnance_add_num; ++i)
		{
			ret &= ds.GetInt32(ordnance_add[i].soldier_type)
				&& DeserializeAddParams(ordnance_add[i].attr_add, ds, data_version);
		}
	}
	
	return ret;
}

void AttrAddStruct::Reset()
{
	skill_add_num = 0;
	memset(skill_add, 0, sizeof(skill_add));

	wartech_add_num = 0;
	memset(wartech_add, 0, sizeof(wartech_add));

	memset(&equip_add, 0, sizeof(equip_add));

	ordnance_add_num = 0;
	memset(ordnance_add, 0, sizeof(ordnance_add));
}

bool AttrAddStruct::SerializeAddParams(AttrAddArrayStruct& attrs, jxsstr::Serializer& se)
{
	bool res = se.PutInt16(attrs.param_num);
	for (Int16 i = 0; i < attrs.param_num && i < AttrAddArrayStruct::MAX_ATTR_ADD_PARAM_NUM; ++i)
	{
		res &= se.PutInt16(attrs.params[i].add_mode)
			&& se.PutInt16(attrs.params[i].attr_type)
			&& se.PutInt32(attrs.params[i].add_value);
	}

	return res;
}

bool AttrAddStruct::DeserializeAddParams(AttrAddArrayStruct& attrs, jxsstr::Deserializer& ds, Int32 data_version)
{
	bool res = ds.GetInt16(attrs.param_num);
	for (Int16 i = 0; i < attrs.param_num && i < AttrAddArrayStruct::MAX_ATTR_ADD_PARAM_NUM; ++i)
	{
		res &= ds.GetInt16(attrs.params[i].add_mode)
			&& ds.GetInt16(attrs.params[i].attr_type)
			&& ds.GetInt32(attrs.params[i].add_value);
	}

	return res;
}
