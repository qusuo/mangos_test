#ifndef JXS_SOLDIERSTRUCT_H_
#define JXS_SOLDIERSTRUCT_H_
//author:hsj
//士兵数据

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../soldier_data.h"

struct SoldierStruct
{
	SoldierStruct();
	~SoldierStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	Int16 inner_unit_num;
	SoldierUnit inner_units[MAX_SOLDIER_UNIT_NUM];
	Int16 injured_unit_num;
	SoldierUnit injured_units[MAX_SOLDIER_UNIT_NUM];
};

#endif