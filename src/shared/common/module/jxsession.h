#ifndef JXS_JXSESSION_H_
#define JXS_JXSESSION_H_
//author:hsj

#include "../include/basic_type_def.h"
#include "../include/value_def.h"

struct JXSession
{
	JXSession()
		:server_node_type(0)
		, id(0)
		, net_id(0)
		, port(0)
		, last_active_time(0)
	{
		ip[0] = '\0';
	}
	Int32 server_node_type;
	Int64 id; //如果是服务器，就是node_index, 如果是玩家，就是role_id
	Int32 net_id;
	char ip[MAX_32_CHAR_LEN];
	Int32 port;
	Int32 last_active_time;  //最后活跃时间，用来判断超一定时间不连接，则清空session

	void* operator new(size_t s);
	void operator delete(void* p);

};


#endif