#ifndef JXS_CSROLEATTRADDPROTOCOL_H_
#define JXS_CSROLEATTRADDPROTOCOL_H_
// 加成属性更改通知

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../../common/game/include/battle_def.h"
#include "../../../common/game/structs/attr_data.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	// 加成属性改变时服务器通知
	struct SCRoleAttrAddChange
	{
		SCRoleAttrAddChange()
			:header(ROLE_ATTR_ADD_CHANGE_NOTICE_SC)
			, attr_add_type(0)
			, attr_add_value(0)
		{
		}

		prot::ProtHeader header;
		int32 attr_add_type;        // enum ProtRoleAttrAdd
		int32 attr_add_value;	// 根据attr_add_type不同而改变
	};

	
}
#pragma pack(pop)

#endif
