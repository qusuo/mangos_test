#ifndef JXS_ROLEEQUIPPROT_H_
#define JXS_ROLEEQUIPPROT_H_
//角色装备协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/battle_def.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
//请求装备信息
struct CSRoleEquipReqInfo
{
	CSRoleEquipReqInfo()
		:header(ROLEEQUIP_REQ_INFO_CS)
	{
	}
	prot::ProtHeader header;
};

struct EquipInfo
{
	EquipInfo()
		:unit_pos(0)
		, equip_type(0)
		, level(0)
		, id(0)	
		, exp(0)
		, dead_line(0)
	{

	}

	Int8 unit_pos;
	Int8 equip_type;
	Int16 level;
	int32 id;
	int32 exp;
	Int64 dead_line;
};

//发送所有装备信息
struct SCRoleEquipSendAllInfo
{
	SCRoleEquipSendAllInfo()
		:header(ROLEEQUIP_SEND_ALL_INFO_SC)
		, equip_num(0)
	{
	}
	prot::ProtHeader header;
	int32 equip_num;
	EquipInfo unit_equip_infos[MAX_ROLE_EQUIP_NUM];
};

//同步单个装备信息变更
struct SCRoleEquipInfoChange
{
	enum ChangeType
	{
		CHANGE_TYPE_INVALID,
		CHANGE_TYPE_ADD,
		CHANGE_TYPE_UPDATE,
		CHANGE_TYPE_REMOVE,
	};
	SCRoleEquipInfoChange()
		:header(ROLEEQUIP_EQUIP_INFO_CHANGE_SC)
		, change_type(0)
	{
	}
	prot::ProtHeader header;
	int32 change_type;
	EquipInfo equip_info;
};

//穿上或更换装备
struct CSRoleEquipPutOnEquip
{
	enum SrcType
	{
		SRC_TYPE_INALID,
		SRC_TYPE_EQUIP,		//已装备的
		SRC_TYPE_KNAP,		//背包里的
	};
	CSRoleEquipPutOnEquip()
		:header(ROLEEQUIP_PUTON_EQUIP_CS)
		, dest_unit_pos(0)
		, equip_type(0)
		, src_type(0)
		, src_pos(0)
	{
	}
	prot::ProtHeader header;
	Int8 dest_unit_pos;		//目标单元序号
	Int8 equip_type;		
	Int16 src_type;
	int32 src_pos;
};

struct CSRoleEquipTakeOffEquip
{
	CSRoleEquipTakeOffEquip()
		:header(ROLEEQUIP_TAKEOFF_EQUIP_CS)
		, dest_unit_pos(0)
		, dest_equip_type(0)
		, reserved(0)
	{

	}
	prot::ProtHeader header;
	Int8 dest_unit_pos;		//目标单元序号
	Int8 dest_equip_type;
	Int16 reserved;
};

const int32 MAX_KNAP_EQUIP_NUM = 200;
struct CSRoleEquipUpgrade
{
	enum DestType
	{
		DEST_TYPE_INVALID,
		DEST_TYPE_EQUIP,
		DEST_TYPE_KNAP,
	};
	CSRoleEquipUpgrade()
		:header(ROLEEQUIP_UPGRADE_EQUIP_CS)
		, equip_type(0)
		, dest_type(0)
		, dest_pos(0)
		, materail_num(0)
	{
		memset(material_list, 0, sizeof(material_list));
	}
	prot::ProtHeader header;
	Int8 equip_type;
	Int8 dest_type;
	Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
	int32 materail_num;
	int32 material_list[MAX_KNAP_EQUIP_NUM];
};

struct SCRoleEquipUpgradeRet
{
	enum Rescode
	{
		RESCODE_SUCCEED = 0,
		RESCODE_FAIL = -1,
	};
	SCRoleEquipUpgradeRet()
		:header(ROLEEQUIP_UPGRADE_EQUIP_RET_SC)
		, res_code(0)
		, equip_type(0)
		, dest_type(0)
		, dest_pos(0)
	{

	}
	prot::ProtHeader header;
	int32 res_code;
	Int8 equip_type;
	Int8 dest_type;
	Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
};

//交换两个单元的装备
struct CSRoleEquiopTransPosition
{
	CSRoleEquiopTransPosition()
		:header(ROLEEQUIP_TRANSPOSITION_CS)
		, dest_unit_pos(0)
		, src_unit_pos(0)
		, reserved(0)
	{

	}
	prot::ProtHeader header;
	Int8 dest_unit_pos;
	Int8 src_unit_pos;
	Int16 reserved;
};
	
//交换两个单元的装备返回
struct SCRoleEquiopTransPositionRet
{
	enum ResCode
	{
		RESCODE_SUCCEED = 0,
		RESCODE_FAIL = -1,
	};
	SCRoleEquiopTransPositionRet()
		:header(ROLEEQUIP_TRANSPOSITION_RET_SC)
		, res_code(0)
		, dest_unit_pos(0)
		, src_unit_pos(0)
	{

	}
	prot::ProtHeader header;
	Int16 res_code;
	Int8 dest_unit_pos;
	Int8 src_unit_pos;
};
}
#pragma pack(pop)

#endif
