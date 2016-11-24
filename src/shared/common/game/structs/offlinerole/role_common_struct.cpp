#include "role_common_struct.h"

RoleCommonStruct::RoleCommonStruct()
{
	Reset();
}

RoleCommonStruct::~RoleCommonStruct()
{

}

bool RoleCommonStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret &= se.PutInt64(last_calc_resoure_time)
		&& se.PutInt16(building_list_buy_num)
		&& se.PutInt64(last_restore_energy_time)
		&& se.PutInt64(last_restore_boom_time)
		&& se.PutInt16Vector(scheme_develop_arr, SCHEME_TYPE_COUNT)
		&& se.PutInt32(leadership_add_times)
		&& se.PutInt32Vector(resource_gather, RESOURCE_TYPE_COUNT);

	ret &= se.PutInt8(skills_num);
	for (Int8 i = 0; i < skills_num; ++i)
	{
		ret &= se.PutInt16(role_skills[i]);
	}

	ret &= se.PutInt32(general_cf)
		&& se.PutInt32(general_skill_lead_num)
		&& se.PutInt32s(general_attr, GENERAL_ATTR_TYPE_COUNT)
		&& se.PutInt32(total_cf);

	return ret;
}

bool RoleCommonStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	Int16 tmp_len = 0;
	ret &= ds.GetInt64(last_calc_resoure_time)
		&& ds.GetInt16(building_list_buy_num)
		&& ds.GetInt64(last_restore_energy_time)
		&& ds.GetInt64(last_restore_boom_time)
		&& ds.GetInt16Vector(scheme_develop_arr, SCHEME_TYPE_COUNT, tmp_len)
		&& ds.GetInt32(leadership_add_times)
		&& ds.GetInt32Vector(resource_gather, RESOURCE_TYPE_COUNT, tmp_len);

	ret &= ds.GetInt8(skills_num);
	for (Int8 i = 0; i < skills_num && i < ROLE_SKILL_COUNT; ++i)
	{
		ret &= ds.GetInt16(role_skills[i]);
	}


	ret &= ds.GetInt32(general_cf)
		&& ds.GetInt32(general_skill_lead_num)
		&& ds.GetInt32s(general_attr, GENERAL_ATTR_TYPE_COUNT)
		&& ds.GetInt32(total_cf);

	return ret;
}

void RoleCommonStruct::Reset()
{
	last_calc_resoure_time = 0;
	building_list_buy_num = 0;
	last_restore_boom_time = 0;
	last_restore_energy_time = 0;
	memset(scheme_develop_arr, 0, sizeof(scheme_develop_arr));	
	skills_num = 0;
	memset(role_skills, 0, sizeof(role_skills));
	leadership_add_times = 0;
	memset(resource_gather, 0, sizeof(resource_gather));
	general_cf = 0;
	general_skill_lead_num = 0;
	memset(general_attr, 0, sizeof(general_attr));
	total_cf = 0;
}