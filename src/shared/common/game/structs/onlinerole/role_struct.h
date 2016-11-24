#ifndef NOC_COMMON_GAME_STRUCTS_ROLESTRUCT_H_
#define NOC_COMMON_GAME_STRUCTS_ROLESTRUCT_H_
//author:hsj
//保存角色数据

#include "role_base_struct.h"
#include "chapter_struct.h"
#include "item_struct.h"
#include "role_logic_struct.h"
#include "activity_struct.h"
#include "general_struct.h"
#include "task_struct.h"
#include "role_login_struct.h"

struct RoleStruct
{
	RoleStruct();
	~RoleStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	RoleBaseStruct role_base;
	ChapterStruct chapter;			//副本
	ItemStruct items;				//道具
	RoleLogicStruct logic_attr;
	ActivityStruct activity;
	GeneralStruct general;
	TaskStruct task;
	RoleLoginStruct role_login;
};

#endif