#ifndef JXS_PROTOCOLDEF_H_
#define JXS_PROTOCOLDEF_H_
//author:hsj
//协议
#include "../include/basic_type_def.h"
#include "../module/server_node_def.h"

#pragma pack(push) 
#pragma pack(4)

enum K_RESULT
{
	K_PROCESS_WAIT = 1,
	K_SUC = 0,
	K_EXCEPTION = -1,
	K_MEDIC_CD = -2,
	K_CANT_USE = -3,
	K_CANT_DISCARD = -4,
	K_KNAPSACK_FULL = -5,
};

namespace prot
{
	//S:server
	//G:gatewayserver
	//L:loginserver
	//W:gamelogicserver
	//A:globalserver
	//C:client
	//B:backend
	//M:gmserver
	enum 
	{
		RESERVED_MSG_BEGIN = 1,						//保留协议号
		CLIENT_LOGIN_MSG_BEGIN = 1000,				//客户端与登录服协议
		CLIENT_GAMELOGIC_MSG_BEGIN = 10000,			//客户端与逻辑服协议
		CLIENT_GLOBAL_MSG_BEGIN = 50000,			//客户端与全局服协议
		SERVER_SERVER_MSG_BEGIN = 100000,			//直接连接的服务器之间的协议
		LOGIN_GLOBAL_MSG_BEGIN = 110000,			//登录服与全局服协议
		GAMELOGIC_GLOBAL_MSG_BEGIN = 120000,		//逻辑服与全局服协议
		WORLD_GLOBAL_MSG_BEGIN = 160000,			//世界服与全局服协议
		BACKEND_GM_MSG_BEGIN = 180000,				//后台与gm服协议
		GM_SERVER_LOGIC_MSG_BEGIN = 190000,			//gm服与其他服的逻辑协议
	};

	enum SSMsgType
	{
		SS_MSG_TYPE_INVALID,
		SS_MSG_TYPE_SYS,
		SS_MSG_TYPE_USER,

		SS_MSG_TYPE_END,
	};

	struct ProtHeader
	{
		ProtHeader() :cmd(0), node_index(0){}
		ProtHeader(Int32 command) :cmd(command), node_index(0){}
		Int32 cmd;
		Int32 node_index; //c-s时是node_index, s-c时是net_id
	};

	struct SSHeader
	{
		SSHeader() :cmd(0), tar_node_index(0), tar_node_type(0), msg_type(0){}
		SSHeader(Int32 command, Int32 node_type, Int32 msg_type) : cmd(command), tar_node_index(0), tar_node_type(node_type), msg_type(msg_type){}
		Int32 cmd;
		Int32 tar_node_index;
		Int32 tar_node_type; 
		Int32 msg_type;
	};

	struct SSUHeader
	{
		SSUHeader() :role_id(0){}
		SSUHeader(Int32 command, Int32 tar_node_type) :ss_header(command, tar_node_type, prot::SS_MSG_TYPE_USER), role_id(0){}
		SSHeader ss_header;
		Int64 role_id;
	};
}

#pragma pack(pop)

#endif