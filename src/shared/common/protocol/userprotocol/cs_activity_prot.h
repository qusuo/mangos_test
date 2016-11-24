#ifndef JXS_ACTIVITYPROT_H_
#define JXS_ACTIVITYPROT_H_
//活动协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
struct ActInfo
{
	static const Int32 ACT_DATA_LEN = 512;
	ActInfo()
		:act_id(0)
	{
		memset(act_data, 0, sizeof(act_data));
	}
	Int32 act_id;
	char act_data[ACT_DATA_LEN];
};

//请求活动列表
struct CSActReqActList
{
	CSActReqActList()
		:header(ACT_REQ_LIST_INFO_CS)
	{

	}
	prot::ProtHeader header;
};



//发送活动列表
struct SCActSendActList
{
	static const Int32 ACTS_DATA_LEN = 1024 * 10;
	SCActSendActList()
		:header(ACT_SEND_LIST_INFO_SC)
		, act_num(0)
	{
		memset(acts_data, 0, sizeof(acts_data));
	}
	prot::ProtHeader header;
	Int32 act_num;
	char acts_data[ACTS_DATA_LEN];
};

//活动信息变更通知
struct SCActSendInfoChange
{
	SCActSendInfoChange()
		:header(ACT_SEND_INFO_CHANGE_SC)
	{

	}
	prot::ProtHeader header;
	ActInfo act_info;
};

//领取礼包类活动奖励
struct CSActFetchGiftActReward
{
	CSActFetchGiftActReward()
		:header(ACT_FETCH_GIFT_ACT_REWARD_CS)
		, ac_id(0)
		, reward_pos(0)
	{

	}

	enum {
		REWARD_CAN_FETCH,
		REWARD_FETCHED,
	};

	prot::ProtHeader header;
	Int32 ac_id;
	Int32 reward_pos;
};

}
#pragma pack(pop)

#endif
