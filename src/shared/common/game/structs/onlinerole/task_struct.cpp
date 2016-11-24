#include "task_struct.h"

TaskStruct::TaskStruct()
{
	Reset();
}

TaskStruct::~TaskStruct()
{

}

bool TaskStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = true;
	res &= se.PutInt8s(record_data, sizeof(record_data))
		&& SerializeItems(se);	
	return res;
}

bool TaskStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool res = true;
	Int16 tmp = 0;

	res &= ds.GetInt8s(record_data, sizeof(record_data))
		&& DeserializeItems(ds, data_version);

	return res;
}

bool TaskStruct::SerializeItems(jxsstr::Serializer& se)
{
	bool res = se.PutInt16(task_num);
	for (Int32 i = 0; i < task_num && i < MAX_ROLE_TASK_NUM; ++i)
	{
		res &= se.PutInt32(task_items[i].task_id)
			&& se.PutInt16(task_items[i].task_unit_type)
			&& se.PutInt8(task_items[i].task_state)
			&& se.PutInt32(task_items[i].data_param);
	}
	return res;
}

bool TaskStruct::DeserializeItems(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool res = ds.GetInt16(task_num);
	for (Int32 i = 0; i < task_num; ++i)
	{
		res &= ds.GetInt32(task_items[i].task_id)
			&& ds.GetInt16(task_items[i].task_unit_type)
			&& ds.GetInt8(task_items[i].task_state)
			&& ds.GetInt32(task_items[i].data_param);
	}
	return res;
}

void TaskStruct::Reset()
{
	task_num = 0;
	memset(task_items, 0, sizeof(task_items));
}
