#ifndef JXS_CSCHATPROT_H_
#define JXS_CSCHATPROT_H_
//聊天相关协议

#include "../cs_global_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../include/value_def.h"
#include "../../game/structs/chat_room_data.h"
#include "../wa_protocol_def.h"
#include "../cs_protocol_def.h"


#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//聊天室类型，所有聊天记录保存3天
	enum ChatRoomType
	{
		CHAT_ROOM_TYPE_INVALID,
		
		CHAT_ROOM_TYPE_CUSTOMER_SERVICE,	//客服聊天室，一直存在，在聊天后添加一个
		CHAT_ROOM_TYPE_WORLD,				//世界聊天室一直存在
		CHAT_ROOM_TYPE_ALLIANCE,			//军团聊天室，每个军团一个
		CHAT_ROOM_TYPE_CAMPS,				//阵营
		CHAT_ROOM_TYPE_PRIVATE,				//私聊，一方下线时清理

		CHAT_ROOM_TYPE_END,
		CHAT_ROOM_TYPE_COUNT = CHAT_ROOM_TYPE_END - 1,
	};

	//广播级别
	enum BroadcastLevel
	{
		BROADCAST_LEVEL_INVALID,

		BROADCAST_LEVEL_WORLD,			//只在世界频道中出现
		BROADCAST_LEVEL_TRUMPET_WORLD,	//发送喇叭，并在世界频道中出现
		BROADCAST_LEVEL_TRUMPET_ALL,	//发送喇叭，在所有频道中出现

		BROADCAST_LEVEL_END,
		BROADCAST_LEVEL_COUNT = BROADCAST_LEVEL_END - 1,
	};


	//说话，在频道内说话
	struct CSChatRoomSpeak
	{
		CSChatRoomSpeak()
			:header(CHAT_SPEAK_CS)
			, chat_type(0)
			, role_id(0)
			, chat_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 chat_type;				//所在房间类型,enum ChatRoomType
		Int64 chat_id;					//聊天室id,世界、客服频道只有一个，默认为1；军团默认为军团id,
										//私聊用户创建，服务器返回
		//role base of speaker 
		RoleID role_id;					//进一步的详细消息可以通过role_id获取
		ChatMsgData msg_data;			//消息内容
	};

	//在频道内说话返回
	struct CSChatRoomSpeakRet
	{
		CSChatRoomSpeakRet()
			:header(CHAT_SPEAK_RET_SC)
			, chat_type(0)
			, res_code(K_EXCEPTION)
			, chat_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 chat_type;				//所在房间类型,enum ChatRoomType
		Int64 chat_id;					//聊天室id,世界、客服频道只有一个，默认为1；军团默认为军团id,
		Int32 res_code;					//在频道说话是否成功：0 = 成功， -1=失败
		
		ChatMsgData msg_data;			//消息内容
	};

	//向聊天室发送某用户的消息,一次一条消息
	struct SCChatRoomSendMsg
	{
		SCChatRoomSendMsg()
			:header(csprot::CHAT_SEND_MSG_SC)
			, chat_type(0)
			, chat_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 chat_type;						//所在房间类型
		Int64 chat_id;							//聊天室id

		ChatData chat_data;						//消息内容及相关信息
	};


	//点击说话用户时进一步获取用户信息
	struct CSChatReqRoleInfo
	{
		CSChatReqRoleInfo()
			:header(csprot::CHAT_REQ_ROLE_INFO_CS)
			, role_id(0)
		{
		}
		prot::ProtHeader header;
		RoleID role_id;
	};

	//获取聊天时用户信息返回
	struct SCChatReqRoleInfoRet
	{
		SCChatReqRoleInfoRet()
			:header(csprot::CHAT_REQ_ROLE_INFO_RET_SC)
		{
		}
		prot::ProtHeader header;
		ChatRoleInfo chat_role_info;
	};

	//请求历史记录
	struct CSChatReqCacheMsg
	{
		CSChatReqCacheMsg()
			:header(csprot::CHAT_REQ_CACHE_MSG_CS)
			, chat_type(0)
			, chat_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 chat_type;				//所在房间类型,enum ChatRoomType
		Int64 chat_id;					//聊天室id
	};
	
	//向聊天室发送某房间的历史消息，可以是多条消息
	struct SCChatReqCacheRet
	{
		SCChatReqCacheRet()
			:header(csprot::CHAT_REQ_CACHE_MSG_RET_SC)
			, chat_type(0)
			, msg_num(0)
			, chat_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 chat_type;						//所在房间类型
		Int32 msg_num;							//聊天内容个数
		Int64 chat_id;							//聊天室id

		ChatData chat_data[MAX_CACHE_MSG_NUM];	//消息内容及相关信息
	};
	
	//广播, 从client 到logic server
	struct CSReqBroadcast
	{
		CSReqBroadcast()
			:header(CHAT_REQ_BROADCAST_CS)
			, broadcast_level(0)
			//, role_id(0)
			, item_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 broadcast_level;			//广播级别,enum BroadcastLevel
		//role base of speaker 
		//RoleID role_id;				//进一步的详细消息可以通过role_id获取
		Int32 item_id;					//消耗喇叭道具id,因喇叭有多种

		ChatMsgData msg_data;			//消息内容
	};

	//广播返回,global 2 client
	struct SCReqBroadcastRet
	{
		SCReqBroadcastRet()
			:header(CHAT_REQ_BROADCAST_RET_SC)
			, broadcast_level(0)
			, res_code(K_EXCEPTION)
		{
		}
		prot::ProtHeader header;
		Int32 broadcast_level;			//广播类型,enum BroadcastLevel
		Int32 res_code;					//扣除道具是否成功：0 = 成功， -1=失败

		ChatData chat_data;				//消息内容
	};

	//创建私聊频道
	struct CSCreatePrivateChat
	{
		CSCreatePrivateChat()
			:header(CHAT_REQ_CREATE_PRI_CHAT_CS)
		{
			memset(role_id, 0, sizeof(role_id));
		}

		prot::ProtHeader header;
		RoleID role_id[2];
	};

	//创建私聊频道返回
	struct SCCreatePrivateChatRet
	{
		SCCreatePrivateChatRet()
			:header(CHAT_REQ_CREATE_PRI_CHAT_RET_SC)
			, chat_room_id(0)
			, res_code(K_EXCEPTION)
		{
			memset(role_id, 0, sizeof(role_id));
		}

		prot::ProtHeader header;
		Int64 chat_room_id;			//返回私人聊天室id
		Int32 res_code;				//0 = 成功， -1=失败
		RoleID role_id[2];
	};

	//私聊房间已清除 g 2 c
	struct SCDestroyPrivateChat
	{
		SCDestroyPrivateChat()
			:header(CHAT_DESTROY_PRI_CHAT_SC)
			, chat_room_id(0)
		{
		}

		prot::ProtHeader header;
		Int64 chat_room_id;			//返回私人聊天室id
	};

	//私聊信息已读通知
	struct CSReadedPrivateChat
	{
		CSReadedPrivateChat()
			:header(CHAT_READED_PRIVATE_CHAT_CS)
			, chat_id(0)
		{
		}
		prot::ProtHeader header;
		Int64 chat_id;					//私人聊天室id
	};
	
	//请求私聊频道列表
	struct CSReqPriChatsList
	{
		CSReqPriChatsList()
			:header(csprot::CHAT_REQ_PRIVATE_ROOM_CS)
		{
		}
		prot::ProtHeader header;
	};

	//请求私聊频道列表返回
	struct CCSReqPriChatsListRet
	{
		CCSReqPriChatsListRet()
			:header(csprot::CHAT_REQ_PRIVATE_ROOM_RET_SC)
			, chat_num(0)
		{
		}
		prot::ProtHeader header;
		Int32 chat_num;				//私聊个数
		ChatRoomInfo role_info[MAX_PRIVATE_CHAT_NUM];
	};
}
#pragma pack(pop)

#endif
