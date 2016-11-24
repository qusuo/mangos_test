#ifndef JXS_ITEMSTRUCT_H_
#define JXS_ITEMSTRUCT_H_
//道具数据，包括背包、角色装备和配件

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../include/battle_def.h"
#include "../../include/game_value_def.h"
#include "../item_data.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"

struct ItemStruct
{
	static const Int32 MAX_ITEM_NUM = 500;
	static const Int32 MAX_SAVE_GENERAL_EQUIP_NUM = MAX_GENERAL_EQUIP_NUM * MAX_GENERAL_NUM;
	//static const Int32 MAX_ROLE_ORDNANCE_NUM = SOLDIER_TYPE_COUNT * ROLE_ORDNANCE_GRID_NUM; //fix(hsj)

	static const Int32 ROLE_EQUIP_BEGIN_POS = 10000;
	static const Int32 ROLE_ORDNANCE_BEGIN_POS = 11000;
	static const Int32 GENERAL_EQUIP_BEGIN_POS = 12000;

	struct ItemStructParam
	{
		ItemStructParam()
			:db_opt(DB_OPT_INVALID)
			, grid_pos(0)
			, item_id(0)
			, item_num(0)
			, is_equip(0)
			, dead_line(0)
		{
			memset(equip_data, 0, sizeof(equip_data));
		}
		Int8 db_opt;
		Int16 grid_pos;
		Int32 item_id;
		Int32 item_num;
		Int64 dead_line;
		Int8 is_equip;
		char equip_data[EQUIP_LOGIC_DATA_LEN];
	};
	ItemStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();

	bool SerializeParams(Int16 param_num, ItemStructParam params[], jxsstr::Serializer& se);
	bool DeserializeParams(Int16& param_num, ItemStructParam params[], jxsstr::Deserializer& ds);

	//背包
	Int16 grid_item_num;
	ItemStructParam grid_items[MAX_ITEM_NUM];

	//将领身上装备
	Int16 general_equip_num;
	ItemStructParam general_equips[MAX_SAVE_GENERAL_EQUIP_NUM];

	//身上军械
	Int16 role_parts_num;
	ItemStructParam role_parts[MAX_ROLE_ORDNANCE_NUM];

	//角色装备（奇门循甲）
	Int16 role_equip_num;
	ItemStructParam role_equips[MAX_ROLE_EQUIP_NUM];
};

#endif