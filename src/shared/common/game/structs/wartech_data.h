#ifndef JXS_WARTECHDATA_H_
#define JXS_WARTECHDATA_H_
#include "../include/server_def.h"
#include "../include/battle_def.h"
#include "../include/game_value_def.h"

struct WarTechData
{
	WarTechData()
		:wartech_id(0)
		, level(0)
	{

	}
	Int16 wartech_id;
	Int16 level;
};

struct SoldierWarTechData
{
	SoldierWarTechData()
		:soldier_id(0)
		, grid_num(0)
	{
		memset(grid_state, 0, sizeof(grid_state));
	}
	Int16 soldier_id;
	Int16 grid_num;
	Int16 grid_state[MAX_WARTECH_GRID_NUM];
};

#endif