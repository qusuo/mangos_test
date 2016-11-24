#include "item_struct.h"



bool ItemStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = SerializeParams(grid_item_num, grid_items, se)
		&& SerializeParams(general_equip_num, general_equips, se)
		&& SerializeParams(role_parts_num, role_parts, se)
		&& SerializeParams(role_equip_num, role_equips, se);
	return res;
}

bool ItemStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool res = DeserializeParams(grid_item_num, grid_items, ds)
		&& DeserializeParams(general_equip_num, general_equips, ds)
		&& DeserializeParams(role_parts_num, role_parts, ds)
		&& DeserializeParams(role_equip_num, role_equips, ds);

	return res;
}

bool ItemStruct::SerializeParams(Int16 param_num, ItemStructParam params[], jxsstr::Serializer& se)
{
	bool res = se.PutInt16(param_num);
	for (Int32 i = 0; i < param_num; ++i)
	{
		res &= se.PutInt8(params[i].db_opt)
			&& se.PutInt16(params[i].grid_pos)
			&& se.PutInt32(params[i].item_id)
			&& se.PutInt32(params[i].item_num)
			&& se.PutInt64(params[i].dead_line)
			&& se.PutInt8(params[i].is_equip);
		if (1 == params[i].is_equip)
		{
			res &= se.PutInt8Vector(params[i].equip_data, sizeof(params[i].equip_data));
		}
	}
	return res;
}

bool ItemStruct::DeserializeParams(Int16& param_num, ItemStructParam params[], jxsstr::Deserializer& ds)
{
	Int16 tmp = 0;
	bool res = ds.GetInt16(param_num);
	for (Int32 i = 0; i < param_num; ++i)
	{
		res &= ds.GetInt8(params[i].db_opt)
			&& ds.GetInt16(params[i].grid_pos)
			&& ds.GetInt32(params[i].item_id)
			&& ds.GetInt32(params[i].item_num)
			&& ds.GetInt64(params[i].dead_line)
			&& ds.GetInt8(params[i].is_equip);
		if (1 == params[i].is_equip)
		{
			res &= ds.GetInt8Vector(params[i].equip_data, sizeof(params[i].equip_data), tmp);
		}
	}
	return res;
}

void ItemStruct::Reset()
{
	grid_item_num = 0;
	memset(grid_items, 0, sizeof(grid_items));

	general_equip_num = 0;
	memset(general_equips, 0, sizeof(general_equips));

	role_parts_num = 0;
	memset(role_parts, 0, sizeof(role_parts));

	role_equip_num = 0;
	memset(role_equips, 0, sizeof(role_equips));
}

ItemStruct::ItemStruct()
{
	Reset();
}
