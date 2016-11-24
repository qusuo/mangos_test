#ifndef JXS_NETAPI_H_
#define JXS_NETAPI_H_

#include "net_handler-interface.h"
#include "net-interface.h"

namespace jxsnet
{
	Net* CreateNet(NetParam& net_param);
}


#endif