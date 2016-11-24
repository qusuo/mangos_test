#include "mail_content_struct.h"

MailBattleContent::MailBattleContent()
{
	Reset();
}

bool MailBattleContent::Serialize(jxsstr::Serializer& se)
{
	bool res = se.PutInt16(content_type)
		&& se.PutInt64(battle_time)
		&& se.PutInt8s(target_name, sizeof(target_name))
		&& se.PutInt16(target_pos.x)
		&& se.PutInt16(target_pos.y)
		&& se.PutInt8s(target_alliance, sizeof(target_alliance))
		&& se.PutInt8s(target_pos_name, sizeof(target_pos_name))
		&& SerializeSideData(se, attaker)
		&& SerializeSideData(se, defender)
		&& se.PutInt8s(garrison_name, sizeof(garrison_name))
		&& se.PutInt8(battle_result)
		&& se.PutInt32s(resource, RESOURCE_TYPE_COUNT)
		&& se.PutInt64(battle_id)
		&& se.PutInt32(item_param_num);
	for (Int32 i = 0; i < item_param_num && i < MAX_MAIL_ITEM_PARAM_NUM; ++i)
	{
		res &= se.PutInt32(item_params[i].item_id)
			&& se.PutInt32(item_params[i].item_num);
	}

	return res;
}

bool MailBattleContent::Deserialize(jxsstr::Deserializer& ds)
{
	bool res = ds.GetInt16(content_type)
		&& ds.GetInt64(battle_time)
		&& ds.GetInt8s(target_name, sizeof(target_name))
		&& ds.GetInt16(target_pos.x)
		&& ds.GetInt16(target_pos.y)
		&& ds.GetInt8s(target_alliance, sizeof(target_alliance))
		&& ds.GetInt8s(target_pos_name, sizeof(target_pos_name))
		&& DeserializeSideData(ds, attaker)
		&& DeserializeSideData(ds, defender)
		&& ds.GetInt8s(garrison_name, sizeof(garrison_name))
		&& ds.GetInt8(battle_result)
		&& ds.GetInt32s(resource, RESOURCE_TYPE_COUNT)
		&& ds.GetInt64(battle_id)
		&& ds.GetInt32(item_param_num);
	for (Int32 i = 0; i < item_param_num && i < MAX_MAIL_ITEM_PARAM_NUM; ++i)
	{
		res &= ds.GetInt32(item_params[i].item_id)
			&& ds.GetInt32(item_params[i].item_num);
	}

	return res;
}

bool MailBattleContent::SerializeSideData(jxsstr::Serializer& se, SideData& side_data)
{
	bool res = se.PutInt8(side_data.obj_type) 
		&& se.PutInt8s(side_data.role_name, sizeof(side_data.role_name))
		&& se.PutInt64(side_data.role_id)
		&& se.PutInt16(side_data.city_pos.x)
		&& se.PutInt16(side_data.city_pos.y)
		&& se.PutInt16(side_data.vip_level)
		&& se.PutInt8s(side_data.alliance_name, sizeof(side_data.alliance_name))
		&& se.PutInt32(side_data.cur_boom)
		&& se.PutInt32(side_data.max_boom)
		&& se.PutInt32(side_data.boom_change)
		&& se.PutInt8(side_data.soldier_unit_num);
	for (Int32 i = 0; i < side_data.soldier_unit_num && i < FORMATION_GRID_NUM; ++i)
	{
		res &= se.PutInt32(side_data.soldiers[i].soldier_id)
			&& se.PutInt32(side_data.soldiers[i].soldier_num);
	}
	res &= se.PutInt8(side_data.general_num);
	if (side_data.general_num > 0)
	{
		res &= se.PutInt32s(side_data.generals, side_data.general_num);
	}
	res &= se.PutInt8(side_data.is_ahead);
	
	return res;
}

bool MailBattleContent::DeserializeSideData(jxsstr::Deserializer& ds, SideData& side_data)
{
	bool res = ds.GetInt8(side_data.obj_type) 
		&& ds.GetInt8s(side_data.role_name, sizeof(side_data.role_name))
		&& ds.GetInt64(side_data.role_id)
		&& ds.GetInt16(side_data.city_pos.x)
		&& ds.GetInt16(side_data.city_pos.y)
		&& ds.GetInt16(side_data.vip_level)
		&& ds.GetInt8s(side_data.alliance_name, sizeof(side_data.alliance_name))
		&& ds.GetInt32(side_data.cur_boom)
		&& ds.GetInt32(side_data.max_boom)
		&& ds.GetInt32(side_data.boom_change)
		&& ds.GetInt8(side_data.soldier_unit_num);
	for (Int32 i = 0; i < side_data.soldier_unit_num && i < FORMATION_GRID_NUM; ++i)
	{
		res &= ds.GetInt32(side_data.soldiers[i].soldier_id)
			&& ds.GetInt32(side_data.soldiers[i].soldier_num);
	}
	res &= ds.GetInt8(side_data.general_num);
	if (side_data.general_num > 0)
	{
		res &= ds.GetInt32s(side_data.generals, side_data.general_num);
	}
	res &= ds.GetInt8(side_data.is_ahead);

	return res;
}

void MailBattleContent::Reset()
{
	content_type = 0;
	battle_time = 0;
	memset(target_name, 0, sizeof(target_name));
	memset(target_alliance, 0, sizeof(target_alliance));
	memset(target_pos_name, 0, sizeof(target_pos_name));
	memset(garrison_name, 0, sizeof(garrison_name));
	battle_result = 0;
	memset(resource, 0, sizeof(resource));
	battle_id = 0;
	item_param_num = 0;
}

