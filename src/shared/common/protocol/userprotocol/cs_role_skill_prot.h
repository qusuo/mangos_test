#ifndef JXS_ROLESKILLPROT_H_
#define JXS_ROLESKILLPROT_H_
//角色技能协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{

//请求技能信息
struct CSRoleSkillReqInfo
{
	CSRoleSkillReqInfo()
		:header(ROLESKILL_REQ_INFO_CS)
	{
	}
	prot::ProtHeader header;
};

//发送技能信息
struct SCRoleSkillSendInfo
{
	SCRoleSkillSendInfo()
		:header(ROLESKILL_SEND_INFO_SC)
	{
		memset(skill_levels, 0, sizeof(skill_levels));
	}
	prot::ProtHeader header;
	Int16 skill_levels[ROLE_SKILL_COUNT];
};

//技能升级
struct CSRoleSkillUpgradeSkill
{
	CSRoleSkillUpgradeSkill()
		:header(ROLESKILL_UPGRADE_SKILL_CS)
		, skill_type(0)
		, is_use_gold(0)
	{

	}
	prot::ProtHeader header;
	Int16 skill_type;
	Int16 is_use_gold; // 1：use:
};

//技能升级返回
struct SCRoleSkillUpgradeSkillRet
{
	SCRoleSkillUpgradeSkillRet()
		:header(ROLESKILL_UPGRADE_SKILL_RET_SC)
		, res_code(K_EXCEPTION)
		, skill_type(0)
		, skill_level(0)
		, reserve(0)
	{

	}
	prot::ProtHeader header;
	Int16 res_code;
	Int16 skill_type;
	Int16 skill_level;
	Int16 reserve;
};


//技能重置,重置后技能等级为0，返还全部技能书
struct CSRoleSkillResetSkill
{
	CSRoleSkillResetSkill()
		:header(ROLESKILL_RESET_SKILL_CS)
	{

	}
	prot::ProtHeader header;
};

//技能重置结果返回
struct SCRoleSkillResetSkillRet
{
	SCRoleSkillResetSkillRet()
		:header(ROLESKILL_RESET_SKILL_RET_SC)
		, res_code(K_EXCEPTION)
		, reserve(0)
		, skill_books(0)
	{

	}
	prot::ProtHeader header;
	Int16 res_code;
	Int16 reserve;
	Int32 skill_books;	//重置后获取的技能书数量
};


}
#pragma pack(pop)

#endif
