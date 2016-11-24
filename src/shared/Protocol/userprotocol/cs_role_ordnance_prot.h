#ifndef JXS_ROLEORDNANCEPROT_H_
#define JXS_ROLEORDNANCEPROT_H_
//角色机械协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/battle_def.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	enum PositionType
	{
		DEST_TYPE_INVALID,
		DEST_TYPE_EQUIP,		//已装备的
		DEST_TYPE_KNAP,			//背包里的
	};

	enum ResCode
	{
		RESCODE_FAIL = -1,
		RESCODE_SUCCEED = 0,
	};

	//请求装备信息
	struct CSOrdnanceReqInfo
	{
		CSOrdnanceReqInfo()
			:header(ORDNANCE_REQ_INFO_CS)
		{
		}
		prot::ProtHeader header;
	};

	struct OrdnanceInfo
	{
		OrdnanceInfo()
			:soldier_type(0)
			, ordnance_type(0)
			, stren_level(0)
			, reform_level(0)
			, id(0)
			, dead_line(0)
		{

		}

		Int8  soldier_type;		//士兵类型
		Int8  ordnance_type;	//军械类型
		Int16 stren_level;		//强化等级
		int32 reform_level;		//改造等级
		int32 id;				//道具id
		Int64 dead_line;		//到期时间
	};

	//发送所有军械信息
	struct SCOrdnanceSendAllInfo
	{
		SCOrdnanceSendAllInfo()
			:header(ORDNANCE_SEND_ALL_INFO_SC)
			, ordnance_num(0)
		{
		}
		prot::ProtHeader header;
		int32 ordnance_num;									//
		OrdnanceInfo ordnance_infos[MAX_ROLE_ORDNANCE_NUM];	//
	};

	
	//同步单个军械信息变更
	struct SCOrdnanceInfoChange
	{
		enum ChangeType
		{
			CHANGE_TYPE_INVALID,
			CHANGE_TYPE_ADD,
			CHANGE_TYPE_UPDATE,
			CHANGE_TYPE_REMOVE,
		};
		SCOrdnanceInfoChange()
			:header(ORDNANCE_INFO_CHANGE_SC)
			, change_type(CHANGE_TYPE_INVALID)
		{
		}
		prot::ProtHeader header;
		int32 change_type;				//
		OrdnanceInfo ordnance_info;		//
	};

	//穿上军械
	struct CSOrdnancePutOn
	{
		CSOrdnancePutOn()
			:header(ORDNANCE_PUTON_CS)
			, soldier_type(0)
			, ordnance_type(0)
			, src_pos(-1)
		{
		}
		prot::ProtHeader header;
		Int8 soldier_type;			//士兵类型，1=盾兵 2=骑兵 3=枪兵 4=弓兵
		Int8 ordnance_type;			//军械类型：1=生命 2=攻击 3=防护 4=穿刺 5=生命防护 6=攻击穿刺 7=攻击生命 8=穿刺防护
		Int16 src_pos;				//来源位置：这里填背包格子
	};				

	//穿上军械返回
	struct SCOrdnancePutOnRet
	{
		SCOrdnancePutOnRet()
			:header(ORDNANCE_PUTON_RET_SC)
			, res_code(RESCODE_FAIL)
			, soldier_type(0)
			, ordnance_type(0)
			, src_pos(-1)
		{
		}
		prot::ProtHeader header;
		int32 res_code;				// 
		Int8 soldier_type;			//士兵类型，1=盾兵 2=骑兵 3=枪兵 4=弓兵
		Int8 ordnance_type;			//军械类型：1=生命 2=攻击 3=防护 4=穿刺 5=生命防护 6=攻击穿刺 7=攻击生命 8=穿刺防护
		Int16 src_pos;				//来源位置：这里填背包格子
	};


	//卸下军械
	struct CSOrdnanceTakeOff
	{
		CSOrdnanceTakeOff()
			:header(ORDNANCE_TAKEOFF_CS)
			, soldier_type(0)
			, ordnance_type(0)
			, reserved(0)
		{

		}
		prot::ProtHeader header;
		Int8 soldier_type;		//
		Int8 ordnance_type;		//
		Int16 reserved;
	};
			
	//卸下军械返回
	struct SCOrdnanceTakeOffRet
	{
		SCOrdnanceTakeOffRet()
			:header(ORDNANCE_TAKEOFF_RET_SC)
			, res_code(RESCODE_FAIL)
			, soldier_type(0)
			, ordnance_type(0)
			, reserved(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code;			//
		Int8 soldier_type;		//
		Int8 ordnance_type;		//
		Int16 reserved;
	};

	//强化军械
	struct CSOrdnanceStrengthen
	{
		CSOrdnanceStrengthen()
			:header(ORDNANCE_STRENGTHEN_CS)
			, soldier_type(0)
			, dest_type(0)
			, dest_pos(-1)
			, item_num(0)
		{
		}
		prot::ProtHeader header;
		Int8 soldier_type;	//
		Int8 dest_type;		//来源类型：1=已装上的军械，2=背包中的军械
		Int16 dest_pos;		//如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
		int32 item_num;		//增加机率物品数量
	};

	//强化军械结果返回
	struct SCOrdnanceStrengthenRet
	{
		SCOrdnanceStrengthenRet()
			:header(ORDNANCE_STRENGTHEN_RET_SC)
			, res_code(RESCODE_FAIL)
			, soldier_type(0)
			, dest_type(0)
			, dest_pos(-1)
		{
		}
		prot::ProtHeader header;
		int32 res_code;		//
		Int8 soldier_type;	//
		Int8 dest_type;		//
		Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
	};


	//改造军械
	struct CSOrdnanceReform
	{
		CSOrdnanceReform()
			:header(ORDNANCE_REFORM_CS)
			, soldier_type(0)
			, dest_type(0)
			, dest_pos(0)
			, item_num(0)
		{
		}
		prot::ProtHeader header;
		Int8 soldier_type;//
		Int8 dest_type;	  //
		Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
		int32 item_num;		//
	};
		
	//改造军械返回
	struct SCOrdnanceReformRet
	{
		SCOrdnanceReformRet()
			:header(ORDNANCE_REFORM_RET_SC)
			, res_code(RESCODE_FAIL)
			, soldier_type(0)
			, dest_type(0)
			, dest_pos(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;		//
		Int8 soldier_type;	//
		Int8 dest_type;		//
		Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
	};

	//进阶军械
	struct CSOrdnanceAdvance
	{
		CSOrdnanceAdvance()
			:header(ORDNANCE_ADVANCE_CS)
			, soldier_type(0)
			, dest_type(0)
			, dest_pos(0)
		{
		}
		prot::ProtHeader header;
		Int8 soldier_type;//
		Int8 dest_type;	  //
		Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
	};

	//进阶军械结果返回
	struct CSOrdnanceAdvanceRet
	{
		CSOrdnanceAdvanceRet()
			:header(ORDNANCE_ADVANCE_RET_SC)
			, res_code(RESCODE_FAIL)
			, soldier_type(0)
			, dest_type(0)
			, dest_pos(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;		//
		Int8 soldier_type;	//
		Int8 dest_type;		//
		Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
	};

	//分解军械
	struct CSOrdnanceBreakdown
	{
		CSOrdnanceBreakdown()
			:header(ORDNANCE_BREAK_DOWN_CS)
			, grid_num(0)
		{
			memset(dest_pos, 0, sizeof(dest_pos));
			memset(dest_num, 0, sizeof(dest_num));
		}

		prot::ProtHeader header;
		int32 grid_num;								//共有多少个格子
		int32 dest_pos[TOTAL_ORDNANCE_GRID_NUM];	//只能是背包的，表示哪个格子 从0开始
		int32 dest_num[TOTAL_ORDNANCE_GRID_NUM];	//每个格子的数量
	};

	//分解军械结果返回
	struct CSOrdnanceBreakdownRet
	{
		CSOrdnanceBreakdownRet()
			:header(ORDNANCE_BREAK_DOWN_RET_SC)
			, ordnace_num(0)
		{
			memset(res_code, 0, sizeof(res_code));
			memset(dest_pos, 0, sizeof(dest_pos));
		}
		prot::ProtHeader header;
		int32 ordnace_num;
		int32 res_code[TOTAL_ORDNANCE_GRID_NUM];	//
		int32 dest_pos[TOTAL_ORDNANCE_GRID_NUM];	//只能是背包的，表示哪个格子 从0开始
	};

	//合成军械
	struct CSOrdnanceSynthesis
	{
		CSOrdnanceSynthesis()
			:header(ORDNANCE_SYNTHESIS_CS)
			, dest_pos(-1)
		{
		}

		prot::ProtHeader header;
		int32 dest_pos;		//表示哪个背包格子 从0开始		
	};

	//合成军械结果返回
	struct CSOrdnanceSynthesisRet
	{
		CSOrdnanceSynthesisRet()
			:header(ORDNANCE_SYNTHESIS_RET_SC)
			, res_code(RESCODE_FAIL)
			, dest_pos(-1)
			, synthesis_num(0)
			, master_synthesis_num(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;					//
		int32 dest_pos;					//背包格子索引
		Int16 synthesis_num;			//使用碎片数量
		Int16 master_synthesis_num;		//使用万能碎片数量
	};

}
#pragma pack(pop)

#endif
