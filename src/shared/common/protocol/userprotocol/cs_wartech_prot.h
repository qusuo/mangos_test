#ifndef JXS_WARTECHPROT_H_
#define JXS_WARTECHPROT_H_
//兵法科技协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/battle_def.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//请求兵法科技信息
	struct CSWarTechReqInfo
	{
		CSWarTechReqInfo()
			:header(WARTECH_REQ_INFO_CS)
		{
		}
		prot::ProtHeader header;
	};

	struct WarTechInfo
	{
		WarTechInfo()
			:wartech_id(0)
			, level(0)
		{

		}
		Int16 wartech_id;
		Int16 level;
	};
	struct WarTechSoldierInfo
	{
		WarTechSoldierInfo()
			:soldier_id(0)
			, research_point(0)
		{
			memset(grid_state, 0, sizeof(grid_state));
		}
		Int32 soldier_id;
		Int32 research_point;
		Int16 grid_state[MAX_WARTECH_GRID_NUM];
	};

	//发送全部信息
	struct SCWarTechSendAllInfo
	{
		SCWarTechSendAllInfo()
			:header(WARTECH_SEND_ALL_INFO_SC)
			, tech_num(0)
			, soldier_num(0)
		{
		}
		prot::ProtHeader header;
		Int32 tech_num;
		WarTechInfo tech_infos[MAX_WARTECH_NUM];
		Int32 soldier_num;
		WarTechSoldierInfo soldier_infos[MAX_SOLDIER_WARTECH_NUM];
	};

	//科技信息变更同步
	struct SCWarTechInfoChange
	{
		enum ChangeType
		{
			CHANGE_TYPE_INVALID,
			CHANGE_TYPE_ADD,
			CHANGE_TYPE_UPDATE,
			//CHANGE_TYPE_REMOVE,
		};
		SCWarTechInfoChange()
			:header(WARTECH_TECH_INFO_CHANGE_SC)
		{
		}
		prot::ProtHeader header;
		Int32 change_type;
		WarTechInfo tech_info;
	};

	//士兵兵法信息变更同步
	struct SCWarTechSoldierInfoChange
	{
		enum ChangeType
		{
			CHANGE_TYPE_INVALID,
			CHANGE_TYPE_ADD,
			CHANGE_TYPE_UPDATE,
			//CHANGE_TYPE_REMOVE,
		};
		SCWarTechSoldierInfoChange()
			:header(WARTECH_SOLDIER_INFO_CHANGE_SC)
		{
		}
		prot::ProtHeader header;
		Int32 change_type;
		WarTechSoldierInfo soldier_info;
	};

	//升级兵法科技
	struct CSWarTechUpgrade
	{
		CSWarTechUpgrade()
			:header(WARTECH_UPGRADE_TECH_CS)
			, wartech_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 wartech_id;
	};

	//装备科技
	struct CSWarTechPutOn
	{
		CSWarTechPutOn()
			:header(WARTECH_PUTON_TECH_CS)
			, soldier_id(0)
			, grid_pos(0)
			, wartech_id(0)
		{
		}
		prot::ProtHeader header;
		Int32 soldier_id;
		Int16 grid_pos;
		Int16 wartech_id;
	};

	//卸下科技
	struct CSWarTechTakeOff
	{
		CSWarTechTakeOff()
			:header(WARTECH_TAKEOFF_TECH_CS)
			, soldier_id(0)
			, grid_pos(0)
		{
		}
		prot::ProtHeader header;
		Int32 soldier_id;
		Int32 grid_pos;
	};

	//解锁格子
	struct CSWarTechUnlockGrid
	{
		CSWarTechUnlockGrid()
			:header(WARTECH_UNLOCK_GRID_CS)
			, soldier_id(0)
			, grid_pos(0)
		{
		}
		prot::ProtHeader header;
		Int32 soldier_id;
		Int32 grid_pos;
	};

	//改造士兵
	struct CSWarTechReformSoldier
	{
		CSWarTechReformSoldier()
			:header(WARTECH_REFORM_SOLDIER_CS)
			, soldier_id(0)
			, num(0)
		{
		}
		prot::ProtHeader header;
		Int32 soldier_id;
		Int32 num;
	};

	//改造士兵返回
	struct SCWarTechReformSoldierRet
	{
		enum ResCode
		{
			RESCODE_FAIL = -1,
			RESCODE_SUCCEED = 0,
		};
		SCWarTechReformSoldierRet()
			:header(WARTECH_REFORM_SOLDIER_RET_SC)
			, res_code(K_EXCEPTION)
		{
		}
		prot::ProtHeader header;
		Int32 res_code;
	};
}
#pragma pack(pop)

#endif
