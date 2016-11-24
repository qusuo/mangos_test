#ifndef JXS_GENERALSTRUCT_H_
#define JXS_GENERALSTRUCT_H_

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../include/game_value_def.h"

struct GeneralStruct
{
	static const Int32 MAX_GENERAL_NUM = 300;
	struct EquipStructParam
	{
		EquipStructParam()
			:db_opt(DB_OPT_INVALID)
			, formation_pos(0)
			, item_id(0)
		{
		}
		Int8 db_opt;
		Int16 formation_pos;
		Int32 item_id;
		char equip_data[EQUIP_LOGIC_DATA_LEN];
	};
	struct GeneralItem
	{
		GeneralItem()
			:db_opt(0)
			, general_id(0)
			, grid_pos(0)
		{
			memset(logic_data, 0, sizeof(logic_data));
		}
		Int8 db_opt;
		Int32 general_id;
		Int16 grid_pos; //store_index
		char logic_data[GENERAL_LOGIC_DATA_LEN];
	};
	GeneralStruct();
	~GeneralStruct();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds, Int32 data_version);
	void Reset();

	Int16 general_num;
	GeneralItem general_items[MAX_GENERAL_NUM];
};

#endif