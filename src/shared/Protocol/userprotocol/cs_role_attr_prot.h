#ifndef JXS_CSROLEATTRPROTOCOL_H_
#define JXS_CSROLEATTRPROTOCOL_H_
//修改用户属性信息的协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"
#include "../../game/include/battle_def.h"
#include "../../game/structs/soldier_data.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	// 请求修改角色名
	struct CSRoleAttrChangeName
	{
		CSRoleAttrChangeName()
			:header(ROLE_ATTR_CHANGE_NAME_CS)
		{
			memset(role_name, 0, sizeof(role_name));
		}
		prot::ProtHeader header;
		char role_name[32];
	};

	// 修改角色名后通知
	struct SCRoleAttrChangeName
	{
		SCRoleAttrChangeName()
			:header(ROLE_ATTR_CHANGE_NAME_NOTICE_SC)
		{
			// memset(role_name, 0, 32);
		}
		prot::ProtHeader header;
		// char role_name[32];
	};

	//请求修改头像
	struct CSRoleChangeAvatar
	{
		CSRoleChangeAvatar()
			:header(ROLE_ATTR_CHANGE_AVATAR_CS)
			, avatar_id(0)
		{
		}
		prot::ProtHeader header;
		int32 avatar_id;
		//todo:hunter(757155633@qq.com)
	};

	//请求购买军令
	struct CSRoleAttrBuyEnergy
	{
		CSRoleAttrBuyEnergy()
			:header(ROLE_ATTR_BUY_ENERGY_CS)
		{
			//energy_value = 0;
		}
		prot::ProtHeader header;
	};
	
}
#pragma pack(pop)

#endif
