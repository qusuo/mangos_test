#ifndef JXS_ATTRBASESTRUCT_H_
#define JXS_ATTRBASESTRUCT_H_
//author:hsj
//离线基础属性

#include "../../include/game_value_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../../include/value_def.h"
#include "../attr_data.h"

struct AttrBaseStruct
{
	AttrBaseStruct();
	~AttrBaseStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	RoleID role_id;
	RoleName role_name;
	AccountName account;
	Int8 gender;
	Int8 avatar;				//头像
	Int16 hat;
	Int16 level;				//等级
	Int64 exp;					//经验
	Int32 boom;					//繁荣度
	Int32 cur_boom;				//当前繁荣度
	Int16 leadership;			//统率等级
	Int16 military_rank;		//军衔
	Int32 gold;
	Int32 resource[RESOURCE_TYPE_COUNT];			//资源
	Int32 resource_yields[RESOURCE_TYPE_COUNT];		//资源产量
	Int32 resource_store[RESOURCE_TYPE_COUNT];		//资源存储
	Int16 vip_level;
	Int32 total_charge_gold;
	Int64 alliance_id;
	Int64 origin_alliance_id;
	char alliance_name[MAX_32_CHAR_LEN];
	Int16 energy;
	Int16 cur_energy;
	Int32 reputation;
	Int16 reputation_level;

};

#endif