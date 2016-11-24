#include "alliance_chapter_struct.h"

bool AllianceChapterStruct::Serialize(jxsstr::Serializer &se)
{
	return se.PutInt64(alliance_id) && se.PutInt32(logic_data_len) && se.PutInt8s(logic_data, logic_data_len);
}

bool AllianceChapterStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt64(alliance_id) && ds.GetInt32(logic_data_len);
	if (logic_data_len > 0)
	{
		ds.GetInt8s(logic_data, logic_data_len);
	}

	return res;
}

void AllianceChapterStruct::Reset()
{
	alliance_id = 0;
	logic_data_len = 0;
}