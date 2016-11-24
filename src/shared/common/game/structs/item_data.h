#ifndef JXS_ITEMDATA_H_
#define JXS_ITEMDATA_H_

#include "../include/data_len_def.h"

struct EquipAttr
{
	EquipAttr()
		:level(0)
		, reform_level(0)
		, exp(0)
	{

	}
	void Reset()
	{
		level = 0;
		reform_level = 0;
		exp = 0;
	}

	Int16 level;		//强化等级
	Int16 reform_level; //改造等级
	Int32 exp;
};

struct ItemParam
{
	ItemParam()
	:item_id(0)
	, item_num(0)
	{

	}
	Int32 item_id;
	Int32 item_num;
};

//struct EquipData
//{
//	enum PosType
//	{
//		POS_TYPE_INVALID,
//		POS_TYPE_EQUIP,			//装备背包
//		POS_TYPE_PARTS,			//配件背包	
//		POS_TYPE_FORMATION,		//阵型
//		POS_TYPE_SOLDIER,		//士兵
//
//	};
//
//	EquipData()
//		:level(0)
//		, exp(0)
//	{
//
//	}
//
//	Int16 level;
//	Int32 exp;
//};

#endif