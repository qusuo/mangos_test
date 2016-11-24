#ifndef JXS_ROLECOMMONSTRUCT_H_
#define JXS_ROLECOMMONSTRUCT_H_
//author:hunter(757155633@qq.com)
//离线公共属性

#include "../../include/game_value_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../attr_data.h"



struct RoleCommonStruct
{
	RoleCommonStruct();
	~RoleCommonStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	
	Int64 last_calc_resoure_time;	//上一次更新资源时间
	Int16 building_list_buy_num;    //已购买建筑队列数
	Int64 last_restore_energy_time; //最后一次恢复军令时间
	Int64 last_restore_boom_time;	//最后一次恢复繁荣度的时间
	
	Int32 leadership_add_times;		//统帅提升次数

	Int16 scheme_develop_arr[SCHEME_TYPE_COUNT];	//计略俯
	Int8 skills_num;
	Int16 role_skills[ROLE_SKILL_COUNT];			//角色技能
	Int32 resource_gather[RESOURCE_TYPE_COUNT];		//资源采集量

	//战力相关
	Int32 general_cf;
	Int32 general_skill_lead_num;					//带兵加成
	Int32 general_attr[GENERAL_ATTR_TYPE_COUNT];  //部队将领加成属性
	Int32 total_cf;
};

#endif