#ifndef JXS_PROCESSORPROT_H_
#define JXS_PROCESSORPROT_H_
//游戏进度协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../game/structs/item_data.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	struct ProcessorHeader
	{
		ProcessorHeader()
			:proc_type(0)
			, proc_id(0)
			, time_len(0)
			, left_time_len(0)
		{

		}
		Int16 proc_type;
		Int64 proc_id;
		Int32 time_len;
		Int32 left_time_len;
	};

	struct SCProcList
	{
		SCProcList()
			:header(PROC_LIST_SC)
			, proc_num(0)
		{
			memset(proc_list_data, 0, sizeof(proc_list_data));
		}
		prot::ProtHeader header;
		Int32 proc_num;	//进度数量
		char proc_list_data[MAX_1K_BUFFER_LEN];
	};

	enum ProcChangeType
	{
		PROC_CHANGE_TYPE_INVALID,
		PROC_CHANGE_TYPE_ADD,
		PROC_CHANGE_TYPE_UPDATE,
		PROC_CHANGE_TYPE_CANCEL,
		PROC_CHANGE_TYPE_COMPLETE,
	};

	enum ProcQuickType
	{
		PROC_QUICK_TYPE_INVALID,
		PROC_QUICK_TYPE_GOLD,
		PROC_QUICK_TYPE_ITEM,
	};

	struct SCProcChangeNotice
	{
		SCProcChangeNotice()
			:header(PROC_CHANGE_NOTICE_SC)
			, change_type(0)
		{
			memset(proc_data, 0, sizeof(proc_data));
		}
		prot::ProtHeader header;
		Int32 change_type; 
		ProcessorHeader proc_header;
		char proc_data[MAX_128_CHAR_LEN];
	};

	struct ProtSoldierRecruit
	{
		ProtSoldierRecruit()
			:building_id(0)
			, recruit_index(0)
			, soldier_id(0)
			, soldier_num(0)
		{

		}
		Int16 building_id;
		Int16 recruit_index;
		Int32 soldier_id;
		Int32 soldier_num;
	};

	//增益道具工坊
	struct ProtItemAdd
	{
		ProtItemAdd()
			: recruit_index(0)
			, item_id(0)
			, item_num(0)
		{

		}
		Int32 recruit_index;
		Int32 item_id;
		Int32 item_num;
	};

	//计略俯计谋研发队列进度
	struct ProtSchemeDevelop
	{
		ProtSchemeDevelop()
			: scheme_id(0)
			, scheme_level(0)
		{

		}
		Int32 scheme_id;
		Int32 scheme_level;
	};

	struct ProtBuildingUpgrade
	{
		ProtBuildingUpgrade()
			:building_id(0)
			, level(0)
			, resource_building_type(0)
		{

		}
		Int16 building_id;				// 当前id
		Int16 level;					// 用户的建筑当前等级
		Int16 resource_building_type;	// 空地上面的矿产等
	};

	//struct ProtMarchArmy
	//{
	//	ProtMarchArmy()
	//		:role_id(0)
	//		, army_pos(0)
	//		, march_type(0)
	//		, target_x(0)
	//		, target_y(0)
	//	{

	//	}
	//	RoleID role_id;		//进攻主角色id  note（hsj)可能还需要名字，等级等信息
	//	Int16 army_pos;		//部队编号
	//	Int8 march_type;	//行军类型
	//	Int16 target_x;		//目标
	//	Int16 target_y;
	//};

	struct ProtSoldierTrain
	{
		ProtSoldierTrain()
			:soldier_id(0)
			, soldier_num(0)
		{

		}
		Int32 soldier_id;
		Int32 soldier_num;
	};

	struct ProtTest
	{
		ProtTest()
			:data_len(0)
		{
			memset(data, 0, sizeof(data));
		}
		Int32 data_len;
		Int32 data[MAX_128_CHAR_LEN];
	};

}
#pragma pack(pop)

#endif
