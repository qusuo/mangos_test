#ifndef JXS_BUILDINGDATA_H_
#define JXS_BUILDINGDATA_H_
#include "../include/server_def.h"


struct BuildingData
{
	BuildingData()
	{
		Reset();
	}
	Int16 building_id;				// 当前id
	Int16 level;					// 用户的建筑当前等级
	Int16 resource_building_type;	// 空地上面的矿产等
	Int16 reserved;

	void Reset()
	{
		building_id = 0;			// 当前id
		level = 0;					// 用户的建筑当前等级
		resource_building_type = 0;	// 空地上面的矿产等
		reserved = 0;
	}
};

#endif