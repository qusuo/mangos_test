#ifndef JXS_GENERALDATA_H_
#define JXS_GENERALDATA_H_
//author:hsj
#include "../include/game_value_def.h"
#include "../include/battle_def.h"

struct GeneralData
{
	GeneralData()
		:general_id(0)
		, store_index(0)
		, level(0)
		, grade(0)
		, general_skill_id(0)
		, soldier_skill_id(0)
		, spe_skill_id(0)
	{
		memset(attr, 0, sizeof(attr));
		memset(proficiency, 0, sizeof(proficiency));
	}
	Int32 general_id;					//配置id
	Int32 store_index;						//将领在背包中的位置

	//以下作离线记录用
	Int32 level;
	Int32 grade;
	Int32 attr[GENERAL_ATTR_TYPE_COUNT];	//属性
	Int32 proficiency[SOLDIER_TYPE_COUNT];	//相性
	Int32 general_skill_id;					//主动技
	Int32 soldier_skill_id;					//被动技
	Int32 spe_skill_id;						//特殊技
};

typedef Int32 GeneralFormat[GENERAL_TYPE_COUNT];
typedef Int64 GeneralFormatEx[GENERAL_TYPE_COUNT];

#endif