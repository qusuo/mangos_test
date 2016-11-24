#include "chapter_struct.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

ChapterStruct::ChapterStruct()
{
	Reset();
}

ChapterStruct::~ChapterStruct()
{

}

bool ChapterStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret &= se.PutInt16(chapter_num);
	for (Int32 i = 0; i < chapter_num; ++i)
	{
		ret &= se.PutInt8(chapters[i].db_opt)
			&& se.PutInt32(chapters[i].chapter_id)
			&& se.PutInt8Vector(chapters[i].logic_data, sizeof(chapters[i].logic_data));
	}

	return ret;
}

bool ChapterStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	Int16 tmp = 0;
	ret &= ds.GetInt16(chapter_num);
	if (chapter_num > MAX_ALL_CHAPTER_NUM)
	{
		return false;
	}
	for (Int32 i = 0; i < chapter_num; ++i)
	{
		ret &= ds.GetInt8(chapters[i].db_opt)
			&& ds.GetInt32(chapters[i].chapter_id)
			&& ds.GetInt8Vector(chapters[i].logic_data, sizeof(chapters[i].logic_data), tmp);
	}

	return ret;
}

void ChapterStruct::Reset()
{
	chapter_num = 0;
	for (Int32 i = 0; i < MAX_ALL_CHAPTER_NUM; ++i)
	{
		chapters[i].db_opt = 0;
		chapters[i].chapter_id = 0;
		memset(chapters[i].logic_data, 0, sizeof(chapters[i].logic_data));		
	}
}
