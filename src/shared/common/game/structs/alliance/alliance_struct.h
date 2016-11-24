#ifndef JXS_ALLIANCESTRUCT_H_
#define JXS_ALLIANCESTRUCT_H_
#include "../../include/game_value_def.h"
#include "../../include/data_len_def.h"
#include "../../../include/value_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct AllianceStruct
{
	AllianceStruct()
		: alliance_id(0)
		, level(0)
		, alliance_leader_id(0)
		, schedule(0)
		, type(0)
		, join_type(0)
		, m_level_limit(0)
		, m_attack_power_limit(0)
		, m_assist_num(0)
		, apply_list_num(0)
		, logic_data_len(0)
	{
		memset(m_custom_member_num, 0, sizeof(m_custom_member_num));
	}

	~AllianceStruct(){}

	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	Int64	alliance_id;
	char	alliance_name[MAX_32_CHAR_LEN];
	Int32	level;
	Int64	alliance_leader_id;

	Int32	schedule;															// 建设度
	Int8	type;																// 创建类型
	Int8	join_type;															// 加入类型 (要审批?)
	Int32	m_level_limit;														// 加入等级限制
	Int32	m_attack_power_limit;												// 加入战力限制
	Int8	m_assist_num;														// 副团长数量
	RoleID	m_assist[2];														// 副团长ID列表 
	Int8	m_custom_member_num[4];												// 自定义职位数量
	RoleID	m_custom_member[4][3];												// 自定义职位id列表
	char	m_alliance_declaration[MAX_128_CHAR_LEN];							// 军团宣言
	char	m_custom_post_name[4][MAX_32_CHAR_LEN];								// 自定义职位名称
	Int32	apply_list_num;														// 申请数量
	RoleID	apply_list[MAX_ALLIANCE_APPLY_LIST_NUM];							// 申请列表

	/** format: 
	alliance_id
	schedule
	type
	join_type
	m_level_limit
	m_attackpower_limit
	m_assit_num, m_assit[2]
	m_custom_memebr_num[4]
	m_cust_member[4][3]
	m_alliance_declaration[MAX_128_CHAR_LEN]
	m_custom_post_name[4][MAX_32_CHAR_LEN]
	apply_list_num
	apply_list[MAX_ALLIANCE_APPLY_LIST_NUM]
	*/
	Int32 logic_data_len;
	Int8 logic_data[ALLIANCE_LOGIC_DATA_LEN];
};

struct AllianceActWarStruct
{
	AllianceActWarStruct():logic_data_len(0)
	{
		Reset();
	}

	bool Serialize(jxsstr::Serializer &se)
	{
		return se.PutInt32(logic_data_len) && se.PutInt8s(logic_data, logic_data_len);
	}

	bool Deserialize(jxsstr::Deserializer &ds)
	{
		return ds.GetInt32(logic_data_len) && ds.GetInt8s(logic_data, logic_data_len);
	}

	void Reset()
	{
		memset(logic_data, 0, sizeof(logic_data));
	}

	static const Int32 MAX_ACT_MATCH_NUM = 30;
	static const Int32 ALLIANCE_ACT_WAR_LOGIC_DATA_LEN = ALLIANCE_LOGIC_DATA_LEN * MAX_ACT_MATCH_NUM;

	Int32 logic_data_len;
	char logic_data[ALLIANCE_ACT_WAR_LOGIC_DATA_LEN];
};

#endif