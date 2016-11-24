#ifndef JXS_BUILDINGSTRUCT_H_
#define JXS_BUILDINGSTRUCT_H_
//author:hsj
//士兵数据

#include "../../include/building_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../building_data.h"
#include "../../include/posi.h"

struct BuildingStruct
{
	BuildingStruct();
	~BuildingStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	Int16 building_num;
	BuildingData buildings[MAX_BUILDING_NUM];
	Posi city_pos;

};

#endif