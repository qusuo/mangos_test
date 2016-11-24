#ifndef JXS_USERMETA_H_
#define JXS_USERMETA_H_
//角色相关数据交换结构 
//请确保4字节对齐

#include "../include/server_def.h"
#include "../include/posi.h"
#include "../../../common/include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

struct GlobalUserInfo
{
	GlobalUserInfo()
		:role_id(0)
		, vip_level(0)
		, avatar(0)
		, gender(0)
		, hat(0)
		, role_level(0)
		, boom(0)
		, cur_boom(0)
		, military_rank(0)
		, alliance_id(0)
		, alliance_post(0)
		, reserve(0)
		, force(0)
		, net_id(0)
		, silent_end_time(0)
	{
		memset(role_name, 0, sizeof(role_name));
		memset(account, 0, sizeof(account));
		memset(alliance_name, 0, sizeof(alliance_name));
	}
	RoleID role_id;
	Int16 vip_level;
	Int8 avatar;
	Int8 gender;
	Int16 hat;
	Int16 role_level;	//等级
	Int32 boom;
	Int32 cur_boom;
	Int32 military_rank;//军衔
	Int64 alliance_id;	//军团id
	Int16 alliance_post;
	Int16 reserve;
	Int64 force;
	Int32 net_id;
	Int64 silent_end_time;	//禁言结束时间

	Posi pos;
	RoleName role_name;
	AccountName account;
	char alliance_name[MAX_32_CHAR_LEN];
};

struct GlobalUserInfoTest
{
	GlobalUserInfoTest()
		:role_id(0)
		/*, vip_level(0)
		, avatar(0)
		, gender(0)
		, hat(0)
		, role_level(0)
		, boom(0)
		, cur_boom(0)
		, military_rank(0)
		, alliance_id(0)
		, alliance_post(0)
		, reserve(0)
		, force(0)
		, net_id(0)*/
	{
		/*memset(role_name, 0, sizeof(role_name));
		memset(account, 0, sizeof(account));
		memset(alliance_name, 0, sizeof(alliance_name));*/
	}
	RoleID role_id;
	Int16 vip_level;
	Int8 avatar;
	Int8 gender;
	Int16 hat;
	Int16 role_level;	//等级
	Int32 boom;
	Int32 cur_boom;
	Int32 military_rank;//军衔
	Int64 alliance_id;
	Int16 alliance_post;
	Int16 reserve;
	Int64 force;
	Int32 net_id;

	/*RoleName role_name;
	AccountName account;
	char alliance_name[MAX_32_CHAR_LEN];
	Posi pos;*/
};

#pragma pack(pop) 
#endif