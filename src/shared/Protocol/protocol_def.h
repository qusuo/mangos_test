#ifndef JXS_PROTOCOLDEF_H_
#define JXS_PROTOCOLDEF_H_
//author:hunter
//协议
#include "Platform/Define.h"


enum ServerNodeType
{
	SERVER_NODE_TYPE_INVALID,
	SERVER_NODE_TYPE_DATA,			//数据服
	SERVER_NODE_TYPE_LOGIN,			//登录服
	SERVER_NODE_TYPE_GAMELOGIC,		//逻辑服
	SERVER_NODE_TYPE_GATEWAY,		//网关服
	SERVER_NODE_TYPE_GLOBAL,		//全局服
	SERVER_NODE_TYPE_CLIENT,		//客户端
	SERVER_NODE_TYPE_GM,			//gm服

	SERVER_NODE_TYPE_COUNT,

};

#define MAX_NODE_SERVER 32

//S:server
//D:dbserver
//G:gatewayserver
//L:loginserver
//W:gamelogicserver
//A:globalserver
//C:client
//B:backend
//M:gmserver

enum MsgTarget
{
	MSG_TARGET_INVALID,
	MSG_TARGET_ALL_SERVERS,		//all server
	MSG_TARGET_ALL_USERS,		//all user
	MSG_TARGET_G,				//G:gatewayserver
	MSG_TARGET_L,				//L:loginserver
	MSG_TARGET_W,				//W:gamelogicserver
	MSG_TARGET_A,				//A:globalserver
	MSG_TARGET_C,				//C:client
	MSG_TARGET_B,				//B:backend
	MSG_TARGET_M,				//M:gmserver

	MSG_TARGET_END,
};


enum
{
	RESERVED_MSG_BEGIN = 1,					//保留协议号
	SERVER_SERVER_MSG_BEGIN = 1000,			//直接连接的服务器之间的协议,gateway 2 logic

	CLIENT_LOGIN_MSG_BEGIN = 30000,			//客户端与登录服协议
	CLIENT_GAMELOGIC_MSG_BEGIN = 60000,		//客户端与逻辑服协议
	CLIENT_GLOBAL_MSG_BEGIN = 90000,		//客户端与全局服协议

	LOGIN_GLOBAL_MSG_BEGIN = 120000,		//登录服与全局服协议
	GAMELOGIC_GLOBAL_MSG_BEGIN = 150000,	//逻辑服与全局服协议
	WORLD_GLOBAL_MSG_BEGIN = 180000,		//世界服与全局服协议
	BACKEND_GM_MSG_BEGIN = 210000,			//后台与gm服协议
	GM_SERVER_LOGIC_MSG_BEGIN = 240000,		//gm服与其他服的逻辑协议
};

enum SSMsgType
{
	SS_MSG_TYPE_INVALID,
	SS_MSG_TYPE_SYS,		//
	SS_MSG_TYPE_USER,

	SS_MSG_TYPE_END,
};


#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

//#pragma pack(push) 
//#pragma pack(4)


namespace prot
{
	struct ProtHeader
	{
		ProtHeader() :cmd(0), node_index(0), size(0){}
		ProtHeader(int32 command) :cmd(command), node_index(0), size(0){}
		uint16 size;
		uint32 cmd;
		int32 node_index; //c-s时是node_index, s-c时是net_id
	};

	struct SSHeader
	{
		SSHeader() :cmd(0), tar_node_index(0), tar_node_type(0), cmd_type(0), size(0){}
		SSHeader(int32 command, int32 node_type, int32 cmd_type) : cmd(command), tar_node_index(0), tar_node_type(node_type), cmd_type(cmd_type), size(0){}
		uint16 size;
		uint16 cmd;
		int32 cmd_type;
		int32 tar_node_index; //c-s时是node_index, s-c时是net_id
		int32 tar_node_type; 
	};

	struct SSUHeader
	{
		SSUHeader() :role_id(0), size(0){}
		SSUHeader(int32 command, int32 tar_node_type) :ss_header(command, tar_node_type, 1), role_id(0), size(0){}
		uint16 size;
		SSHeader ss_header;
		int64 role_id;
	};
}

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif
//#pragma pack(pop)

#endif