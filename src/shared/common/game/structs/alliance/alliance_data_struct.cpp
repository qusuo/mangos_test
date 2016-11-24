#include "alliance_data_struct.h"
#include "../../../log/logapi.h"

bool AllianceDataStruct::Serialize(jxsstr::Serializer& se)
{
	bool res = se.PutInt32(logic_data_len) && se.PutInt8s(logic_data, logic_data_len);
	Int32 num = (alliance_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : alliance_num;
	num = (num < 0) ? 0 : num;
	res &= se.PutInt32(num);
	for (Int32 i = 0; i < num; ++i)
	{
		res &= alliances[i].Serialize(se);
	}

	num = (member_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : member_num;
	num = (num < 0) ? 0 : num;
	res &= se.PutInt32(num);
	for (Int32 i = 0; i < num; ++i)
	{
		res &= members[i].Serialize(se);
	}

	num = (chapter_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : chapter_num;
	num = (num < 0) ? 0 : num;
	res &= se.PutInt32(num);
	for (Int32 i = 0; i < num; ++i)
	{
		res &= chapters[i].Serialize(se);
	}

	res &= building_data.Serialize(se);

	return res;
}

bool AllianceDataStruct::Deserialize(jxsstr::Deserializer& ds)
{
	
	bool res= ds.GetInt32(logic_data_len) && ds.GetInt8s(logic_data, logic_data_len);
	res &= ds.GetInt32(alliance_num);
	alliance_num = (alliance_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : alliance_num;
	alliance_num = (alliance_num < 0) ? 0 : alliance_num;
	for (Int32 i = 0; i < alliance_num; ++i)
	{
		res &= alliances[i].Deserialize(ds);
	}

	res &= ds.GetInt32(member_num);
	member_num = (member_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : member_num;
	member_num = (member_num < 0) ? 0 : member_num;
	for (Int32 i = 0; i < member_num; ++i)
	{
		res &= members[i].Deserialize(ds);
	}

	res &= ds.GetInt32(chapter_num);
	chapter_num = (chapter_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : chapter_num;
	chapter_num = (chapter_num < 0) ? 0 : chapter_num;
	for (Int32 i = 0; i < chapter_num; ++i)
	{
		res &= chapters[i].Deserialize(ds);
	}

	res &= building_data.Deserialize(ds);

	return res;
}

void AllianceDataStruct::Reset()
{	
	for (Int32 i = 0; i < alliance_num && i < MAX_ALLIANCE_NUM; ++i)
	{
		alliances[i].Reset();
	}
	alliance_num = 0;

	for (Int32 i = 0; i < member_num && i < MAX_ALLIANCE_NUM; ++i)
	{
		members[i].Reset();
	}
	member_num = 0;

	for (Int32 i = 0; i < member_num && i < MAX_ALLIANCE_NUM; ++i)
	{
		chapters[i].Reset();
	}
	chapter_num = 0;
	logic_data_len = 0;
}

bool AllianceBuildingDataStruct::Serialize(jxsstr::Serializer& se)
{
	Int32 num = (info_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM: info_num;
	num = (num > 0) ? num: 0;
	bool res = se.PutInt32(num);
	for (Int32 i = 0; i < num; ++i)
	{
		res &= infos[i].Serialize(se);
	}

	/**

	num = (welfare_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : welfare_num;
	num = (num > 0) ? num : 0;
	res &= se.PutInt32(num);
	for (Int32 i = 0; i < num; ++i)
	{
		res &= welfares[i].Serialize(se);
	}
	*/

	num = (tech_hall_num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : tech_hall_num;
	num = (num > 0) ? num : 0;
	res &= se.PutInt32(num);
	for (Int32 i = 0; i < num; ++i)
	{
		res &= tech_halls[i].Serialize(se);
	}

	return res;
}

bool AllianceBuildingDataStruct::Deserialize(jxsstr::Deserializer& ds)
{
	Int32 num = 0;
	bool res = ds.GetInt32(num);
	num = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM: num;
	num = (num > 0) ? num : 0;
	for (Int32 i = 0; i < num; ++i)
	{
		res &= infos[i].Deserialize(ds);
	}

	/**
	num = 0;
	res &= ds.GetInt32(num);
	for (Int32 i = 0; i < num && num < MAX_ALLIANCE_NUM; ++i)
	{
		res &= shops[i].Deserialize(ds);
	}

	num = 0;
	res &= ds.GetInt32(num);
	for (Int32 i = 0; i < num && num < MAX_ALLIANCE_NUM; ++i)
	{
		res &=welfares[i].Deserialize(ds);
	}
	*/

	res &= ds.GetInt32(num);
	num = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : num;
	num = (num > 0) ? num : 0;
	for (Int32 i = 0; i < num && num < MAX_ALLIANCE_NUM; ++i)
	{
		res &= tech_halls[i].Deserialize(ds);
	}

	return res;
}

void AllianceBuildingDataStruct::Reset()
{
	info_num = 0;
	//shop_num = 0;
	//welfare_num = 0;
	tech_hall_num = 0;
}

bool AllianceMemberDataStruct::Serialize(jxsstr::Serializer &se)
{
	bool res = se.PutInt64(alliance_id);
	Int32 count = (num > MAX_ALLIANCE_MEMBER_NUM) ?MAX_ALLIANCE_MEMBER_NUM:num;
	count = (count > 0) ? count: 0;
	res &= se.PutInt32(count);
	for (Int32 i = 0; i < count; ++i)
	{
		res &= members[i].Serialize(se);
	}

	return res;
}

bool AllianceMemberDataStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt64(alliance_id) && ds.GetInt32(num);
	num = (num > MAX_ALLIANCE_MEMBER_NUM) ? MAX_ALLIANCE_MEMBER_NUM : num;
	num = (num > 0) ? num : 0;
	DEBUG_LOG("alliance id[%lld] alliance member num [%d]", alliance_id, num);
	for (Int32 i = 0; i < num; ++i)
	{
		res &= members[i].Deserialize(ds);
	}

	return res;
}

void AllianceMemberDataStruct::Reset()
{
	alliance_id = 0;
	num = 0;
}

bool AllianceChapterDataStruct::Serialize(jxsstr::Serializer &se)
{
	Int32 count = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : num;
	count = (count < 0) ? 0 : count;
	bool res = se.PutInt32(count);
	for (Int32 i = 0; i < count; ++i)
	{
		res &= chapters[i].Serialize(se);
	}
	return res;
}

bool AllianceChapterDataStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt32(num);
	num = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : num;
	num = (num > 0) ? num : 0;
	for (Int32 i = 0; i < num; ++i)
	{
		res &= chapters[i].Deserialize(ds);
	}
	return res;
}

void AllianceChapterDataStruct::Reset()
{
	alliance_id = 0;
	num = 0;
}

bool AllianceShopDataStruct::Serialize(jxsstr::Serializer &se)
{
	Int32 count = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : num;
	count = (count > 0) ? count : 0;
	bool res = se.PutInt32(count);
	for (Int32 i = 0; i < count; ++i)
	{
		res &= shops[i].Serialize(se);
	}
	return res;
}

bool AllianceShopDataStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt32(num);
	num = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : num;
	num = (num > 0) ? num : 0;
	for (Int32 i = 0; i < num; ++i)
	{
		res &= shops[i].Deserialize(ds);
	}
	return res;
}

void AllianceShopDataStruct::Reset()
{
	num = 0;
}

bool AllianceWelfareDataStruct::Serialize(jxsstr::Serializer &se)
{
	Int32 count = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : num;
	count = (count > 0) ? count : 0;
	bool res = se.PutInt32(count);
	for (Int32 i = 0; i < count; ++i)
	{
		res &= welfares[i].Serialize(se);
	}
	return res;
}

bool AllianceWelfareDataStruct::Deserialize(jxsstr::Deserializer &ds)
{
	bool res = ds.GetInt32(num);
	num = (num > MAX_ALLIANCE_NUM) ? MAX_ALLIANCE_NUM : num;
	num = (num > 0) ? num : 0;
	for (Int32 i = 0; i < num; ++i)
	{
		res &= welfares[i].Deserialize(ds);
	}
	return res;
}

void AllianceWelfareDataStruct::Reset()
{
	num = 0;
}