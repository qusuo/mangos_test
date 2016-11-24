#ifndef JXS_CSBOOMPROT_H_
#define JXS_CSBOOMPROT_H_
//繁荣度相关协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//繁荣状态
	enum BoomStatus{
		BOOM_STATUS_INVALID,
		BOOM_STATUS_NOR,	//正常
		BOOM_STATUS_RUIN,	//废墟

		BOOM_STATUS_END,
		BOOM_STATUS_COUNT = BOOM_STATUS_END - 1,
	};


	//繁荣度恢复，花费金币
	struct CSBoomRestore
	{
		CSBoomRestore()
			:header(ROLE_ATTR_RESTORE_BOOM_CS)
		{

		}
		prot::ProtHeader header;
	};

}
#pragma pack(pop)

#endif
