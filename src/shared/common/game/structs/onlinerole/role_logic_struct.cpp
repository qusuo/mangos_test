#include "role_logic_struct.h"
#include "../../include/data_version_def.h"

bool RoleLogicStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret &= se.PutInt32(DATA_VERSION_CURRENT)
		&& se.PutInt8(avatar)
		&& se.PutInt16Vector(day_count_data, MAX_DAYCOUNT_COUNT_ID)
		&& se.PutInt16(wartech_num);
	for (Int16 i = 0; i < wartech_num; ++i)
	{
		ret &= se.PutInt16(wartech[i].wartech_id)
			&& se.PutInt16(wartech[i].level);
	}
	ret &= se.PutInt16(soldier_wartech_num);
	for (Int16 i = 0; i < soldier_wartech_num; ++i)
	{
		ret &= se.PutInt16(soldier_wartech[i].soldier_id)
			&& se.PutInt16(soldier_wartech[i].grid_num);
		if (soldier_wartech[i].grid_num > 0 && soldier_wartech[i].grid_num < MAX_SOLDIER_WARTECH_NUM)
		{
			ret &= se.PutInt16Vector(soldier_wartech[i].grid_state, soldier_wartech[i].grid_num);
		}
	}

	//将领
	ret &= se.PutInt32(comm_recruit_num)
		&& se.PutInt32(advance_recruit_num)
		&& se.PutInt32(free_comm_recruit_num)
		&& se.PutInt32(free_advance_recruit_num)
		&& se.PutInt32(free_comm_avail_recruit_num)
		&& se.PutInt32(free_advance_avail_recruit_num)
		&& se.PutInt32(compose_num)
		&& se.PutInt64(last_free_comm_recruit_count_point)
		&& se.PutInt64(last_free_advance_recruit_count_point);

	//禁言
	ret &= se.PutInt64(silence_time);

	ret &= se.PutInt8(knap_role_equip_buy_times);

	ret &= se.PutInt32(conduct_step)
		&& se.PutInt32(conduct_substep)
		&& se.PutInt32(conduct_param);

	ret = se.PutInt8(format_num);
	for (Int8 i = 0; i < format_num && i < MAX_SAVE_FORMAT_NUM; ++i)
	{
		for (Int32 j = 0; j < FORMATION_GRID_NUM; ++j)
		{
			ret &= se.PutInt32(army_formats[i].soldier_format[j].soldier_id)
				&& se.PutInt32(army_formats[i].soldier_format[j].soldier_num);
		}
		ret &= se.PutInt32s(army_formats[i].general_format, GENERAL_TYPE_COUNT);
	}

	return ret;
}

bool RoleLogicStruct::Deserialize(jxsstr::Deserializer& ds)
{
	bool ret = true;
	Int16 tmp = 0;
	ret &= ds.GetInt32(data_version)
		&& ds.GetInt8(avatar)
		&& ds.GetInt16Vector(day_count_data, MAX_DAYCOUNT_COUNT_ID, tmp)
		&& ds.GetInt16(wartech_num);
	for (Int16 i = 0; i < wartech_num; ++i)
	{
		ret &= ds.GetInt16(wartech[i].wartech_id)
			&& ds.GetInt16(wartech[i].level);
	}
	ret &= ds.GetInt16(soldier_wartech_num);
	for (Int16 i = 0; i < soldier_wartech_num; ++i)
	{
		ret &= ds.GetInt16(soldier_wartech[i].soldier_id)
			&& ds.GetInt16(soldier_wartech[i].grid_num);
		if (soldier_wartech[i].grid_num > 0 && soldier_wartech[i].grid_num < MAX_SOLDIER_WARTECH_NUM)
		{
			ret &= ds.GetInt16Vector(soldier_wartech[i].grid_state, soldier_wartech[i].grid_num, tmp);
		}
	}

	//将领
	ret &= ds.GetInt32(comm_recruit_num)
		&& ds.GetInt32(advance_recruit_num)
		&& ds.GetInt32(free_comm_recruit_num)
		&& ds.GetInt32(free_advance_recruit_num)
		&& ds.GetInt32(free_comm_avail_recruit_num)
		&& ds.GetInt32(free_advance_avail_recruit_num)
		&& ds.GetInt32(compose_num)
		&& ds.GetInt64(last_free_comm_recruit_count_point)
		&& ds.GetInt64(last_free_advance_recruit_count_point);

	//禁言
	ret &= ds.GetInt64(silence_time);

	ret &= ds.GetInt8(knap_role_equip_buy_times);

	ret &= ds.GetInt32(conduct_step)
		&& ds.GetInt32(conduct_substep)
		&& ds.GetInt32(conduct_param);
	
	ret = ds.GetInt8(format_num);
	for (Int8 i = 0; i < format_num && i < MAX_SAVE_FORMAT_NUM; ++i)
	{
		for (Int32 j = 0; j < FORMATION_GRID_NUM; ++j)
		{
			ret &= ds.GetInt32(army_formats[i].soldier_format[j].soldier_id)
				&& ds.GetInt32(army_formats[i].soldier_format[j].soldier_num);
		}	
		ret &= ds.GetInt32s(army_formats[i].general_format, GENERAL_TYPE_COUNT);
	}

	return ret;
}

void RoleLogicStruct::Reset()
{
	data_version = 0;
	avatar = 0;
	memset(day_count_data, 0, sizeof(day_count_data));
	wartech_num = 0;
	soldier_wartech_num = 0;
	memset(wartech, 0, sizeof(wartech));
	memset(soldier_wartech, 0, sizeof(soldier_wartech));

	organ_buy_limit_num = 0;
	memset(organ_buy_limit, 0, sizeof(organ_buy_limit));

	comm_recruit_num = 0;
	advance_recruit_num = 0;
	free_comm_recruit_num = 0;
	free_advance_recruit_num = 0;
	free_comm_avail_recruit_num = 0;
	free_advance_avail_recruit_num = 0;
	compose_num = 0;
	last_free_comm_recruit_count_point = 0;
	last_free_advance_recruit_count_point = 0;
	silence_time = 0;

	knap_role_equip_buy_times = 0;

	conduct_step = 0;
	conduct_substep = 0;
	conduct_param = 0;

	format_num = 0;

}
