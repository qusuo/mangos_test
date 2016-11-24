#ifndef JXS_CSTESTPROT_H_
#define JXS_CSTESTPROT_H_
//测试用协议协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//协议长度测试
	struct CSTestProtDataLen
	{
		CSTestProtDataLen()
			:header(TEST_PROT_DATA_LEN_CS)
			, test_id(0)
		{

		}
		prot::ProtHeader header;
		int32 test_id;
	};
}
#pragma pack(pop)

#endif
