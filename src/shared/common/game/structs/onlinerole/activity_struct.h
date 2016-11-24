#ifndef JXS_ACTIVITYSTRUCT_H_
#define JXS_ACTIVITYSTRUCT_H_

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct ActivityStruct
{
	static const Int32 MAX_ACT_DATA_LEN = 512;
	static const Int32 MAX_ACT_NUM = 40;
	struct ActItem
	{
		ActItem()
			:act_id(0)
			, act_data_len(0)
		{
			memset(act_data, 0, sizeof(act_data));
		}
		Int32 act_id;
		Int16 act_data_len;
		char act_data[MAX_ACT_DATA_LEN];
	};
	ActivityStruct();
	~ActivityStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	Int16 act_num;
	ActItem act_items[MAX_ACT_NUM];
};

#endif