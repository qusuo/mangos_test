#include "attr_base_struct.h"

AttrBaseStruct::AttrBaseStruct()
{
	Reset();
}

AttrBaseStruct::~AttrBaseStruct()
{

}

bool AttrBaseStruct::Serialize(jxsstr::Serializer& se)
{
	bool ret = true;
	ret &= se.PutInt64(role_id)
		&& se.PutInt8Vector(role_name, sizeof(role_name))
		&& se.PutInt8Vector(account, sizeof(account))
		&& se.PutInt8(gender)
		&& se.PutInt8(avatar)
		&& se.PutInt16(hat)
		&& se.PutInt16(level)
		&& se.PutInt64(exp)
		&& se.PutInt32(boom)
		&& se.PutInt32(cur_boom)
		&& se.PutInt16(leadership)
		&& se.PutInt16(military_rank)
		&& se.PutInt32(gold)
		&& se.PutInt32Vector(resource, RESOURCE_TYPE_COUNT)
		&& se.PutInt32Vector(resource_yields, RESOURCE_TYPE_COUNT)
		&& se.PutInt32Vector(resource_store, RESOURCE_TYPE_COUNT)
		&& se.PutInt16(vip_level)
		&& se.PutInt32(total_charge_gold)
		&& se.PutInt64(alliance_id)
		&& se.PutInt64(origin_alliance_id)
		&& se.PutInt8Vector(alliance_name, sizeof(alliance_name))
		&& se.PutInt16(energy)
		&& se.PutInt16(cur_energy)
		&& se.PutInt32(reputation)
		&& se.PutInt16(reputation_level);

	return ret;
}

bool AttrBaseStruct::Deserialize(jxsstr::Deserializer& ds, Int32 data_version)
{
	bool ret = true;
	Int16 tmp = 0;
	ret &= ds.GetInt64(role_id)
		&& ds.GetInt8Vector(role_name, sizeof(role_name), tmp)
		&& ds.GetInt8Vector(account, sizeof(account), tmp)
		&& ds.GetInt8(gender)
		&& ds.GetInt8(avatar)
		&& ds.GetInt16(hat)
		&& ds.GetInt16(level)
		&& ds.GetInt64(exp)
		&& ds.GetInt32(boom)
		&& ds.GetInt32(cur_boom)
		&& ds.GetInt16(leadership)
		&& ds.GetInt16(military_rank)
		&& ds.GetInt32(gold)
		&& ds.GetInt32Vector(resource, RESOURCE_TYPE_COUNT, tmp)
		&& ds.GetInt32Vector(resource_yields, RESOURCE_TYPE_COUNT, tmp)
		&& ds.GetInt32Vector(resource_store, RESOURCE_TYPE_COUNT, tmp)
		&& ds.GetInt16(vip_level)
		&& ds.GetInt32(total_charge_gold)
		&& ds.GetInt64(alliance_id)
		&& ds.GetInt64(origin_alliance_id)
		&& ds.GetInt8Vector(alliance_name, sizeof(alliance_name), tmp)
		&& ds.GetInt16(energy)
		&& ds.GetInt16(cur_energy)
		&& ds.GetInt32(reputation)
		&& ds.GetInt16(reputation_level);

	return ret;
}

void AttrBaseStruct::Reset()
{
	role_id = 0;
	memset(role_name, 0, sizeof(role_name));
	memset(account, 0, sizeof(account));
	gender = 0;
	avatar = 0;
	hat = 0;
	level = 0;
	exp = 0;
	boom = 0;
	cur_boom = 0;
	leadership = 0;
	military_rank = 0;
	gold = 0;
	memset(resource, 0, sizeof(resource));
	memset(resource_yields, 0, sizeof(resource_yields));
	memset(resource_store, 0, sizeof(resource_store));
	vip_level = 0;
	total_charge_gold = 0;
	alliance_id = 0;
	memset(alliance_name, 0, sizeof(alliance_name));
	energy = 0;
	cur_energy = 0;
	reputation = 0;
	reputation_level = 0;
}