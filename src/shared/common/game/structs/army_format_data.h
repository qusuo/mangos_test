#ifndef JXS_ARMYFORMATDATA_H_
#define JXS_ARMYFORMATDATA_H_
//部队阵型

#include "soldier_data.h"
#include "general_data.h"
#include "../../util/util_fun.h"

//部队阵型信息
struct ArmyFormatData
{
	ArmyFormatData()
	{
		for (Int32 i = 0; i < GENERAL_TYPE_COUNT; ++i)
		{
			general_format[i] = -1;
		}
	}

	bool IsEmpty()
	{
		for (Int32 i = 0; i < FORMATION_GRID_NUM; ++i)
		{
			if (soldier_format[i].soldier_id > 0 && soldier_format[i].soldier_num > 0)
			{
				return false;
			}
		}
		return true;
	}

	SoldierFormat soldier_format;
	GeneralFormat general_format;
};

//部队阵型信息
struct ArmyFormatDataEx
{
	ArmyFormatDataEx()
	{
		memset(general_format, -1, sizeof(general_format));
	}

	SoldierFormat soldier_format;
	GeneralFormatEx general_format;
};

//部队暂存信息，用作离线后出战部队的数据暂存
struct ArmyBuffData
{
	void* operator new(size_t size);
	void operator delete(void* p);
	void Reset()
	{
		memset(soldiers, 0, sizeof(soldiers));
		memset(generals, 0, sizeof(generals));
		for (Int32 i = 0; i < GENERAL_TYPE_COUNT; ++i)
		{
			generals[i].store_index = -1;
		}
	}


	SoldierUnit soldiers[FORMATION_GRID_NUM];
	GeneralData generals[GENERAL_TYPE_COUNT];
};

#endif