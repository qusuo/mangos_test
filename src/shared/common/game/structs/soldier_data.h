#ifndef JXS_SOLIDERDATA_H_
#define JXS_SOLIDERDATA_H_
#include "../include/server_def.h"
#include "../include/battle_def.h"
#include "../include/game_value_def.h"
#include "../../include/function_def.h"

struct SoldierUnit
{
	SoldierUnit()
	:soldier_id(0)
	, soldier_num(0)
	{

	}
	Int32 soldier_id;
	Int32 soldier_num;
};

typedef SoldierUnit SoldierFormat[FORMATION_GRID_NUM];

struct SoldierTeam
{
	SoldierTeam()
		:pos(-1)
	{
	}
	void Reset()
	{
		pos = -1;
	}

	void AddSoldiers(Int32 format_pos, Int32 num)
	{
		if (format_pos < 0 || format_pos >= FORMATION_GRID_NUM)
		{
			return;
		}
		format[format_pos].soldier_num += num;
	}

	void RemoveSoldiers(Int32 format_pos, Int32 num)
	{
		if (format_pos < 0 || format_pos >= FORMATION_GRID_NUM)
		{
			return;
		}
		format[format_pos].soldier_num -= num;
		format[format_pos].soldier_num = Max(0, format[format_pos].soldier_num);
	}

	Int8 pos;  //从0 开始
	SoldierFormat format;
};

#endif