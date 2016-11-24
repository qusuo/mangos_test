#ifndef JXS_WACOMMONPROT_H_
#define JXS_WACOMMONPROT_H_
//gamelogicserver与globalserver之间的角色基础相关协议定义

#include "../wa_protocol_def.h"
#include "../../include/value_def.h"
#include "../../game/metadata/user_meta.h"

#pragma pack(push) 
#pragma pack(4)

namespace waprot
{
	//角色登录
	struct WARoleLogin
	{
		WARoleLogin()
			:header(USER_LOGIN_INFO_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
		{
		}
		prot::SSHeader header;
		GlobalUserInfo user_info;
	};

	//角色登出
	struct WARoleLogout
	{
		WARoleLogout()
			:header(USER_LOGOUT_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, role_id(0)
		{
		}

		prot::SSHeader header;
		RoleID role_id;
	};

	//USER_INFO_CHANGE_WA,		//角色信息变更
	struct WARoleAttrChange
	{
		WARoleAttrChange()
			:header(USER_INFO_CHANGE_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, role_id(0)
			, attr_type(0)
			, attr_value(0)
		{
			memset(buf, 0, sizeof(buf));
		}
		prot::SSHeader header;
		RoleID role_id;
		int32 attr_type;        // enum ProtRoleAttr
		int32 attr_value;		// 改变之后用户的属性值
		char buf[MAX_32_CHAR_LEN];
	};

	//同步角色信息
	struct WARoleSyncInfo
	{
		WARoleSyncInfo()
			:header(USER_SYNC_INFO_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, role_id(0)
			, silent_end_time(0)
		{
		}
		prot::SSHeader header;
		RoleID role_id;
		Int64 silent_end_time;  //禁言结束时间
	};

	//USER_LOGOUT_WA,				//角色登出
	//USER_GET_ITEMS_AW,			//获得物品
	//USER_GET_ITEMS_RET_WA,		//获得物品返回

	//struct WANewMailNotice
	//{
	//	WANewMailNotice()
	//		:header(MAIL_NEW_MAIL_NOTICE_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
	//		, role_id(0)
	//		, mail_id(0)
	//		, is_spy_report(0)
	//	{

	//	}
	//	prot::SSHeader header;
	//	RoleID role_id;
	//	Int64 mail_id;
	//	int32 is_spy_report;	//是否侦查报告
	//};
}
#pragma pack(pop)

#endif
