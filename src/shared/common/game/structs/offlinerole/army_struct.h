#ifndef JXS_ARMYSTRUCT_H_
#define JXS_ARMYSTRUCT_H_
//author:hsj
//部队数据

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../army_format_data.h"

struct ArmyStruct
{
	struct ArmyInfo
	{
		ArmyInfo()
			:pos(INVALID_POS)
		{

		}
		Int8 pos;
		ArmyBuffData army_data;
	};

	ArmyStruct();
	~ArmyStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);

	bool SerializeArmyBuffData(ArmyBuffData& data, jxsstr::Serializer& se);
	bool DeserializeArmyBuffData(ArmyBuffData& data, jxsstr::Deserializer& ds, Int32 data_version);

	void Reset();
	Int8 army_num;
	ArmyInfo armies[MAX_ROLE_ARMY_NUM];
	ArmyBuffData city_army;
	ArmyBuffData arena_army;
};

#endif