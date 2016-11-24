#ifndef JXS_TASKPROT_H_
#define JXS_TASKPROT_H_
//任务协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
struct TaskInfo
{
	TaskInfo()
		:task_id(0)
		, task_state(0)
		, reserved(0)
		, data_param(0)
	{

	}
	int32 task_id;
	Int16 task_state;
	Int16 reserved;
	int32 data_param; //当前进度
};

//请求任务列表
struct CSTaskReqTaskList
{
	CSTaskReqTaskList()
		:header(TASK_REQ_TASK_LIST_INFO_CS)
	{

	}
	prot::ProtHeader header;
};

//发送任务列表
struct SCTaskSendTaskList
{
	SCTaskSendTaskList()
		:header(TASK_SEND_TASK_LIST_INFO_SC)
		, task_num(0)
	{

	}
	prot::ProtHeader header;
	int32 task_num;
	TaskInfo task_infos[MAX_ROLE_TASK_NUM];
};

//接受任务(日常任务）
struct CSTaskAccept
{
	CSTaskAccept()
		:header(TASK_ACCEPT_CS)
		, task_id(0)
	{

	}
	prot::ProtHeader header;
	int32 task_id;
};

//放弃任务(日常任务）
struct CSTaskGiveup
{
	CSTaskGiveup()
		:header(TASK_GIVEUP_CS)
		, task_id(0)
	{

	}
	prot::ProtHeader header;
	int32 task_id;
};

//提交任务
struct CSTaskCommit
{
	CSTaskCommit()
		:header(TASK_COMMIT_CS)
		, task_id(0)
	{

	}
	prot::ProtHeader header;
	int32 task_id;
};

//任务信息变更通知
struct SCTaskSendInfoChange
{
	enum ChangeType
	{
		CHANGE_TYPE_INVALID,
		CHANGE_TYPE_ADD,
		CHANGE_TYPE_UPDATE,
		CHANGE_TYPE_DELETE,
	};
	SCTaskSendInfoChange()
		:header(TASK_SEND_INFO_CHANGE_SC)
		, change_type(0)
	{

	}
	prot::ProtHeader header;
	int32 change_type;
	TaskInfo task_info;
};

//重置日常任务
struct CSTaskResetDailyTask
{
	CSTaskResetDailyTask()
		:header(TASK_RESET_DAILY_TASK_CS)
	{

	}
	prot::ProtHeader header;
};

//领取活跃度奖励
struct CSTaskFetchActiveReward
{
	CSTaskFetchActiveReward()
		:header(TASK_FETCH_ACTIVE_REWARD_CS)
		, task_id(0)
	{

	}
	prot::ProtHeader header;
	int32 task_id;
};

//领取活跃度奖励返回
struct SCTaskFetchActiveRewardRet
{
	SCTaskFetchActiveRewardRet()
		:header(TASK_FETCH_ACTIVE_REWARD_RET_SC)
		, task_id(0)
		, res_code(0)
	{

	}
	prot::ProtHeader header;
	int32 task_id;
	int32 res_code; //1 = 成功， 0 = 失败
};

//领取成就奖励
struct CSTaskFetchAchieveReward
{
	CSTaskFetchAchieveReward()
		:header(TASK_FETCH_ACHIEVE_REWARD_CS)
		, reward_pos(0)
	{

	}
	prot::ProtHeader header;
	int32 reward_pos;
};

//领取成就奖励返回
struct SCTaskFetchArchieveRewardRet
{
	SCTaskFetchArchieveRewardRet()
		:header(TASK_FETCH_ACHIEVE_REWARD_RET_SC)
		, reward_pos(0)
		, res_code(0)
	{

	}
	prot::ProtHeader header;
	int32 reward_pos;
	int32 res_code; //1 = 成功， 0 = 失败
};

}
#pragma pack(pop)

#endif
