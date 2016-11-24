#ifndef JXS_ALLIANCEDATASTRUCT_H_
#define JXS_ALLIANCEDATASTRUCT_H_

#include "alliance_struct.h"
#include "alliance_member_struct.h"
#include "alliance_chapter_struct.h"
#include "alliance_info_struct.h"
#include "alliance_shop_struct.h"
#include "alliance_tech_hall_struct.h"
#include "alliance_welfare_struct.h"

struct AllianceBuildingDataStruct
{
	AllianceBuildingDataStruct()
		: info_num(0)
		//, shop_num(0)
		//, welfare_num(0)
		, tech_hall_num(0)
		{}

	~AllianceBuildingDataStruct() {}

	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	Int32 info_num;
	AllianceInfoStruct infos[MAX_ALLIANCE_NUM];
	/**
	Int32 shop_num;
	AllianceShopStruct shops[MAX_ALLIANCE_NUM];
	Int32 welfare_num;
	AllianceWelfareStruct welfares[MAX_ALLIANCE_NUM];
	*/
	Int32 tech_hall_num;
	AllianceTechHallStruct tech_halls[MAX_ALLIANCE_NUM];
};

struct AllianceMemberDataStruct
{
	AllianceMemberDataStruct():alliance_id(0), num(0) {}
	~AllianceMemberDataStruct() {}

	bool Serialize(jxsstr::Serializer &se);
	bool Deserialize(jxsstr::Deserializer &ds);
	void Reset();

	Int64 alliance_id;
	Int32 num;
	AllianceMemberStruct members[MAX_ALLIANCE_MEMBER_NUM];
};

struct AllianceChapterDataStruct
{
	AllianceChapterDataStruct():alliance_id(0),num(0) {}
	~AllianceChapterDataStruct() {}

	bool Serialize(jxsstr::Serializer &se);
	bool Deserialize(jxsstr::Deserializer &ds);
	void Reset();

	Int64 alliance_id;
	Int32 num;
	AllianceChapterStruct chapters[MAX_ALLIANCE_NUM];
};

struct AllianceDataStruct
{
	AllianceDataStruct() :alliance_num(0), member_num(0), chapter_num(0), logic_data_len(0){}
	~AllianceDataStruct() {}
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	Int32 alliance_num;
	AllianceStruct alliances[MAX_ALLIANCE_NUM];
	Int32 member_num;
	AllianceMemberDataStruct members[MAX_ALLIANCE_NUM];
	Int32 chapter_num;
	AllianceChapterStruct chapters[MAX_ALLIANCE_NUM];
	AllianceBuildingDataStruct building_data;
	Int32 logic_data_len;
	char logic_data[ALLIANCE_LOGIC_DATA_LEN];
};

struct AllianceShopDataStruct
{
	AllianceShopDataStruct():num(0) {}
	~AllianceShopDataStruct() {}

	bool Serialize(jxsstr::Serializer &se);
	bool Deserialize(jxsstr::Deserializer &ds);
	void Reset();

	Int32 num;
	AllianceShopStruct shops[MAX_ALLIANCE_NUM];
};

struct AllianceWelfareDataStruct
{
	AllianceWelfareDataStruct():num(0) {}
	~AllianceWelfareDataStruct() {}

	bool Serialize(jxsstr::Serializer &se);
	bool Deserialize(jxsstr::Deserializer &ds);
	void Reset();
	
	Int32 num;
	AllianceWelfareStruct welfares[MAX_ALLIANCE_NUM];
};

#endif