#include "game_util.h"

namespace gameutil
{
	// 目前的位最大只用到53位
	// platform_id: 0 ~ 511
	// server_id: 0 ~ 4095
	// db_index: 0 ~ 255
	// role_type:0~1
	// role_index: 0 ~ 4194303     //note(hsj) 这里要注意一下，一个服的最大角色人数只能去到4百万
	// 每个压缩字段所占用的bit长度
	static const int ROLE_INDEX_BIT_LEN = 23;
	static const int ROLE_TYPE_BIT_LEN = 1;			//0 = 角色， 1= 机器人
	static const int DB_INDEX_BIT_LEN = 8;
	static const int SERVER_ID_BIT_LEN = 12;
	static const int PLATFORM_ID_BIT_LEN = 9;

	// 每个压缩字段相对低位的偏移
	static const int ROLE_INDEX_BIT_OFFSET = 0;
	static const int ROLE_TYPE_BIT_OFFSET = ROLE_INDEX_BIT_OFFSET + ROLE_INDEX_BIT_LEN;
	static const int DB_INDEX_BIT_OFFSET = ROLE_TYPE_BIT_OFFSET + ROLE_TYPE_BIT_LEN;
	static const int SERVER_ID_BIT_OFFSET = DB_INDEX_BIT_OFFSET + DB_INDEX_BIT_LEN;
	static const int PLATFORM_ID_BIT_OFFSET = SERVER_ID_BIT_OFFSET + SERVER_ID_BIT_LEN;
	
	bool IsRobot(RoleID role_id)
	{
		return ((1 << ROLE_TYPE_BIT_OFFSET) & role_id) != 0;
	}

	RoleID PackRoleId(Int32 platform_id, Int32 server_id, Int32 db_index, bool is_robot, Int32 role_index)
	{
		Int32 role_type = is_robot ? 1 : 0;
		return ((RoleID)platform_id << PLATFORM_ID_BIT_OFFSET)
			+ ((RoleID)server_id << SERVER_ID_BIT_OFFSET)
			+ ((RoleID)db_index << DB_INDEX_BIT_OFFSET)
			+ ((RoleID)role_type << ROLE_TYPE_BIT_OFFSET)
			+ role_index;
	}

	const std::string MakeAccountKey(const char* account, Int32 server_id, Int32 plat_id)
	{
		char key[258] = { 0 };
		key[0] = '\0';
		sprintf(key, "%s_%d_%d", account, plat_id, server_id);
		return key;
	}

	Int64 PackAllianceId(Int32 platform_id, Int32 server_id, Int32 db_index, bool is_robot, Int32 alliance_index)
	{
		Int32 role_type = is_robot ? 1 : 0;
		return ((Int64)platform_id << PLATFORM_ID_BIT_OFFSET)
			+ ((Int64)server_id << SERVER_ID_BIT_OFFSET)
			+ ((Int64)db_index << DB_INDEX_BIT_OFFSET)
			+ ((Int64)role_type << ROLE_TYPE_BIT_OFFSET)
			+ alliance_index;
	}
}
