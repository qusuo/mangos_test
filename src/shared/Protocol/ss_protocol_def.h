#ifndef JXS_SSPROTOCOLDEF_H_
#define JXS_SSPROTOCOLDEF_H_
//服务器与客户端协议定义

#include "protocol_def.h"
#include "../include/value_def.h"

#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

namespace ssprot
{
	//enum
	//{
	//	REGISTER_SS = prot::SERVER_SERVER_MSG_BEGIN,		//一个服务器向另一个服务器注册
	//	REGISTER_RET_SS,
	//	REQ_DISCONNECT_USER_SG,						//其他服向网关发起断开玩家连接请求
	//	TELL_DISCONNECT_USER_GS,					//网关通知其他服玩家连接已断开
	//	REQ_SET_TIME_GAP_SM,						//其他服向GM服发起改变服务器时间请求
	//	TELL_SET_TIME_GAP_MS,						//GM通知其他服改变服务器时间
	//	REQ_SET_ANTI_ADDICTION_SM,					//其他服向GM服发起改变防沉迷设置请求
	//	TELL_SET_ANTI_ADDICTION_SM,					//GM服通知其他服改变防沉迷设置
	//	//NOTICE_USER_LOGIN_LG,						//登录服通知网关用户将登录（或许应该带令牌)
	//};


	//struct SSRegister
	//{
	//	SSRegister()
	//		:header(REGISTER_SS, 0, SS_MSG_TYPE_SYS)
	//		, node_type(0)
	//		, node_index(0)
	//	{
	//		ip[0] = '\0';
	//	}
	//	prot::SSHeader header;
	//	int32 node_type;
	//	int32 node_index;
	//	char ip[MAX_32_CHAR_LEN];
	//};

	//struct SSRegisterRet
	//{
	//	SSRegisterRet()
	//		:header(REGISTER_RET_SS, 0, SS_MSG_TYPE_SYS)
	//		, node_type(0)
	//		, node_index(0)
	//		, port(0)
	//		, res(-1)
	//	{
	//		ip[0] = '\0';
	//	}
	//	prot::SSHeader header;
	//	int32 node_type;
	//	int32 node_index;
	//	char ip[MAX_32_CHAR_LEN];
	//	int32 port;
	//	int32 res; //0 = 成功, -1=失败
	//};

	////其他服向网关发起断开玩家连接请求
	//struct SGDisconnectUser
	//{
	//	SGDisconnectUser()
	//		:header(REQ_DISCONNECT_USER_SG, 0, SS_MSG_TYPE_SYS)
	//		, role_id(0)
	//	{

	//	}
	//	prot::SSHeader header;
	//	RoleID role_id;
	//};

	////网关通知其他服玩家连接已断开
	//struct GSTellDisconnectUser
	//{
	//	GSTellDisconnectUser()
	//		:header(TELL_DISCONNECT_USER_GS, 0, SS_MSG_TYPE_SYS)
	//		, role_id(0)
	//		, net_id(0)
	//	{

	//	}
	//	prot::SSHeader header;
	//	RoleID role_id;
	//	int32 net_id;
	//};

	////其他服向Gm服发起改变服务器时间请求
	//struct SMReqSetTimeGap
	//{
	//	SMReqSetTimeGap()
	//		:header(REQ_SET_TIME_GAP_SM, 0, SS_MSG_TYPE_SYS)
	//		, time_gap(0)
	//	{

	//	}
	//	prot::SSHeader header;
	//	int32 time_gap;

	//};

	////Gm服通知其他服改变服务器时间
	//struct MSTellSetTimeGap
	//{
	//	MSTellSetTimeGap()
	//		:header(TELL_SET_TIME_GAP_MS, 0, SS_MSG_TYPE_SYS)
	//		, time_gap(0)
	//	{

	//	}
	//	prot::SSHeader header;
	//	int32 time_gap;

	//};

	////其他服向GM服发起改变防沉迷设置请求
	//struct SMReqSetAntiAddiction
	//{
	//	SMReqSetAntiAddiction()
	//		:header(REQ_SET_ANTI_ADDICTION_SM, 0, SS_MSG_TYPE_SYS)
	//		, param1(0)
	//		, param2(0)
	//	{

	//	}
	//	prot::SSHeader header;
	//	int32 param1;
	//	int32 param2;
	//};

	////GM服通知其他服改变防沉迷设置
	//struct MSTellSetAntiAddiction
	//{
	//	MSTellSetAntiAddiction()
	//		:header(TELL_SET_ANTI_ADDICTION_SM, 0, SS_MSG_TYPE_SYS)
	//		, param1(0)
	//		, param2(0)
	//	{

	//	}
	//	prot::SSHeader header;
	//	int32 param1;
	//	int32 param2;

	//};

}

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

//#pragma pack(pop)

#endif
