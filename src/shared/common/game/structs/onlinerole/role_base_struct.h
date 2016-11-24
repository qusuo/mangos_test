#ifndef NOC_COMMON_GAME_STRUCTS_ROLEBASE_H_
#define NOC_COMMON_GAME_STRUCTS_ROLEBASE_H_
//author:hsj
// 角色基信息保存

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct RoleBaseStruct
{
	RoleBaseStruct();
	~RoleBaseStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	RoleID role_id;
	AccountName account;
	RoleName role_name;
	Int16 role_level;
	Int16 vip_level;
	Int32 gold;
	Int64 last_save_time;
};

#endif