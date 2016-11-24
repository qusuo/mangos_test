#ifndef JXS_GAMEUTIL_H_
#define JXS_GAMEUTIL_H_
#include "server_def.h"

namespace gameutil
{
	/** RoleID 相关操作 **/
	bool IsRobot(RoleID role_id);
	RoleID PackRoleId(Int32 platform_id, Int32 server_id, Int32 db_index, bool is_robot, Int32 role_index);
	const std::string MakeAccountKey(const char* account, Int32 server_id, Int32 plat_id);
	Int64 PackAllianceId(Int32 platform_id, Int32 server_id, Int32 db_index, bool is_robot, Int32 alliance_index);
}

#endif