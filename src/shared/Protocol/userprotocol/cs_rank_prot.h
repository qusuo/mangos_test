#ifndef JXS_RANK_PROTOCOL_H_
#define JXS_RANK_PROTOCOL_H_

// 排行榜

#include "../cs_global_protocol_def.h"
#include "../../game/include/game_value_def.h"
#include "../../game/include/data_len_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot{
	// 排行榜
	struct CSRank{
		CSRank()
			: header(csprot::RANK_CS)
			, type(-1)
		{}
		prot::ProtHeader header;
		int32 type;
	};

	typedef struct RankType{
		RankType(){}
		RoleID role_id;
		RoleName name;
		int32 level;		// 默认为玩家等级, 编制榜(编制称号), 攻击/暴击/闪避 强化时(装备等级和)
		int32 rank_factor;
	}RankType;

	struct SCRank{
		SCRank()
			: header(csprot::RANK_SC)
			, type(-1)
		{}
		prot::ProtHeader header;
		int32 type;							// 排行类型
		int32 num;							// 列表长度
		RankType rank_list[MAX_RANK_NUM];	// 排行列表
	};

	struct SCRankTop{
		SCRankTop()
		:header(csprot::RANK_TOP_SC){}

		prot::ProtHeader header;
		int32 type;				// 巅峰类型
		int32 ret;				// 1: 上榜, 2:未上榜 
		int32 top_value;		// 巅峰数据
		int32 value;			// 玩家自己的数据
	};
}

#pragma pack(pop)
#endif