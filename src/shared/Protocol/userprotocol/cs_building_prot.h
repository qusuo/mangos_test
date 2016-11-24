#ifndef JXS_CSBUILDINGPROT_H_
#define JXS_CSBUILDINGPROT_H_
//建筑相关协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//获取建筑列表
	struct CSBuildingGetList
	{
		CSBuildingGetList()
			:header(BUILDING_GET_BUILDING_LIST_CS)
		{
		}
		prot::ProtHeader header;
	};

	//同步建筑列表
	struct SCBuildingSyncList
	{
		SCBuildingSyncList()
			:header(BUILDING_SYNC_BUILDING_LIST_SC)
			, building_num(0)
		{
			memset(buildings, 0, MAX_BUILDING_NUM);
		}
		prot::ProtHeader header;
		int32 building_num;
		BuildingData buildings[MAX_BUILDING_NUM];
	};

	//升级建筑
	struct CSBuildingUpgrade
	{
		enum UpgradeType
		{
			UPGRADE_TYPE_INVALID,
			UPGRADE_TYPE_RESOURCE,		//资源升级
			UPGRADE_TYPE_GOLD,			//金币升级
			UPGRADE_TYPE_ITEM,			//道具升级
		};
		CSBuildingUpgrade()
			:header(BUILDING_UPGRADE_CS)
			, building_id(0)
			, resource_type(0)
			, upgrade_type(0)
		{

		}
		prot::ProtHeader header;
		int32 building_id;
		Int16 resource_type;	//资源建筑id
		Int16 upgrade_type; // 1 = 消耗资源， 2 = 用金币补足资源
	};

	struct SCBuildingUpgradeRet
	{
		SCBuildingUpgradeRet()
			:header(BUILDING_UPGRADE_RET_SC)
			, res_code(FAIL_RES)
			, building_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code; //0 = 成功， -1=失败	
		int32 building_id; //操作的building_id
	};

	//加速升级
	struct CSBuildingQuickUpgrade
	{
		CSBuildingQuickUpgrade()
			:header(BUILDING_QUICK_UPGRADE_CS)
			, proc_id(0)
			, upgrade_type(0)
			, item_id(0)
			, item_num(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;
		int32 upgrade_type;		//1 = 消耗金币， 2 = 使用道具
		int32 item_id;			//道具加速时使用
		int32 item_num;			//道具加速时使用
	};
		
	struct SCBuildingQuickUpgradeRet
	{
		SCBuildingQuickUpgradeRet()
			:header(BUILDING_QUICK_UPGRADE_RET_SC)
			, res_code(FAIL_RES)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code; //0 = 成功， -1=失败	
		Int64 proc_id;	//升级进程id
	};

	//拆除建筑
	struct CSBuildingDestroy
	{
		CSBuildingDestroy()
			:header(BUILDING_DESTROY_CS)
			, building_id(0)
		{

		}
		prot::ProtHeader header;
		int32 building_id;
	};

	struct SCBuildingDestroyRet
	{
		SCBuildingDestroyRet()
			:header(BUILDING_DESTROY_RET_SC)
			, res_code(FAIL_RES)
			, building_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code; //0 = 成功， -1=失败	
		int32 building_id;
	};

	//建筑信息变更
	struct SCBuildingChange
	{
		enum ChangeType
		{
			CHANGE_TYPE_INVALID,
			
			CHANGE_TYPE_CONSTRUCT,	//创建
			CHANGE_TYPE_UPGRADE,	//升级
			CHANGE_TYPE_DESTROY,	//拆除
			CHANGE_TYPE_OPEN,		//开放

			CHANGE_TYPE_END,
			CHANGE_TYPE_COUNT = CHANGE_TYPE_END - 1,
		};
		SCBuildingChange()
			:header(BUILDING_CHANGE_NOTICE_SC)
			, building_id(0)
			, change_type(0)
			, level(0)
			, resource_building_type(0)
		{

		}
		prot::ProtHeader header;
		Int16 building_id;
		Int16 change_type;					// 1 = 建造 2 = 升级，3 = 拆除
		Int16 level;						
		Int16 resource_building_type;		// 空地上面的矿产等类型,因building_id可能是空地id
	};

	//取消升级建筑
	struct CSBuildingUpgradeCancel
	{
		CSBuildingUpgradeCancel()
			:header(BUILDING_UPGRADE_CANCEL_CS)
			, proc_id(0)
		{
		}
		prot::ProtHeader header;
		Int64 proc_id;
	};

	struct SCBuildingUpgradeCancelRet
	{
		SCBuildingUpgradeCancelRet()
			:header(BUILDING_UPGRADE_CANCEL_RET_SC)
			, res_code(FAIL_RES)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code; //0 = 成功， -1=失败	
		Int64 proc_id; //操作的进程id
	};
}
#pragma pack(pop)

#endif
