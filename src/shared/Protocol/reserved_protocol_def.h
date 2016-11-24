#ifndef JXS_RESERVEDPROTOCOLDEF_H_
#define JXS_RESERVEDPROTOCOLDEF_H_
//服务器与客户端协议定义

#include "protocol_def.h"
#include "../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace resprot
{
	enum
	{
		RMI_CALL_RES = prot::RESERVED_MSG_BEGIN,
		RMI_CALLBACK_RES,
	};

	struct ResRmiCall
	{
		ResRmiCall()
			:header(RMI_CALL_RES)
			, rmi_cmd(0)
			, call_obj_id(0)
			, data_len(0)
		{
			data_begin[0] = 0;
		}
		prot::ProtHeader header;
		int32 rmi_cmd;
		int32 call_obj_id;
		int32 data_len;
		char data_begin[MAX_1M_BUFFER_LEN];
	};

	struct ResRmiCallBack
	{
		ResRmiCallBack()
			:header(RMI_CALLBACK_RES)
			, call_obj_id(0)
			, data_len(0)
		{
			data_begin[0] = 0;
		}
		prot::ProtHeader header;
		int32 call_obj_id;
		int32 data_len;
		char data_begin[MAX_6M_BUFFER_LEN];
	};
}
#pragma pack(pop)

#endif
