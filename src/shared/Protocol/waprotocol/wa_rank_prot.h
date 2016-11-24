#ifndef JXS_WARANKPROT_H_
#define JXS_WARANKPROT_H_
//gamelogicserver与globalserver之间的排行相关协议定义

#include "../wa_protocol_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace waprot{

	// 排行榜中角色数据变化
	struct WARankRoleAttrChange{
		WARankRoleAttrChange()
			:header(waprot::RANK_ROLE_ATTR_CHANGE_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, role_id(0)
			, type(-1)
			, value(0)
			, time(0)
		{}
		prot::SSHeader header;
		RoleID role_id;
		int32 type;		// 排行榜类型
		int32 value;	// 属性值
		Int64 time;		// 发生时间
	};

	struct WARankInfo{
		WARankInfo()
			:header(waprot::RANK_INFO_WA, SERVER_NODE_TYPE_GLOBAL, SS_MSG_TYPE_SYS)
			, type(0)
		{}
		prot::SSHeader header;
		int32 type;			// 排行榜类型
		int32 num;			// 显示数量: -1:全部显示
	};
}

#pragma pack(pop)
#endif