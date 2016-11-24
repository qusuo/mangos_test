#ifndef JXS_NETINTERFACE_H_
#define JXS_NETINTERFACE_H_
//author:hsj
#include "../../common/module/module-interface.h"
#include "net_param.h"

namespace jxsnet
{
class NetHandler;
//网络定义
class Net : public jxsmodule::Module
{
public:
	Net(){};
	virtual ~Net(){};

	//from module
	virtual Int32 Init()=0;
	virtual Int32 Start()=0;
	virtual Int32 Update()=0;
	virtual Int32 UnInit()=0;

	virtual void SetNetParam(NetParam& net_param) = 0;
	virtual void RegisterHandler(NetHandler * net_handler)=0;
	//out_netid可以为NULL
	virtual Int32 Listen(const char* ip, Int32 port, Int32 backlog, Int32* out_netid)=0;
	virtual Int32 Connect(const char* ip, Int32 port, Int32* out_netid,Int64 time_out)=0;
	//virtual bool ConnectAsyn(const char *ip, uInt16 port, _out uInt32 *check_handle, uInt64 time_out=3000) = 0;
	virtual Int32 Send(Int32 netid, const char* data, Int32 data_len)=0;
	virtual Int32 SendPacket(Int32 netid, const char* data, Int32 data_len) = 0;
	virtual void Disconnect(Int32 netid)=0;
};
}

#endif