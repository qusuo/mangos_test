#ifndef JXS_ARMYDATA_H_
#define JXS_ARMYDATA_H_
//author:hsj

#include "../include/server_def.h"
#include "../include/battle_def.h"
#include "../include/game_value_def.h"

typedef jxs::Array<Int32, MAX_SOLDIER_RESTRICT_NUM> RestrictIdArray;
typedef jxs::Array<Int32, MAX_SOLDIER_RESTRICT_NUM> AuraIdArray;

//士兵
struct ArmyItemData
{
	ArmyItemData();
	~ArmyItemData();
	void Reset();
	Int8 item_pos;
	Int8 proficiency;			//需要相性
	Int32 soldier_cfg_id;
	Int32 soldier_num;
	Int16 soldier_type;
	Int16 soldier_class;
	Int16 soldier_level;
	Int16 atk_mode;
	Int32 nor_attr[SOLDIER_NOR_ATTR_COUNT];		//基础值
	Int32 nor_attr_add[NOR_ATTR_COUNT];			//最后加成值 在战斗之前就算好，基础值+科技加成+装备加成
	RestrictIdArray restrict_ids;	//
	AuraIdArray aura_ids;		//
};

// 武将基础加成（在战斗之前拿数据时就计算好）
struct ArmyGeneralData
{
	ArmyGeneralData();
	~ArmyGeneralData();
	void Reset();
	Int32 general_id;
	Int32 store_index;
	Int32 level;
	Int32 grade;
	Int32 general_attr[GENERAL_ATTR_TYPE_COUNT];		//武装属性
	Int32 proficiency[SOLDIER_TYPE_COUNT];				//相性
	Int32 general_skill;								//将领主动技能
	Int32 soldier_skill;								//将领被动技能（影响士兵）
};

struct ArmyData
{
	enum ArmyType
	{
		ARMY_TYPE_INVALID,
		ARMY_TYPE_ROLE,
		ARMY_TYPE_MONSTER,
		ARMY_TYPE_ROBOT,
	};
	ArmyData();
	~ArmyData();
	void Reset();
	Int64 obj_id;			//角色id或怪物id
	Int16 obj_level;
	RoleName obj_name;
	Int8 army_type;			//角色，怪物，机器人
	Int16 outer_team_pos;	//士兵队伍位置
	ArmyItemData item_data[FORMATION_GRID_NUM];
	ArmyGeneralData general_data[GENERAL_TYPE_COUNT];	//武将基础属性
	Int32 general_add[NOR_ATTR_COUNT];
	Int32 battle_type_hurt_add;			//战役加成
	Int32 battle_type_hurt_substract;	//战役加成
};


#endif