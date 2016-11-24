#ifndef JXS_RANKSTRUCT_H_
#define JXS_RANKSTRUCT_H_
//author:xsq

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../include/game_value_def.h"

enum E_RANK_TYPE{
	RANK_TYPE_ATTACK_POWER,		// 战力
	RANK_TYPE_FORM,				// 编制
	RANK_TYPE_CHAPTER_STAR,		// 关卡 :done
	RANK_TYPE_HNOR,				// 荣誉	:done
	RANK_TYPE_ATTACK,			// 攻击强化
	RANK_TYPE_BLOW,				// 暴击强化
	RANK_TYPE_DODGE,			// 闪避强化
	RANK_TYPE_ARENA_FORM_ATTAKC_POWE,			// 竞技场(阵型战力)
	RANK_TYPE_ACHIVEMENT,		// 成就
	RANK_TYPE_BOOM,				// 繁荣度 :done

	RANK_TYPE_TONGLUI,			// 统率等级
	RANK_TYPE_SKILL,			// 技能等级和 :done
	RANK_TYPE_TECH,				// 科技等级和 :done
	RANK_TYPE_GENERAL,			// SS 将领数
	RANK_TYPE_EQUIP,			// 穿戴紫装等级和 :done (当前计算所有装备等级和)
	RANK_TYPE_FITTING,			// 配件强度和
	RANK_TYPE_WARTECH,			// 军工科技点数和
	RANK_TYPE_ENERGY_CRISTAL_INLAY,		// 能晶镶嵌等级和
	RANK_TYPE_ARENA_WIN_NUM,			// 竞技场连胜次数
	RANK_TYPE_ALLIANCE_DONATE,			// 军团周捐献贡献
	RANK_TYPE_WORLD_BOSS_HURT,			// 世界boss1晚伤害
	RANK_TYPE_MILITARY_ORE_SCORE,		// 军事矿区1周积分
	RANK_TYPE_ADVANED_TANK_DESTROY_NUM,	// 高阶坦克击毁数
	RANK_TYPE_RUIN_NUM,					// 废墟玩家次数
	RANK_TYPE_FORM_EXP,					// 1场战斗编制经验
	RANK_TYPE_ALLIANCE_WAR_SCORE,		// 军团争霸1晚积分
	RANK_TYPE_LOGIN_DAY,				// 登录天数
	RANK_TYPE_COUNT,
};

struct RankStruct
{
	typedef struct RankRecord{
		RoleID role_id;
		Int32 value;
		Int64 time;
	}RankRecord;

	RankStruct(){}
	~RankStruct(){}
	bool Serialize(jxsstr::Serializer& se){
		for (Int32 i = 0; i < RANK_TYPE_COUNT; i++)
			if (!se.PutInt8s((char*)rank_record[i], sizeof(rank_record[i])))
				return false;
		return true;
	}

	bool Deserialize(jxsstr::Deserializer& ds){
		for (Int32 i = 0; i < RANK_TYPE_COUNT; i++)
			if (!ds.GetInt8s((char*)rank_record[i], sizeof(rank_record[i])))
				return false;
		return true;
	}
	void Reset();

	RankRecord rank_record[RANK_TYPE_COUNT][MAX_RANK_NUM];
};

#endif