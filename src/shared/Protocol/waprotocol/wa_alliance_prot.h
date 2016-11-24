#ifndef JXS_WAALLIANCERROT_H_
#define JXS_WAALLIANCEPROT_H_
//gamelogicserver与globalserver之间的邮件相关协议定义

#include "../wa_protocol_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace waprot{

	// 玩家军团属性变化
	struct WARoleAllianceAttrChange{
		WARoleAllianceAttrChange()
			:header(waprot::ALLIANCE_ROLE_ATTR_CHANGE_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
		{}
		prot::SSHeader header;
		RoleID role_id;
		Int64 alliance_id;
		char alliance_name[MAX_32_CHAR_LEN];
	};
}

#pragma pack(pop)
#endif