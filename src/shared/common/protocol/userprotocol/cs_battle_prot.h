#ifndef JXS_CSBATTLEPROT_H_
#define JXS_CSBATTLEPROT_H_
//author:hsj
//战场协议 这里只定义协议的头部

#include "../cs_protocol_def.h"
#include "../../game/include/battle_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../game/structs/item_data.h"

#pragma pack(push) 
#pragma pack(1)

namespace csprot
{
	struct CSBattleTest
	{
		CSBattleTest()
			:header(BATTLE_TEST_CS)
		{

		}
		prot::ProtHeader header;
	};

	struct ProtBattleItem
	{
		ProtBattleItem()
		{
			Reset();
		}
		~ProtBattleItem(){};
		void Reset()
		{
			cfg_id = 0;
			pos = 0;
			soldier_num = 0;
			//soldier_hp = 0;
		}

		Int32 cfg_id;
		Int8 pos;
		Int32 soldier_num;
		//Int32 soldier_hp;
	};
	struct ProtBattleGeneral
	{
		ProtBattleGeneral()
			:general_id(0)
			, level(0)
			, grade(0)
		{

		}
		Int32 general_id;
		Int32 level;
		Int32 grade;
	};

	struct ProtBattleArmy
	{
		ProtBattleArmy()
		{
			Reset();
		}
		~ProtBattleArmy(){}
		void Reset()
		{		
			//obj_id = 0;
			obj_level = 0;
			memset(obj_name, 0, sizeof(obj_name));
			memset(generals, 0, sizeof(generals));
			item_num = 0;
			for (Int32 i = 0; i < FORMATION_GRID_NUM; ++i)
			{
				items[i].Reset();
			}
		}
		
		//Int64 obj_id;
		Int16 obj_level;
		RoleName obj_name;
		ProtBattleGeneral generals[GENERAL_TYPE_COUNT];
		Int8 item_num;
		ProtBattleItem items[FORMATION_GRID_NUM];
	};

	enum ProtActionType
	{
		PROT_ACTION_TYPE_INVALID,
		PROT_ACTION_TYPE_NORMAL,
		PROT_ACTION_TYPE_DODGE,
		PROT_ACTION_TYPE_BLOW,
	};

	//第一轮（美人对决）动作
	struct ProtFirstRoundTarget
	{
		ProtFirstRoundTarget()
		{
			memset(src_general_attr, 0, sizeof(src_general_attr));
			memset(change_general_attr, 0, sizeof(change_general_attr));
		}
		Int32 src_general_attr[GENERAL_ATTR_TYPE_COUNT];		//初始属性
		Int32 change_general_attr[GENERAL_ATTR_TYPE_COUNT];		//对冲属性（负数）
	};

	struct ProtBattleTarget
	{
		ProtBattleTarget()
		{
			Reset();
		}
		~ProtBattleTarget(){}
		void Reset()
		{
			tar_pos = 0;
			action_type = 0;
			hurt = 0;
			left_soldier_num = 0;
			skill_num = 0;
			memset(skill_list, 0, sizeof(skill_list));
		}
		Int8 tar_pos;
		Int8 action_type;
		Int32 hurt;
		Int32 left_soldier_num;
		Int8 skill_num;
		Int16 skill_list[MAX_CONCURRENT_SKILL_NUM];
	};

	const Int32 MAX_BATTLE_TARGET_NUM = 10;
	struct ProtBattleAction
	{
		ProtBattleAction()
		{
			Reset();
		}
		~ProtBattleAction(){}
		void Reset()
		{
			round_index = 0;
			src_pos = 0;
			skill_num = 0;
			memset(skill_list, 0, sizeof(skill_list));
			target_num = 0;
			for (Int32 i = 0; i < MAX_BATTLE_TARGET_NUM; ++i)
			{
				battle_targets[i].Reset();
			}
		}
		Int16 round_index;
		Int8 src_pos;
		Int8 skill_num;
		Int16 skill_list[MAX_CONCURRENT_SKILL_NUM];
		Int8 target_num;		
		ProtBattleTarget battle_targets[MAX_BATTLE_TARGET_NUM];
	};

	const Int32 MAX_BATTLE_ACTION_NUM = MAX_BATTLE_ROUND_NUM * FORMATION_GRID_NUM * 2;
	struct SCBattleReport
	{
		SCBattleReport()
			:header(BATTLE_REPORT_SC)
		{
			Reset();
		}
		void Reset()
		{
			battle_type = 0;
			atk_army.Reset();
			def_army.Reset();
			action_num = 0;
			for (Int32 i = 0; i < MAX_BATTLE_ACTION_NUM; ++i)
			{
				battle_actions[i].Reset();
			}
			win_side = 0;
		}
		prot::ProtHeader header;
		Int8 battle_type;
		ProtBattleArmy atk_army;
		ProtBattleArmy def_army;
		Int16 action_num;
		ProtBattleAction battle_actions[MAX_BATTLE_ACTION_NUM];
		Int8 win_side;

	};

	struct ProtBattleResult
	{
		ProtBattleResult()
			:init_soldier_num(0)
			, left_soldier_num(0)
		{
			memset(store_indexs, 0, sizeof(store_indexs));
		}
		~ProtBattleResult(){}

		Int32 init_soldier_num;
		Int32 left_soldier_num;
		Int32 store_indexs[GENERAL_TYPE_COUNT];
	};

	struct BattleExpResult
	{
		BattleExpResult()
			:old_level(0)
			, new_level(0)
			, old_exp(0)
			, new_exp(0)
		{

		}
		Int32 old_level;
		Int32 new_level;
		Int32 old_exp;
		Int32 new_exp;
	};

	struct SCBattleResult
	{
		static const Int32 EXP_RES_NUM = 4;
		SCBattleResult()
			:header(BATTLE_RESULT_SC)
			, exp_get(0)
			, battle_type(0)
			, win_side(0)
			, reward_num(0)
		{
		}
		prot::ProtHeader header;
		ProtBattleResult atk_res;
		ProtBattleResult def_res;
		Int32 exp_get;
		BattleExpResult exp_res[EXP_RES_NUM];
		Int8 battle_type;
		Int8 win_side;
		Int16 reward_num;
		ItemParam rewards[MAX_BATTLE_REWARD_NUM];
	};

	//查看战报
	struct CSBattleOpenReport
	{
		CSBattleOpenReport()
			:header(BATTLE_OPEN_REPORT_CS)
			, report_type(0)
			, report_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 report_type; //1=普通战报， 2=新手战报
		Int64 report_id;
		
	};

	//查看战报返回
	struct SCBattleOpenReportRet
	{
		enum ResCode
		{
			RESCODE_SUCCC = 0,
			RESCODE_NOT_EXIST = -1,
		};
		SCBattleOpenReportRet()
			:header(BATTLE_OPEN_REPORT_RET_SC)
			, res_code(0)
		{
			memset(report_data, 0, sizeof(report_data));
		}
		prot::ProtHeader header;
		Int32 res_code;
		char report_data[sizeof(SCBattleReport)];
	};
}
#pragma pack(pop)

#endif
