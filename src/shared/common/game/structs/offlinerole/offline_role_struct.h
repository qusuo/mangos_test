#ifndef JSX_OFFLINEROLESTRUCT_H_
#define JSX_OFFLINEROLESTRUCT_H_
//author:hsj
//保存角色数据

#include "building_struct.h"
#include "soldier_struct.h"
#include "attr_add_struct.h"
#include "attr_base_struct.h"
#include "role_common_struct.h"
#include "army_struct.h"

struct OfflineRoleStruct
{
	OfflineRoleStruct();
	~OfflineRoleStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();

	Int64 save_time;
	AttrBaseStruct attr_base;
	AttrAddStruct attr_add;
	BuildingStruct building;
	SoldierStruct soldier;
	RoleCommonStruct role_comm;	
	ArmyStruct armies;
};




#endif