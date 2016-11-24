#ifndef NOC_COMMON_GAME_STRUCTS_RMILOGINSTRUCTS_H_
#define NOC_COMMON_GAME_STRUCTS_RMILOGINSTRUCTS_H_
//author:hsj

#include "../include/server_def.h"

//登录查询
struct RoleLoginLD
{
	RoleLoginLD()
		:plat_id(0)
		,server_id(0)
	{
		account[0] = '\0';
		device_code[0] = '\0';
	}
	Int32 plat_id;
	Int32 server_id;
	AccountName account;
	char device_code[MAX_128_CHAR_LEN];  //机器码
};

//登录查询结果
struct RoleLoginDL
{
	enum ResCode
	{
		RC_SUCC = 0,
		RC_ROLE_NOT_EXIST = -1,
		RC_EXCEPTION = -2,
	};
	RoleLoginDL()
		:res_code(0)
		,role_id(0)
		, forbid_time(0)
		, online_time(0)
		, logout_time(0)
		, anti_addiction(0)
	{
		role_name[0] = '\0';
	}
	Int32 res_code;  //0 成功， -1 角色不存在, -2已经在线， -3异常
	RoleID role_id;
	RoleName role_name;
	Int64 forbid_time;
	Int32 online_time;
	Int64 logout_time;
	Int32 anti_addiction;	//是否已通过
};

//创建角色
struct CreateRoleLD
{
	CreateRoleLD()
		:plat_id(0)
		,server_id(0)
		,profession(0)
		,avatar(0)
		,reserved(0)
	{
		account[0] = '\0';
		role_name[0] = '\0';
		client_sys[0] = '\0';
		device_code[0] = '\0';
	}
	Int32 plat_id;
	Int32 server_id;
	AccountName account;
	RoleName role_name;
	Int8 profession;
	Int8 avatar;
	Int16 reserved;
	char client_sys[MAX_128_CHAR_LEN];
	char device_code[MAX_128_CHAR_LEN];
};

//创建角色返回
struct CreateRoleDL
{
	enum ResCode
	{
		RC_SUCC = 0,
		RC_USER_EXIST = -1,
		RC_ROLE_NAME_EXIST = -2,
		RC_EXCEPTION = -3,
	};
	CreateRoleDL()
		:res_code(0)
		,role_id(0)
	{
	}
	Int32 res_code;
	RoleID role_id;
};

#endif