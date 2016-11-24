#ifndef JXS_TASKSTRUCT_H_
#define JXS_TASKSTRUCT_H_

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../include/game_value_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct TaskStruct
{
	struct TaskItem
	{
		TaskItem()
			:task_id(0)
			, task_unit_type(0)
			, task_state(0)
			, data_param(0)
		{
		}
		Int32 task_id;
		Int16 task_unit_type;
		Int8 task_state;
		Int32 data_param;
	};

	TaskStruct();
	~TaskStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();

	bool SerializeItems(jxsstr::Serializer& se);
	bool DeserializeItems(jxsstr::Deserializer& ds, Int32 data_version);

	char record_data[TASK_RECORD_DATA_LEN];		//完成记录
	Int16 task_num;
	TaskItem task_items[MAX_ROLE_TASK_NUM];
};


#endif