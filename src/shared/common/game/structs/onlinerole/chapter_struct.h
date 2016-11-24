#ifndef JXS_CHAPTERSTRUCT_H_
#define JXS_CHAPTERSTRUCT_H_

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../include/game_value_def.h"

struct ChapterStruct
{
	enum{
		ROLE_CHAPTER_ROB_TYPE_EXTREME,	//1 : 极限, 
		ROLE_CHAPTER_ROB_TYPE_WARTECH,	//2 : 军工, 
		ROLE_CHAPTER_ROB_TYPE_EQUIP,	//3 : 装备, 
		ROLE_CHAPTER_ROB_TYPE_PARTS,	//4 : 配件
		ROLE_CHAPTER_ROB_TYPE_COUNT = ROLE_CHAPTER_ROB_TYPE_PARTS - ROLE_CHAPTER_ROB_TYPE_EXTREME + 1
	};

	enum{
		ROLE_CHPATER_TIME_LIMIT_TYPE_EXPLOER,
		ROLE_CHPATER_TIME_LIMIT_TYPE_COUNT = ROLE_CHPATER_TIME_LIMIT_TYPE_EXPLOER - ROLE_CHPATER_TIME_LIMIT_TYPE_EXPLOER + 1,
	};

	ChapterStruct();
	~ChapterStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();
	struct ChapterParam
	{
		ChapterParam()
			:db_opt(0)
			,chapter_id(0)
		{
			memset(logic_data, 0, sizeof(logic_data));
		}
		Int8 db_opt;
		Int32 chapter_id;
		Int8 logic_data[CHAPTER_LOGIC_DATA_LEN];
	};

	Int16 chapter_num;
	static const Int32 MAX_ALL_CHAPTER_NUM = MAX_ROLE_CHAPTER_NUM + ROLE_CHAPTER_ROB_TYPE_COUNT + ROLE_CHPATER_TIME_LIMIT_TYPE_COUNT;
	ChapterParam chapters[MAX_ALL_CHAPTER_NUM];
};


#endif