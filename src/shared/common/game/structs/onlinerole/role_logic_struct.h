#ifndef JXS_ROLELOGICSTRUCT_H_
#define JXS_ROLELOGICSTRUCT_H_

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../include/game_value_def.h"
#include "../army_format_data.h"
#include "../wartech_data.h"

struct RoleLogicStruct
{
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	Int32 data_version;

	Int8 avatar;
	Int16 day_count_data[MAX_DAYCOUNT_COUNT_ID];		//每日记数，次数有可能超过127
	//Int16 role_skills[DATA_ROLE_SKILL_NUM];				//角色技能

	Int16 wartech_num;												
	WarTechData wartech[MAX_WARTECH_NUM];	//兵法等级
	Int16 soldier_wartech_num;
	SoldierWarTechData soldier_wartech[MAX_SOLDIER_WARTECH_NUM]; //士兵装备兵法
	//编制商城个人限购计数
	Int16 organ_buy_limit_num;
	Int32 organ_buy_limit[MAX_POS_GOOD_LEN];

	//将领
	Int32 comm_recruit_num;
	Int32 advance_recruit_num;
	Int32 free_comm_recruit_num;
	Int32 free_advance_recruit_num;
	Int32 free_comm_avail_recruit_num;
	Int32 free_advance_avail_recruit_num;
	Int32 compose_num;
	Int64 last_free_comm_recruit_count_point;
	Int64 last_free_advance_recruit_count_point;

	//禁言
	Int64 silence_time;		//禁言结束时间

	//背包扩充
	Int8 knap_role_equip_buy_times;	//奇门扩充次数，最大10次

	//新手引导
	Int32 conduct_step;
	Int32 conduct_substep;
	Int32 conduct_param;

	//套用阵型保存
	Int8 format_num;
	ArmyFormatData army_formats[MAX_SAVE_FORMAT_NUM];
};


#endif