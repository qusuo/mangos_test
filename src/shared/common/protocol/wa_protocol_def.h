#ifndef JXS_WAPROTOCOLDEF_H_
#define JXS_WAPROTOCOLDEF_H_
//gamelogicserver与globalserver之间的协议定义

#include "protocol_def.h"
#include "../game/include/server_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace waprot
{

enum
{
	//基本信息
	USER_LOGIN_INFO_WA = prot::GAMELOGIC_GLOBAL_MSG_BEGIN,  //角色登录
	USER_INFO_CHANGE_WA,		//角色信息变更
	USER_LOGOUT_WA,				//角色登出
	USER_GET_ITEMS_AW,			//获得物品
	USER_GET_ITEMS_RET_WA,		//获得物品返回
	USER_SYNC_INFO_WA,			//同步角色信息

	//邮件
	MAIL_NEW_MAIL_NOTICE_WA = prot::GAMELOGIC_GLOBAL_MSG_BEGIN + 500,  //有新邮件通知
	MAIL_FETCH_APPENDEX_AW,						//提取附件
	MAIL_FETCH_APPENDEX_RET_WA,					//提取附件返回

	//排行榜
	RANK_ROLE_ATTR_CHANGE_WA = prot::GAMELOGIC_GLOBAL_MSG_BEGIN + 600, // 玩家属性变化
	RANK_INFO_WA,

	//军团
	ALLIANCE_ROLE_ATTR_CHANGE_WA = prot::GAMELOGIC_GLOBAL_MSG_BEGIN + 650,	// 玩家军团属性变化

	//聊天系统
	CHAT_REQ_BROADCAST_WA = prot::GAMELOGIC_GLOBAL_MSG_BEGIN + 700,		//广播，logic服务器调用
	CHAT_ALLIANCE_CREATE_WA,		//军团创建
	CHAT_ALLIANCE_DESTROY_WA,		//军团注销
	CHAT_ROLE_IN_ALLIANCE_WA,		//角色加入军团
	CHAT_ROLE_OUT_ALLIANCE_WA,		//角色退出军团

	CHAT_CAMPS_END_WA,			//阵营活动结束
	CHAT_ROLE_IN_CAMPS_WA,		//角色加入阵营
	
	


};
}
#pragma pack(pop)

#endif
