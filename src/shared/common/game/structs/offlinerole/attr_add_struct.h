#ifndef JXS_ATTRADDSTRUCT_H_
#define JXS_ATTRADDSTRUCT_H_
//author:hsj
//加成属性

#include "../../include/game_value_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../attr_data.h"
//#include "../../../../common/protocol/userprotocol/cs_role_attr_add_prot.h"

struct AttrAddStruct
{
	static const Int32 MAX_SOLDIER_ID_NUM = 100; //士兵种数
	static const Int32 MAX_SOLDIER_WORTECH_NUM = 10;  //每种士兵最大装备兵法数
	struct WartechSoldier
	{
		WartechSoldier()
			:soldier_id(0)
			, wartech_add_num(0)
		{

		}
		Int32 soldier_id;
		Int16 wartech_add_num;
		AttrAddParam wartech_add[MAX_SOLDIER_WORTECH_NUM];
	};

	AttrAddStruct();
	~AttrAddStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();

	bool SerializeAddParams(AttrAddArrayStruct& attrs, jxsstr::Serializer& se);
	bool DeserializeAddParams(AttrAddArrayStruct& attrs, jxsstr::Deserializer& ds, Int32 data_version);

	Int16 skill_add_num;
	SkillAttrAddStruct skill_add[SOLDIER_TYPE_COUNT];

	Int16 wartech_add_num;
	WartechAttrAddStruct wartech_add[MAX_SOLDIER_ID_NUM];

	Int16 equip_add_num;
	EquipAttrAddStruct equip_add[FORMATION_GRID_NUM];

	Int16 ordnance_add_num;
	OrdnanceAttrAddStruct ordnance_add[SOLDIER_TYPE_COUNT];
};

#endif