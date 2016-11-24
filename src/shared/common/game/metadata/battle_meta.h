#ifndef JXS_BATTLEMETA_H_
#define JXS_BATTLEMETA_H_
//author:hsj

#include "../include/server_def.h"
#include "../include/battle_def.h"

#pragma pack(push) 
#pragma pack(4)

struct BattleUnitMeta
{
	BattleUnitMeta() :unit_type(0), unit_cfg_id(0), cur_hp(0), cap_hp(0), cur_ap(0), cur_dp(0){}
	Int32 unit_type;
	Int32 unit_cfg_id; //如果是英雄，就是英雄id，如果是生物，就是生物id
	Int32 cur_hp;
	Int32 cap_hp;
	Int32 cur_ap;
	Int32 cur_dp;

};

struct ArmyMeta
{
	ArmyMeta()
		:role_id(0)
		//, army_type(0)
		, mp(0)
		,card_num(0)
	{
		role_name[0] = '\0';
		memset(card_group, 0, sizeof(card_group));
	}
	RoleID role_id;
	RoleName role_name;
	//Int32 army_type; //1 = 机器人， 2 = 玩家
	Int32 mp;
	BattleUnitMeta hero;
	Int32 card_num;
	Int32 card_group[MAX_CARD_NUM_PER_GROUP];
};

struct BattleMeta
{
	BattleMeta() :battle_type(0), offensive_side(0){}
	Int32 battle_type;
	Int32 offensive_side;
	ArmyMeta army_a;
	ArmyMeta army_b;
};

#pragma pack(pop)

#endif