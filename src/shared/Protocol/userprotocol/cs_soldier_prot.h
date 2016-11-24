#ifndef JXS_CSSOLDIERPROT_H_
#define JXS_CSSOLDIERPROT_H_
//士兵协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"
#include "../../game/structs/army_format_data.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//士兵列表
	struct SCSoldierInfo
	{
		SCSoldierInfo()
			:header(SOLDIER_SOLDIER_INFO_SC)
			, inner_soldier_id_num(0)
			, injured_soldier_id_num(0)
		{
		}
		prot::ProtHeader header;
		int32 inner_soldier_id_num;	
		SoldierUnit inner_soldier_units[MAX_SOLDIER_UNIT_NUM];
		int32 injured_soldier_id_num;
		SoldierUnit injured_soldier_units[MAX_SOLDIER_UNIT_NUM];
	};

	//请求治疗士兵
	struct CSSoldierCure
	{
		CSSoldierCure()
			:header(SOLDIER_CURE_CS)
			, soldier_id(0)
			, is_use_gold(0)
		{

		}
		prot::ProtHeader header;
		int32 soldier_id;
		int32 is_use_gold;
	};

	//请求招募士兵
	struct CSSoldierRecruit
	{
		CSSoldierRecruit()
			:header(SOLDIER_RECRUIT_CS)
			, building_id(0)
			, recruit_index(0)
			, recruit_num(0)
		{

		}
		prot::ProtHeader header;
		Int16 building_id;
		Int16 recruit_index;
		int32 recruit_num;
	};

	//取消招募
	struct CSSoldierCancelRecruit
	{
		CSSoldierCancelRecruit()
			:header(SOLDIER_CANCEL_RECRUIT_CS)
			, processor_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 processor_id;
	};

	//加速招募
	struct CSSoldierQuikenRecruit
	{
		CSSoldierQuikenRecruit()
			:header(SOLDIER_QUICKEN_RECRUIT_CS)
			, processor_id(0)
			, upgrade_type(0)
			, item_id(0)
			, item_num(0)
		{

		}
		prot::ProtHeader header;
		Int64 processor_id;
		int32 upgrade_type;		//1 = 消耗金币， 2 = 使用道具
		int32 item_id;
		int32 item_num;
	};

	//改装士兵
	struct CSSoldierTrain
	{
		CSSoldierTrain()
			:header(SOLDIER_TRAIN_CS)
			, soldier_id(0)
			, train_num(0)
		{

		}
		prot::ProtHeader header;
		int32 soldier_id;
		int32 train_num;
	};

	//取消改装
	struct CSSoldierCancelTrain
	{
		CSSoldierCancelTrain()
			:header(SOLDIER_CANCEL_TRAIN_CS)
			, processor_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 processor_id;
	};

	//加速改装
	struct CSSoldierQuikenTrain
	{
		CSSoldierQuikenTrain()
			:header(SOLDIER_QUICKEN_TRAIN_CS)
			, processor_id(0)
			, upgrade_type(0)
			, item_id(0)
			, item_num(0)
		{

		}
		prot::ProtHeader header;
		Int64 processor_id;
		int32 upgrade_type;		//1 = 消耗金币， 2 = 使用道具
		int32 item_id;
		int32 item_num;
	};
}
#pragma pack(pop)

#endif
