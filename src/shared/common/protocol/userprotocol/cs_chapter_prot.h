#ifndef JXS_CHAPTERPROT_H_
#define JXS_CHAPTERPROT_H_
//副本协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../game/structs/army_format_data.h"
#include "../../game/structs/army_format_data.h"
#include "cs_battle_prot.h"
#include "cs_alliance_prot.h"
#include "../../../gamelogicserver/gamelogicserver/config/reward/reward.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//获取角色副本列表
	struct CSChapterGetInfo
	{
		CSChapterGetInfo()
			:header(CHAPTER_GET_INFO_CS)
		{

		}
		prot::ProtHeader header;
		Int8 type;					// 1:战役, 2:掠夺, 3:限时
	};

	struct ProtChapterInfo
	{
		ProtChapterInfo()
			:chapter_id(0)
			,stage_num(0)
			, box_reward_num(0)
			, used_count(0)
			, all_count(0)
		{
			memset(stage_star, 0, sizeof(stage_star));
			memset(stage_status, 0, sizeof(stage_status));
			memset(box_reward_status, 0, sizeof(box_reward_status));
		}
		Int16 chapter_id;
		Int16 stage_num;
		Int16 stage_star[MAX_CHAPTER_STAGE_NUM];
		Int8 stage_status[MAX_CHAPTER_STAGE_NUM];
		Int8 box_reward_num;
		Int8 box_reward_status[CHAPTER_BOX_REWARD_NUM];
		Int32 used_count;
		Int32 all_count;
	};

	//角色所有副本列表
	struct SCChapterSendInfo
	{
		SCChapterSendInfo()
			:header(CHAPTER_GET_INFO_RET_SC)
			, chapter_num(0)
		{
		}
		prot::ProtHeader header;
		Int8 type;							// 1:战役, 2:掠夺, 3:限时
		Int32 chapter_num;
		ProtChapterInfo chapters[MAX_ROLE_CHAPTER_NUM];
	};

	//某个副本信息变更
	struct SCChapterChange
	{
		enum ChangeType
		{
			CHANGE_TYPE_INVALID,
			CHANGE_TYPE_UPDATE,
			CHANGE_TYPE_ADD,
		};
		SCChapterChange()
			:header(CHAPTER_INFO_CHANGE_SC)
			,change_type(0)
		{

		}
		prot::ProtHeader header;
		Int8 type;						// 1:战役, 2:掠夺, 3:限时
		Int8 change_type;				//1= 变更， 2 = 新加
		ProtChapterInfo chapter_info;
	};

	const Int8 CHAPTER_CHALLGENGE_GENERAL_NUM = 3;
	//挑战关卡
	struct CSChapterChallenge
	{
		//挑战类型：1=详细战报，2=简单战报，3=扫荡
		enum ChallengeType
		{
			CHALLENGE_TYPE_INVALID,
			CHALLENGE_TYPE_DETAIL,
			CHALLENGE_TYPE_SIMPLE,
			CHALLENGE_TYPE_RAID,
		};

		enum ChapterType{
			CHAPTER_TYPE_INVALID,
			CHAPTER_TYPE_WAR,		// 1:战役, 
			CHAPTER_TYPE_ROB,		// 2:掠夺, 
			CHAPTER_TYPE_TIME,		// 3:限时
			CHAPTER_TYPE_FRESH,		// 4:新手
			CHAPTER_TYPE_END,
		};

		CSChapterChallenge()
			:header(CHAPTER_CHALLENGE_STAGE_CS)
			, chapter_id(0)
			, stage_index(0)
			, challenge_type(0)
		{
		}
		prot::ProtHeader header;
		Int8 type;					// 1:战役, 2:掠夺, 3:限时
		Int16 chapter_id;
		Int8 stage_index;
		Int8 challenge_type;  //挑战类型：1=详细战报，2=简单战报，3=扫荡
		SoldierFormat soldier_list;
		Int32 general_list[CHAPTER_CHALLGENGE_GENERAL_NUM];
	};
	
	//打开奖励箱子
	struct CSChapterFetchBoxReward
	{
		CSChapterFetchBoxReward()
			:header(CHAPTER_FETCH_BOX_REWARD_CS)
			, chapter_id(0)
			, box_index(0)
		{

		}
		prot::ProtHeader header;
		Int8 type;						// 类型: 1:战役, 2;掠夺, 3:限时
		Int16 chapter_id;
		Int16 box_index;
	};

	static const Int32 MAX_CHAPTER_BOX_REWARD_ITEM_NUM = 10;
	//打开奖励箱子返回
	struct SCChapterFetchBoxReward
	{
		SCChapterFetchBoxReward()
			:header(CHAPTER_FETCH_BOX_REWARD_SC)
			, ret(0)
			, chapter_id(0)
			, box_index(0)
			, list_num(0)
		{

		}
		prot::ProtHeader header;
		Int8 type;					// 类型: 1:战役, 2;掠夺, 3:限时
		Int8 ret;					// 0:领取失败 1:领取成功
		Int16 chapter_id;
		Int16 box_index;
		Int16 list_num;
		ItemRecord item_list[MAX_CHAPTER_BOX_REWARD_ITEM_NUM];
	};

	//扫荡关卡返回
	struct SCChapterRaidStageRet
	{
		SCChapterRaidStageRet()
			:header(CHAPTER_RAID_STAGE_RET_SC)
			, res_code(0)
			, battle_index(0)
			, chapter_id(0)
			, stage_index(0)
		{

		}
		prot::ProtHeader header;
		Int8 res_code;				 // 0:失败 1:成功
		Int32 battle_index;
		Int16 chapter_id;
		Int16 stage_index;
	};

	struct RaidProtBattleResult{
		RaidProtBattleResult(){}

		SoldierFormat init_soldier_list;
		SoldierFormat left_soldier_list;
		GeneralFormat general_list;
	};

	// 扫荡战斗结果
	struct SCChapterRaidBattleReport{
		SCChapterRaidBattleReport() :header(CHAPTER_RAID_STAGE_BATTLE_REPORT_SC)
			,battle_type(0)
			,win_side(0)
			,reward_num(0)
			,battle_index(0)
			,cost_tael(0){}
		prot::ProtHeader header;
		RaidProtBattleResult atk_res;
		RaidProtBattleResult def_res;
		Int8 battle_type;
		Int8 win_side;
		Int16 reward_num;
		ItemParam rewards[Reward::MAX_REWARD_ITEM_NUM];
		Int32 battle_index;				// 第几次扫荡
		Int32 cost_tael;				// 消耗银两数量
	};

	// 购买掠夺次数
	struct CSChapterRobCountOrder
	{
		CSChapterRobCountOrder()
			:header(CHAPTER_ROB_COUNT_ORDER_CS)
			, chapter_id(0)
		{
		}
		prot::ProtHeader header;
		Int16 chapter_id;			// 1:极限, 2:军工, 3 :装备, 4:配件
	};

	// 购买掠夺次数返回
	struct SCChapterRobCountOrder{
		SCChapterRobCountOrder()
			:header(CHAPTER_ROB_COUNT_ORDER_SC)
			, ret(0)
			, chapter_id(0)
		{}
		prot::ProtHeader header;
		Int8 ret;					// 1:购买成功, 2:购买失败
		Int16 chapter_id;			// 1:极限, 2:军工, 3 :装备, 4:配件
		Int32 used_count;			// 已用次数
		Int32 all_count;			// 总次数
	};
}
#pragma pack(pop)

#endif
