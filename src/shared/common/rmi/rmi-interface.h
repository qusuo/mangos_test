#ifndef JXS_RMIINTERFACE_H_
#define JXS_RMIINTERFACE_H_
//author:hsj
#include "../network/net-interface.h"

class RmiCallBackObj
{
public:
	RmiCallBackObj();
	virtual ~RmiCallBackObj();
	virtual bool CallBack(const char* data, Int32 data_len) = 0;
	bool IsExpired();

private:
	Int64 m_create_time;
};

class RmiClient
{
public:
	RmiClient(){};
	virtual ~RmiClient(){};

	virtual void SetRmiNet(jxsnet::Net* rmi_net) = 0;
	virtual bool Call(Int32 net_id, char* data, Int32 data_len, RmiCallBackObj* back_obj, Int32 *err) = 0;
	virtual bool CallBack(const char* data, Int32 data_len) = 0;
};

#endif