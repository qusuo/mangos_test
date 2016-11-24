#ifndef JXS_ATTRDATA_H_
#define JXS_ATTRDATA_H_
#include "../include/server_def.h"
#include "../include/battle_def.h"

struct AttrAddParam
{
	AttrAddParam() :add_mode(0), attr_type(0), add_value(0){}
	Int16 add_mode; //1=值，2=百分比, 3=其他非战斗属性类型加成
	Int16 attr_type;
	Int32 add_value;
};

struct AttrAddArrayStruct
{
	AttrAddArrayStruct()
		:param_num(0)
	{

	}
	static const Int32 MAX_ATTR_ADD_PARAM_NUM = MAX_NOR_ATTR_NUM * 2;
	Int16 param_num;
	AttrAddParam params[MAX_ATTR_ADD_PARAM_NUM];
};

struct SkillAttrAddStruct
{
	SkillAttrAddStruct()
		:soldier_type(0)
	{

	}
	Int32 soldier_type;
	AttrAddArrayStruct attr_add;
};

struct WartechAttrAddStruct
{
	WartechAttrAddStruct()
		:soldier_id(0)
	{

	}
	Int32 soldier_id;
	AttrAddArrayStruct attr_add;
};

struct OrdnanceAttrAddStruct
{
	OrdnanceAttrAddStruct()
		:soldier_type(0)
	{

	}
	Int32 soldier_type;
	AttrAddArrayStruct attr_add;
};


struct EquipAttrAddStruct
{
	EquipAttrAddStruct()
		:equip_pos(0)
	{

	}
	Int32 equip_pos;
	AttrAddArrayStruct attr_add;
};
#endif