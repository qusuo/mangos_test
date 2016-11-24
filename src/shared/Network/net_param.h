#ifndef JXS_NETPARAM_H_
#define JXS_NETPARAM_H_

#include "Platform/Define.h"
#include "include/value_def.h"

namespace jxsnet
{
	struct NetParam
	{
		NetParam()
			:max_package_len(0)
			, max_package_buff_len(0)
			, init_con_socket_num(0)
			, max_con_socket_num(20000)
		{
			memset(net_name, 0, sizeof(net_name));
		}
		char net_name[MAX_32_CHAR_LEN];
		int32 max_package_len;
		int32 max_package_buff_len;
		int32 init_con_socket_num;
		int32 max_con_socket_num;    //最大socket数量
	}; 
}

#endif