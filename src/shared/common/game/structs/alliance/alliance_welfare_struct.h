﻿#ifndef JXS_ALLIANCE_WELFARE_STRUCT_H_
#define JXS_ALLIANCE_WELFARE_STRUCT_H_

#include "../../include/game_value_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct AllianceWelfareStruct
{
	AllianceWelfareStruct() :alliance_id(0), logic_data_len(0){}

	bool Serialize(jxsstr::Serializer &se);
	bool Deserialize(jxsstr::Deserializer &ds);
	void Reset();

	Int64 alliance_id;
	Int32 logic_data_len;
	static const Int32 ALLIANCE_WELFARE_LOGIC_DATA_LEN = 30 * ALLIANCE_LOGIC_DATA_LEN;
	char logic_data[ALLIANCE_WELFARE_LOGIC_DATA_LEN];
};

#endif