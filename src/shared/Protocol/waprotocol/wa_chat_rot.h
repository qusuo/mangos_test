#ifndef JXS_WACHATPROT_H_
#define JXS_WACHATPROT_H_

//author:hunter(757155633@qq.com)
//gamelogicserver与globalserver之间的聊天系统相关协议定义

#include "../wa_protocol_def.h"
#include "../../include/value_def.h"
#include "../../game/metadata/user_meta.h"
#include "../../game/structs/chat_room_data.h"


#pragma pack(push) 
#pragma pack(4)

namespace waprot
{
	//logic server 转发广播消息
	struct WAReqBroadcast
	{
		WAReqBroadcast()
			:header(CHAT_REQ_BROADCAST_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, broadcast_level(0)
			, role_id(0)
			, res_code(0)
		{
		}
		prot::SSHeader header;
		int32 broadcast_level;			//所在房间类型,enum BroadcastLevel
		ChatMsgData msg_data;			//消息内容

		//role base of speaker 
		RoleID role_id;			//进一步的详细消息可以通过role_id获取
		int32 res_code;			//扣除道具是否成功：0 = 成功， -1=失败
	};

	//logic server 创建军团消息
	struct WAChatAllianceCreate
	{
		WAChatAllianceCreate()
			:header(CHAT_ALLIANCE_CREATE_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, alliance_id(0)
		{
		}
		prot::SSHeader header;
		Int64 alliance_id;		//军团id
	};


	//logic server 注销军团消息
	struct WAChatAllianceDestroy
	{
		WAChatAllianceDestroy()
			:header(CHAT_ALLIANCE_DESTROY_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, alliance_id(0)
		{
		}
		prot::SSHeader header;
		Int64 alliance_id;		//军团id
	};

	//logic server 角色加入军团消息
	struct WAChatRoleInAlliance
	{
		WAChatRoleInAlliance()
			:header(CHAT_ROLE_IN_ALLIANCE_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, role_id(0)
			, alliance_id(0)
		{
		}
		prot::SSHeader header;
		RoleID role_id;			//角色id
		Int64 alliance_id;		//军团id
	};
	
	//logic server 角色退出军团消息
	struct WAChatRoleOutAlliance
	{
		WAChatRoleOutAlliance()
			:header(CHAT_ROLE_OUT_ALLIANCE_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, role_id(0)
			, alliance_id(0)
		{
		}
		prot::SSHeader header;
		RoleID role_id;			//角色id
		Int64 alliance_id;		//军团id
	};

	//logic server 阵营结束消息
	struct WAChatCampsEnd
	{
		WAChatCampsEnd()
			:header(CHAT_CAMPS_END_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
		{
		}
		prot::SSHeader header;
	};

	//logic server 角色加入阵营消息
	struct WAChatRoleInCamps
	{
		WAChatRoleInCamps()
			:header(CHAT_ROLE_IN_CAMPS_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, camps_id(0)
			, role_num(0)
		{
			memset(role_id, 0, sizeof(role_id));
		}
		prot::SSHeader header;
		int32 camps_id;			//阵营id, 1/2
		int32 role_num;			//加入角色个数
		RoleID role_id[MAX_ROLE_FOR_MSG];			//角色id
	};

}
#pragma pack(pop)

#endif
