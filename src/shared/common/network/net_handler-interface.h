#ifndef JXS_NETHANDLERINTERFACE_H_
#define JXS_NETHANDLERINTERFACE_H_
//author:hsj

#include "../../common/include/basic_type_def.h"

namespace jxsnet
{
class NetHandler
{
public:
	NetHandler(){}
	virtual ~NetHandler(){}

	virtual void OnAccept(Int32 netid, const char* remote_ip, Int32 remote_port)=0;
	virtual void OnRecv(Int32 netid, const char *data, Int32 length)=0;
	virtual void OnDisconnect(Int32 netid)=0;
	virtual void OnConnect(Int32 res_code, Int32 netid, const char* remote_ip, Int32 retmote_port)=0;
};
}

#endif