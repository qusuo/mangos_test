#ifndef JXS_FAMOUSCITYPROT_H_
#define JXS_FAMOUSCITYPROT_H_
//名城协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"
#include "../../game/structs/army_format_data.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//请求名城基本信息
	struct CSFamousCityReqBaseInfo
	{
		CSFamousCityReqBaseInfo()
			:header(FAMOUSCITY_REQ_BASE_INFO_CS)
			, city_id(0)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
	};

	//返回名城基本信息
	struct SCFamousCitySendBaseInfo
	{
		SCFamousCitySendBaseInfo()
			:header(FAMOUSCITY_SEND_BASE_INFO_SC)
			, city_id(0)
			, soldier_num(0)
			, defend_points(0)
			, city_state(0)
			, reward_state(0)
			, rank(0)
			, score(0)
		{
			memset(stage_soldier_nums, 0, sizeof(stage_soldier_nums));
		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 soldier_num;		//兵数
		Int32 defend_points;	//城防值
		Int16 city_state;		//城池状态 0=可攻击，1=已破城
		Int16 reward_state;		//排行奖励状态，0=不可领，1=有奖励可领，2=已领
		Int32 rank;				//玩家排名
		Int32 score;			//玩家积分
		Int32 stage_soldier_nums[MAX_FAMOUSCITY_STAGE_NUM];//每个防守点的兵数
	};

	//请求防守点信息
	struct CSFamousCityReqStageInfo
	{
		CSFamousCityReqStageInfo()
			:header(FAMOUSCITY_REQ_STAGE_INFO_CS)
			, city_id(0)
			, stage_index(0)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 stage_index;	//防守点序号 从1开始
	};

	//返回防守点信息
	struct SCFamousCitySendStageInfo
	{
		SCFamousCitySendStageInfo()
			:header(FAMOUSCITY_SEND_STAGE_INFO_SC)
			, city_id(0)
			, stage_index(0)
			, spy_time(0)
		{
			memset(generals, 0, sizeof(generals));
		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 stage_index;	//防守点序号
		Int32 spy_time;		//侦查时间
		SoldierFormat soldiers;
		Int32 generals[GENERAL_TYPE_COUNT];
		//GeneralFormatData generals[GENERAL_TYPE_COUNT];
	};

	//请求积分排行信息
	struct CSFamousCityReqRankInfo
	{
		CSFamousCityReqRankInfo()
			:header(FAMOUSCITY_REQ_RANK_INFO_CS)
			, city_id(0)
			, num_per_page(0)
			, page_index(0)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 num_per_page;		//每页数量
		Int32 page_index;		//第几页
		
	};

	struct FamousCityScoreInfo
	{
		FamousCityScoreInfo()
			:score(0)
			, rank_index(0)
		{
			memset(role_name, 0, sizeof(role_name));
		}
		RoleName role_name;
		Int32 score;
		Int32 rank_index;	//排第几
	};

	//返回积分排行信息
	const Int32 MAX_RANK_INFO_NUM_PER_PAGE = 50;
	struct SCFamousCityReqRankInfo
	{
		SCFamousCityReqRankInfo()
			:header(FAMOUSCITY_SEND_RANK_INFO_SC)
			, city_id(0)
			, total_info_num(0)
			, page_index(0)
			, info_num(0)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 total_info_num;	//总人数
		Int32 page_index;
		Int32 info_num;
		FamousCityScoreInfo infos[MAX_RANK_INFO_NUM_PER_PAGE];
	};

	//请求名城相关的行军列表
	struct CSFamousCityReqArmyList
	{
		CSFamousCityReqArmyList()
			:header(FAMOUSCITY_REQ_ARMY_LIST_CS)
			, city_id(0)
			, data_version(0)
			, begin_pos(INVALID_POS)
		{
		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 data_version;
		Int32 begin_pos;
	};

	enum FamousCityArmyType
	{
		FAMOUSCITY_ARMY_TYPE_INVALID,
		FAMOUSCITY_ARMY_TYPE_SENDING,
		FAMOUSCITY_ARMY_TYPE_BACKING,
	};

	struct FamousCityArmyInfo
	{
		FamousCityArmyInfo()
			:src_x(0)
			, src_y(0)
			, dest_x(0)
			, dest_y(0)
			, progress_id(0)
			, total_time_len(0)
			, left_time_len(0)
			, army_type(0)
			, soldier_id(0)
		{
			memset(army_name, 0, sizeof(army_name));
		}
		Int16 src_x;
		Int16 src_y;
		Int16 dest_x;
		Int16 dest_y;
		Int64 progress_id;		//
		Int32 total_time_len;
		Int32 left_time_len;
		Int32 army_type;		//1=攻打部队，2=返回部队，3=名城部队
		RoleName army_name;
		Int32 soldier_id;
	};

	const Int32 FAMOUS_CITY_MAX_SEND_ARMY_NUM = 30;
	//返回名城相关的行军列表
	struct SCFamousCitySendArmyList
	{
		SCFamousCitySendArmyList()
			:header(FAMOUSCITY_SEND_ARMY_LIST_SC)
			, city_id(0)
			, data_version(0)
			, total_num(0)
			, begin_pos(0)
			, army_num(0)
		{
		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 data_version;
		Int32 total_num;
		Int32 begin_pos;
		Int32 army_num;
		FamousCityArmyInfo armies[FAMOUS_CITY_MAX_SEND_ARMY_NUM];
	};

	//请求名城相关的某个行军具体信息
	struct CSFamousCityReqArmyInfo
	{
		CSFamousCityReqArmyInfo()
			:header(FAMOUSCITY_REQ_ARMY_INFO_CS)
			, city_id(0)
			, progress_id(0)
			, army_type(0)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
		Int64 progress_id;
		Int32 army_type;
	};

	//返回名城相关的某个行军具体信息
	struct SCFamousCitySendArmyInfo
	{
		SCFamousCitySendArmyInfo()
			:header(FAMOUSCITY_SEND_ARMY_INFO_SC)
			, city_id(0)
			, progress_id(0)
			, spy_time(0)
			, src_x(0)
			, src_y(0)
			, dest_x(0)
			, dest_y(0)
			, total_time_len(0)
			, left_time_len(0)
			, army_type(0)
			, army_level(0)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
		Int64 progress_id;
		Int32 spy_time;
		Int16 src_x;
		Int16 src_y;
		Int16 dest_x;
		Int16 dest_y;
		Int32 total_time_len;
		Int32 left_time_len;
		Int32 army_type;	
		Int32 army_level;
		RoleName army_name;
		ArmyFormatData army_info;
	};
		
	//领取积分排行奖励
	struct CSFamousCityFetchRankReward
	{
		CSFamousCityFetchRankReward()
			:header(FAMOUSCITY_FETCH_RANK_REWARD_CS)
			, city_id(0)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
	};

	//领取积分排行奖励结果返回
	enum FamousCityRewardState
	{
		FAMOUSCITY_REWARD_STATE_CANT_FETCH,
		FAMOUSCITY_REWARD_STATE_CAN_FETCH,
		FAMOUSCITY_REWARD_STATE_FETCHED,
	};
	struct SCFamousCityFetchRankRewardRet
	{
		SCFamousCityFetchRankRewardRet()
			:header(FAMOUSCITY_FETCH_RANK_REWARD_RET_SC)			
			, city_id(0)
			, reward_index(0)
			, res_code(-1)
		{

		}
		prot::ProtHeader header;	
		Int32 city_id;
		Int32 reward_index;
		Int32 res_code;		//0 = 成功， -1=失败
	};

	//请求攻击防守点
	struct CSFamousCityAttackStage
	{
		CSFamousCityAttackStage()
			:header(FAMOUSCITY_ATTACK_STAGE_CS)
			, city_id(0)
			, stage_index(0)
		{
		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 stage_index;
		ArmyFormatData army_format;
	};

	//请求攻击防守点结果返回
	struct SCFamousCityAttackStageRet
	{
		SCFamousCityAttackStageRet()
			:header(FAMOUSCITY_ATTACK_STAGE_RET_SC)
			, city_id(0)
			, stage_index(0)
			, res_code(-1)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
		Int32 stage_index;
		Int32 res_code;		//0 = 成功， -1=失败
	};

	//请求攻击行军
	struct CSFamousCityInterCept
	{
		CSFamousCityInterCept()
			:header(FAMOUSCITY_INTERCEPT_CS)
			, city_id(0)
			, progress_id(0)
			, army_type(0)
		{
		}
		prot::ProtHeader header;
		Int32 city_id;
		Int64 progress_id;
		Int32 army_type;
		ArmyFormatData army_format;
	};

	//请求攻击行军返回
	struct SCFamousCityInterceptRet
	{
		SCFamousCityInterceptRet()
			:header(FAMOUSCITY_INTERCEPT_SC)
			, city_id(0)
			, progress_id(0)
			, army_type(0)
			, res_code(-1)
		{

		}
		prot::ProtHeader header;
		Int32 city_id;
		Int64 progress_id;
		Int32 army_type;
		Int32 res_code;		//0 = 成功， -1=失败
	};

	//请求名城列表
	struct CSFamousCityGetCityList
	{
		CSFamousCityGetCityList()
			:header(FAMOUSCITY_GET_CITY_LIST_CS)
		{
		}
		prot::ProtHeader header;
	};

	//返回名城列表
	struct FamousCityInfo
	{
		enum 
		{
			F_CITY_STATE_NORMAL = 1,
			F_CITY_STATE_FIGHTING,
			F_CITY_STATE_WEAK,
			F_CITY_STATE_BROKEN,
		};
		FamousCityInfo()
			:city_id(0)
			, city_level(0)
			, x(0)
			, y(0)
			, city_state(0)
			, reward_state(0)
		{
			memset(city_name, 0, sizeof(city_name));
		}
		Int32 city_id;
		Int32 city_level;
		Int16 x;
		Int16 y;
		char city_name[MAX_32_CHAR_LEN];
		Int16 city_state;	//1=普通，2=燃烧中，3=已破城
		Int16 reward_state;	//0=不可领取，1=可领取，2=已领取
	};
	struct SCFamousCitySendCityList
	{
		static const Int32 MAX_FAMOUS_CITY_NUM = 100;
		SCFamousCitySendCityList()
			:header(FAMOUSCITY_SEND_CITY_LIST_SC)
		{
		}
		prot::ProtHeader header;
		Int32 city_num;
		FamousCityInfo cities[MAX_FAMOUS_CITY_NUM];
	};

}
#pragma pack(pop)

#endif
