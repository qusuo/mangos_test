#ifndef JXS_ROLELOGINSTRUCT_H_
#define JXS_ROLELOGINSTRUCT_H_
//author:hsj
// 角色基信息保存

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct RoleLoginStruct
{
	RoleLoginStruct();
	~RoleLoginStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	Int32 online_time;
	Int64 logout_time; //实现用保存时间
	Int32 anti_addiction; //是否通过防沉迷
	Int64 forbid_time;
	char client_sys[MAX_128_CHAR_LEN];
	char device_code[MAX_128_CHAR_LEN];
	Int64 create_time;  //建号时间
};

#endif