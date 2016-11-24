#ifndef JXS_CSGLOBALPROTOCOLDEF_H_
#define JXS_CSGLOBALPROTOCOLDEF_H_
//服务器与客户端协议定义(global部分)

#include "protocol_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
//client <---> globalserver
enum
{
	//邮件
	MAIL_GET_LIST_CS = prot::CLIENT_GLOBAL_MSG_BEGIN,  //获取邮件列表
	MAIL_SEND_LIST_SC,			//发送邮件列表信息
	MAIL_GET_INFO_CS,			//取邮件详细信息
	MAIL_SEND_INFO_SC,			//发送邮件详细信息
	MAIL_SYNC_INFO_CHANGE_SC,	//邮件信息变更同步
	MAIL_DELETE_MAIL_CS,		//删除邮件
	MAIL_DELETE_MAIL_RET_SC,	//删除邮件返回
	MAIL_POST_MAIL_CS,			//写邮件
	MAIL_POST_MAIL_RET_SC,		//写邮件返回
	MAIL_LOCK_MAIL_CS,			//锁定邮件
	MAIL_LOCK_MAIL_RET_SC,		//锁定邮件返回
	MAIL_FETCH_APPENDIX_CS,		//提取附件
	MAIL_FETCH_APPENDIX_RET_SC,	//提取附件返回

	//排行榜
	RANK_CS = prot::CLIENT_GLOBAL_MSG_BEGIN + 100,				// 排行请求
	RANK_SC,
	RANK_ROLE_ATTR_CHANGE_CS,
	RANK_TOP_SC,

	//聊天
	CHAT_SPEAK_CS = prot::CLIENT_GLOBAL_MSG_BEGIN + 200,//client 发送聊天信息， c 2 g
	CHAT_SPEAK_RET_SC,					//说话返回,提示是否成功
	CHAT_SEND_MSG_SC,					//向聊天室发送某用户的消息 g 2 c
	CHAT_REQ_ROLE_INFO_CS,				//点击说话用户时进一步获取用户信息 c 2 g
	CHAT_REQ_ROLE_INFO_RET_SC,			//获取聊天时用户信息返回 g 2 c
	CHAT_REQ_CACHE_MSG_CS,				//请求聊天历史记录 client 2 global
	CHAT_REQ_CACHE_MSG_RET_SC,			//请求聊天历史记录返回 g 2 c
	//CHAT_REQ_BROADCAST_CS,				//广播，用户调用 c 2 l
	//CHAT_REQ_BROADCAST_WA,				//广播，logic服务器调用 弃用
	CHAT_REQ_BROADCAST_RET_SC,			//广播返回g 2 client 
	CHAT_REQ_CREATE_PRI_CHAT_CS,		//创建私人聊天室 c 2 g
	CHAT_REQ_CREATE_PRI_CHAT_RET_SC,	//创建私人聊天室返回 g 2 c
	CHAT_DESTROY_PRI_CHAT_SC,			//私聊房间已清除 g 2 c

	CHAT_READED_PRIVATE_CHAT_CS,		//私聊信息已读通知 c 2 g
	CHAT_REQ_PRIVATE_ROOM_CS,			//请求私聊频道列表 c 2 g
	CHAT_REQ_PRIVATE_ROOM_RET_SC,		//请求私聊频道列表返回 g 2 c
};
}
#pragma pack(pop)

#endif