MailSpyRoleContent::MailSpyRoleContent()
{
	Reset();
}

bool MailSpyRoleContent::Serialize(jxsstr::Serializer& se)
{
	bool res = true;
	res &= se.PutInt16(content_type)
		&& se.PutInt8s(city_name, sizeof(city_name))
		&& se.PutInt64(spy_time)
		&& se.PutInt16(city_pos.x)
		&& se.PutInt16(city_pos.y)
		&& se.PutInt8s(alliance_name, sizeof(alliance_name))
		&& se.PutInt8s(garrison_name, sizeof(garrison_name))
		&& se.PutInt32(cur_boom)
		&& se.PutInt32(max_boom)
		&& se.PutInt32s(resource, RESOURCE_TYPE_COUNT);
	for (Int32 i = 0; i < FORMATION_GRID_NUM; ++i)
	{
		res &= se.PutInt32(army_foramt.soldier_format[i].soldier_id)
			&& se.PutInt32(army_foramt.soldier_format[i].soldier_num);
	}
	res &= se.PutInt32s(army_foramt.general_format, GENERAL_TYPE_COUNT);

	return res;
}

bool MailSpyRoleContent::Deserialize(jxsstr::Deserializer& ds)
{
	bool res = true;
	res &= ds.GetInt16(content_type)
		&& ds.GetInt8s(city_name, sizeof(city_name))
		&& ds.GetInt64(spy_time)
		&& ds.GetInt16(city_pos.x)
		&& ds.GetInt16(city_pos.y)
		&& ds.GetInt8s(alliance_name, sizeof(alliance_name))
		&& ds.GetInt8s(garrison_name, sizeof(garrison_name))
		&& ds.GetInt32(cur_boom)
		&& ds.GetInt32(max_boom)
		&& ds.GetInt32s(resource, RESOURCE_TYPE_COUNT);
	for (Int32 i = 0; i < FORMATION_GRID_NUM; ++i)
	{
		res &= ds.GetInt32(army_foramt.soldier_format[i].soldier_id)
			&& ds.GetInt32(army_foramt.soldier_format[i].soldier_num);
	}
	res &= ds.GetInt32s(army_foramt.general_format, GENERAL_TYPE_COUNT);

	return res;
}

void MailSpyRoleContent::Reset()
{
	content_type = MAIL_REPORT_TYPE_SPY_ROLE;
	memset(city_name, 0, sizeof(city_name));
	spy_time = 0;
	city_pos.x = -1;
	city_pos.y = -1;
	memset(alliance_name, 0, sizeof(alliance_name));
	memset(garrison_name, 0, sizeof(garrison_name));
	cur_boom = 0;
	max_boom = 0;
	memset(resource, 0, sizeof(resource));
	memset(&army_foramt, 0, sizeof(army_foramt));
}

MailSpyOreContent::MailSpyOreContent()
{
	Reset();
}

bool MailSpyOreContent::Serialize(jxsstr::Serializer& se)
{
	bool res = true;
	res &= se.PutInt16(content_type)
		&& se.PutInt8s(city_name, sizeof(city_name))
		&& se.PutInt64(spy_time)
		&& se.PutInt16(city_pos.x)
		&& se.PutInt16(city_pos.y)
		&& se.PutInt8s(alliance_name, sizeof(alliance_name))
		&& se.PutInt8s(garrison_name, sizeof(garrison_name))
		&& se.PutInt8(ore_type)
		&& se.PutInt16(level)
		&& se.PutInt32(resource);
	for (Int32 i = 0; i < FORMATION_GRID_NUM; ++i)
	{
		res &= se.PutInt32(army_foramt.soldier_format[i].soldier_id)
			&& se.PutInt32(army_foramt.soldier_format[i].soldier_num);
	}
	res &= se.PutInt32s(army_foramt.general_format, GENERAL_TYPE_COUNT);

	return res;
}

bool MailSpyOreContent::Deserialize(jxsstr::Deserializer& ds)
{
	bool res = true;
	res &= ds.GetInt16(content_type)
		&& ds.GetInt8s(city_name, sizeof(city_name))
		&& ds.GetInt64(spy_time)
		&& ds.GetInt16(city_pos.x)
		&& ds.GetInt16(city_pos.y)
		&& ds.GetInt8s(alliance_name, sizeof(alliance_name))
		&& ds.GetInt8s(garrison_name, sizeof(garrison_name))
		&& ds.GetInt8(ore_type)
		&& ds.GetInt16(level)
		&& ds.GetInt32(resource);
	for (Int32 i = 0; i < FORMATION_GRID_NUM; ++i)
	{
		res &= ds.GetInt32(army_foramt.soldier_format[i].soldier_id)
			&& ds.GetInt32(army_foramt.soldier_format[i].soldier_num);
	}
	res &= ds.GetInt32s(army_foramt.general_format, GENERAL_TYPE_COUNT);

	return res;
}

void MailSpyOreContent::Reset()
{
	content_type = MAIL_REPORT_TYPE_SPY_ORE;
	memset(city_name, 0, sizeof(city_name));
	spy_time = 0;
	city_pos.x = -1;
	city_pos.y = -1;
	memset(alliance_name, 0, sizeof(alliance_name));
	memset(garrison_name, 0, sizeof(garrison_name));
	ore_type = 0;
	level = 0;
	resource = 0;
	memset(&army_foramt, 0, sizeof(army_foramt));
}

MailMsgContent::MailMsgContent()
{
	Reset();
}

void MailMsgContent::Reset()
{
	content_type = 0;
	msg_len = 0;
	memset(msg, 0, sizeof(msg));
	item_param_num = 0;
	memset(item_params, 0, sizeof(item_params));
}
