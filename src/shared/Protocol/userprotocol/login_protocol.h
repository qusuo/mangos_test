#ifndef JXS_LOGINPROTOCOL_H_
#define JXS_LOGINPROTOCOL_H_
//author:hsj
//与数据库交互的协议定义
#include "../../include/basic_type_def.h"
#include "../../include/value_def.h"
#include "../protocol_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace loginprot
{
	enum RmiCmd
	{
		USER_LOGIN_CS = prot::CLIENT_LOGIN_MSG_BEGIN,			//玩家请求登录
		USER_LOGIN_RET_SC,								//玩家请求登录返回	
		USER_CREATE_ROLE_CS,							//玩家请求创建角色
		USER_CREATE_ROLE_RET_SC,						//玩家请求创建角色返回
	};

	struct CSUserLogin
	{
		CSUserLogin()
			:header(USER_LOGIN_CS)
			,plat_id(0)
			,server_id(0)
		{
			account[0] = '\0';
		}
		prot::ProtHeader header;
		int32 plat_id;
		int32 server_id;
		char account[MAX_64_CHAR_LEN];
	};

	enum LoginResult
	{
		LI_HAVE_ROLE			= 0,
		LI_INVALID_VALUE		= -1,//无效名柄
		LI_LOGIN_REPEAT			= -2,//相同角色重复登录
		LI_LOGIN_ONLY_ONE_ROLE	= -3,//同一帐号只能登录一个角色
		LI_HAVE_NOT_ROLE		= -4,//没有角色
		LI_EXIST_SAME_ROLE_NAME = -5,//已有角色名称
		LI_OVER_MAX_ROLE_NUM	= -6,//超过最大角色数目
		LI_ILLEAGAL_OPERATION	= -7,//服务器人数过多
		LI_WALLOW_FORBID		= -8,//防沉迷封号
		LI_PLAT_NAME_FORBID		= -9,//帐号被封
		LI_IP_FORBID			= -10,//ip被封
		LI_SERVER_CLOSING		= -11,//服务器关闭
		LI_ANTI_TIMEOUT			= -12,//防沉迷时间到
	};
	struct SCUserLogin
	{
		SCUserLogin()
			:header(USER_LOGIN_RET_SC)
			,login_result(LI_INVALID_VALUE)
			,gateway_port(0)
			,role_id(0)
		{
			gateway_ip[0] = '\0';
			role_name[0] = '\0';
		}
		prot::ProtHeader header;
		int32 login_result;
		char gateway_ip[MAX_32_CHAR_LEN];
		int32 gateway_port;
		Int64 role_id;
		char role_name[MAX_32_CHAR_LEN];
	};

	struct CSUserCreateRole
	{
		CSUserCreateRole()
			:header(USER_CREATE_ROLE_CS)
			, plat_id(0)
			, server_id(0)
			,profession(0)
			,avatar(0)

			
		{
			account[0] = '\0';
			role_name[0] = '\0';
		}
		prot::ProtHeader header;
		int32 plat_id;
		int32 server_id;	
		int32 profession;
		int32 avatar;
		char account[MAX_64_CHAR_LEN];//即帐号
		char role_name[MAX_32_CHAR_LEN];//即角色名称
	};

	//返回创建角色结果
	struct SCUserCreateRole
	{
		SCUserCreateRole()
			:header(USER_CREATE_ROLE_RET_SC)
			, create_result(LI_INVALID_VALUE)
			,gateway_port(0)
			,role_id(0)
		{
			gateway_ip[0] = '\0';
			role_name[0] = '\0';
		}
		prot::ProtHeader header;
		int32 create_result;
		char gateway_ip[MAX_32_CHAR_LEN];
		int32 gateway_port;
		Int64 role_id;
		char role_name[MAX_32_CHAR_LEN];//即角色名称
	};

}//end namespace rmiprotocol

#pragma pack(pop)
#endif