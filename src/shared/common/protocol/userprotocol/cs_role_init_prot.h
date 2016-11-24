#ifndef JXS_CSROLEINITPROTOCOL_H_
#define JXS_CSROLEINITPROTOCOL_H_
//进入游戏时初始化协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"
#include "../../game/include/battle_def.h"
#include "../../game/structs/army_format_data.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	struct CSEnterGame
	{
		CSEnterGame()
			:header(ROLE_ENTER_GAME_CS)
			, role_id(0)
			, net_id(0)
		{
		}
		prot::ProtHeader header;
		Int64 role_id;
		Int32 net_id;
	};

	struct SCEnterGameRet
	{
		enum ProtEnterGameResult
		{
			PROT_ENTER_GAME_INVALIE,
			PROT_ENTER_GAME_SUCCESS,
			PROT_ENTER_GAME_FAIL,
		};

		SCEnterGameRet()
			:header(ROLE_ENTER_GAME_RET_SC)
			, res_code(0)
			, open_server_time(0)
			, server_time(0)
			, role_id(0)
			, gender(0)
			, avatar(0)
			, hat(0)
			, reserved(0)
			//, buy_reputation(0)
			, level(0)
			, exp(0)
			, energy(0)
			, cur_energy(0)
			, reputation(0)
			, reputation_level(0)
			, vip_level(0)
			, boom(0)
			, cur_boom(0)
			, leadership(0)
			, military_rank(0)
			, gold(0)
			, total_charge_gold(0)
			, alliance_id(0)
			, cf(0)
			, last_calc_resoure_time(0)
			, resource_protect(0)
		{
			role_name[0] = '\0';
			alliance_name[0] = '\0';
			memset(resource, 0, sizeof(resource));
			memset(resource_yields, 0, sizeof(resource_yields));
			memset(resource_store, 0, sizeof(resource_store));
			memset(attr_add_list, 0, sizeof(attr_add_list));
		}
		prot::ProtHeader header;
		Int32 res_code; //1 = 成功, 2 = 失败
		Int64 open_server_time;		//开服时间
		Int64 server_time;			//服务器时间
		Int64 role_id;

		RoleName role_name;			//名称
		Int8 gender;				//性别
		Int8 avatar;				//头像
		Int16 hat;					//头像挂件

		Int16 reserved;
		//Int8 buy_reputation;		//购买声望次数
		Int16 level;				//等级
		Int32 exp;					//经验

		Int16 energy;				//最大军令
		Int16 cur_energy;			//当前军令
		Int32 reputation;			//声望
		Int16 reputation_level;		//声望等级
		Int16 vip_level;			//vip等级

		Int32 boom;					//繁荣度
		Int32 cur_boom;				//当前繁荣度
		Int16 leadership;			//统率等级
		Int16 military_rank;		//军衔

		Int32 gold;								//当前金币
		Int32 total_charge_gold;				//总充值金币
		Int64 alliance_id;						//公会id
		char alliance_name[MAX_32_CHAR_LEN];	//公会名字

		Int64 last_calc_resoure_time;					//上一次更新资源时间
		Int32 resource[RESOURCE_TYPE_COUNT];			//当前资源
		Int32 resource_yields[RESOURCE_TYPE_COUNT];		//资源产量
		Int32 resource_store[RESOURCE_TYPE_COUNT];		//资源存储
		Int32 cf;										//战力
		Int32 attr_add_list[ROLE_ATTR_ADD_COUNT];
		Int32 resource_protect;		//各资源保护量
	};


	static const int DAY_COUNT_RECORDER_ID_COUNT = 128;//与daycountrecorder.h中定义的MAX_DAYCOUNT_COUNT_ID一致 ltz
	struct SCDayCountInfo
	{
		SCDayCountInfo()
			:header(ROLE_DAYCOUNT_INFO_SC)
		{
			memset(day_count_data, 0, sizeof(day_count_data));
		}
		prot::ProtHeader	header;
		Int16 day_count_data[DAY_COUNT_RECORDER_ID_COUNT];
	};

	struct SCDayCountInfoChangedNotice
	{
		SCDayCountInfoChangedNotice()
			:header(ROLE_DAYCOUNT_CHANGE_SC)
			, day_count_id(0)
			, day_count_data(0)
		{
		}
		prot::ProtHeader header;
		Int16		day_count_id;
		Int16		day_count_data;
	};

	enum ProtRoleAttr
	{
		ROLE_ATTR_INVALID,
		ROLE_ATTR_AVATAR,			//头像
		ROLE_ATTR_HAT,				//头像挂件
		ROLE_ATTR_LEVEL,			//等级
		ROLE_ATTR_EXP,				//经验
		ROLE_ATTR_CUR_ENERGY,		//当前军令
		ROLE_ATTR_REPUTATION,		//声望
		ROLE_ATTR_REPUTATION_LEVEL,	//声望等级
		ROLE_ATTR_BOOM,				//繁荣度
		ROLE_ATTR_CUR_BOOM,			//当前繁荣度
		ROLE_ATTR_LEADERSHIP,		//统率等级
		ROLE_ATTR_MILITARY_RANK,	//军衔
		ROLE_ATTR_GOLD,				//元宝
		ROLE_ATTR_TAEL,				//银两
		ROLE_ATTR_FORAGE,			//粮草
		ROLE_ATTR_WOOD,				//木头
		ROLE_ATTR_COPPER,			//铜锭
		ROLE_ATTR_INGOT,			//铁锭
		ROLE_ATTR_NAME,				//名称

		// yields
		ROLE_ATTR_YIELDS_TAEL,		//银两产量
		ROLE_ATTR_YIELDS_FORAGE,	//粮草产量
		ROLE_ATTR_YIELDS_WOOD,		//木头产量
		ROLE_ATTR_YIELDS_COPPER,	//铜锭产量
		ROLE_ATTR_YIELDS_INGOT,		//铁锭产量

		// store
		ROLE_ATTR_STORE_TAEL,		//银两存储
		ROLE_ATTR_STORE_FORAGE,		//粮草存储
		ROLE_ATTR_STORE_WOOD,		//木头存储
		ROLE_ATTR_STORE_COPPER,		//铜锭存储
		ROLE_ATTR_STORE_INGOT,		//铁锭存储

		ROLE_ATTR_DAY_COUNT_ENERGY,	//当天购买军令次数
		ROLE_ATTR_BOOM_STATUS,		//繁荣状态, 1=正常，2=废墟
		ROLE_ATTR_VIP_LEVEL,		//VIP等级
		ROLE_ATTR_GENDER,			//性别
		ROLE_ATTR_ALLIANCE_ID,		//军团id

		ROLE_ATTR_ALLIANCE_NAME,	//军团
		ROLE_ATTR_ACCOUNT,			//账号
		ROLE_ATTR_NET_ID,			//网络id
		ROLE_ATTR_RESOURCE_PROTECT,	//各资源保护量

		ROLE_ATTR_END,
	};

	struct SCRoleAttrChange
	{
		SCRoleAttrChange()
			:header(ROLE_ATTR_CHANGE_NOTICE_SC)
			,attr_type(0)
			, attr_value(0)
		{

		}
		prot::ProtHeader header;
		Int32 attr_type;        // enum ProtRoleAttr
		Int32 attr_value;		// 改变之后用户的属性值
	};

	
	//获取城防阵型
	struct CSRoleGetDefendFormat
	{
		CSRoleGetDefendFormat()
			:header(ROLE_GET_DEFEND_FORMAT_INFO_CS)
		{

		}
		prot::ProtHeader header;
	};

	//发送城防阵型
	struct SCRoleSendDefendFormat
	{
		SCRoleSendDefendFormat()
			:header(ROLE_SEND_DEFEND_FORMAT_INFO_SC)
		{
			memset(&army_format, 0, sizeof(army_format));
		}
		prot::ProtHeader header;
		ArmyFormatData army_format;
	};

	//设置城防阵型
	struct CSRoleSetDefendFormat
	{
		CSRoleSetDefendFormat()
			:header(ROLE_SET_DEFEND_FORMAT_INFO_CS)
		{
			memset(&army_format, 0, sizeof(army_format));
		}
		prot::ProtHeader header;
		ArmyFormatData army_format;
	};

	struct CSRoleGmCmd
	{
		CSRoleGmCmd()
			:header(ROLE_GMCMD_CS)
			, cmd_id(0)
			, param1(0)
			, param2(0)
		{
		}
		prot::ProtHeader header;
		Int32 cmd_id;
		Int32 param1;
		Int32 param2;
	};

	//升级统率等级,+1
	struct CSRoleUpgradeLeader
	{
		CSRoleUpgradeLeader()
			:header(ROLE_ATTR_LEADERSHIP_UPGRADE_CS)
		{
		}
		prot::ProtHeader header;
	};

	//升级统率等级返回
	struct SCRoleUpgradeLeader
	{
		SCRoleUpgradeLeader()
			:header(ROLE_ATTR_LEADERSHIP_UPGRADE_RET_SC)
			, res_code(0)
			, reserve(0)
		{
		}
		prot::ProtHeader header;
		Int16 res_code;			//0 = 成功， -1=失败	
		Int16 reserve;
	};
	
	//购买声望值
	struct CSRoleBuyReputation
	{
		CSRoleBuyReputation()
			:header(ROLE_ATTR_BUY_REPU_CS)
			, buy_type(0)
		{
		}
		prot::ProtHeader header;
		Int32 buy_type;				//购买类型1=增俸,2=宴会,3=褒奖,4=宝物
	};
	


	//购买声望值返回
	struct SCRoleBuyReputationRet
	{
		SCRoleBuyReputationRet()
			:header(ROLE_ATTR_BUY_REPU_RET_SC)
			, res_code(0)
			, reserve(0)
		{
		}
		prot::ProtHeader header;
		Int16 res_code;			//0 = 成功， -1=失败	
		Int16 reserve;
	};

	//升级官职
	struct CSRoleMiliRankUpgrade
	{
		CSRoleMiliRankUpgrade()
			:header(ROLE_ATTR_MILI_RANK_UPGRADE_CS)
		{
		}
		prot::ProtHeader header;
	};

	//升级官职返回
	struct SCRoleMiliRankUpgradeRet
	{
		SCRoleMiliRankUpgradeRet()
			:header(ROLE_ATTR_MILI_RANK_UPGRADE_RET_SC)
			, res_code(0)
			, reserve(0)
		{
		}
		prot::ProtHeader header;
		Int16 res_code;			//0 = 成功， -1=失败	
		Int16 reserve;
	};
	
	struct SCRoleInitEnd
	{
		SCRoleInitEnd()
			:header(ROLE_INIT_END_SC)
		{

		}	
		prot::ProtHeader header;
	};

	struct SCRoleAntiAddictionInfo
	{
		SCRoleAntiAddictionInfo()
			:header(ROLE_ANTI_ADDICTION_INFO_SC)
			, online_time(0)
			, anti_addiction(0)
		{

		}
		prot::ProtHeader header;
		Int32 online_time;		//已在线时长
		Int32 anti_addiction;	//是否通过防沉迷
	};

	struct SCRoleErrorCode
	{
		SCRoleErrorCode()
			:header(ROLE_ERROR_CODE_SC)
			, error_code(-1)
		{

		}
		prot::ProtHeader header;
		Int32 error_code;
	};

	//同步新手步骤
	struct SCRoleSyncCondectStep
	{
		SCRoleSyncCondectStep()
			:header(ROLE_SYNC_CONDUCT_STEP_SC)
			, step(0)
			, sub_step(0)
			, param(0)
		{

		}
		prot::ProtHeader header;
		Int32 step;
		Int32 sub_step;
		Int32 param;
	};

	//设置新手步骤
	struct CSRoleSetCondectStep
	{
		CSRoleSetCondectStep()
			:header(ROLE_SET_CONDUCT_STEP_CS)
			, step(0)
			, sub_step(0)
			, param(0)
		{

		}
		prot::ProtHeader header;
		Int32 step;
		Int32 sub_step;
		Int32 param;
	};

	//同步新手步骤
	struct SCRoleSyncCF
	{
		SCRoleSyncCF()
			:header(ROLE_SYNC_CF_SC)
			, cf(0)
		{

		}
		prot::ProtHeader header;
		Int32 cf;
	};

	//上次军令恢复时间
	struct SCEnergyRestoreTime
	{
		SCEnergyRestoreTime()
			:header(ROLE_ENERGY_RESTORE_TIME_SC)
			, restore_time(0)
		{

		}
		prot::ProtHeader header;
		Int64 restore_time;
	};
	

	//新手改名
	struct CSRoleConductChangeRoleName
	{
		CSRoleConductChangeRoleName()
			:header(ROLE_CONDUCT_CHANGE_ROLENAME_CS)
			, gender(0)
		{
			memset(role_name, 0, sizeof(role_name));
		}
		prot::ProtHeader header;
		RoleName role_name;	//角色名
		Int32 gender;		//性别
	};

	//新手改名返回
	struct SCRoleConductChangeRoleNameRet
	{
		enum Rescode
		{
			RESCODE_SUCC = 0,
			RESCODE_EXIST = -1, 
			RESCODE_EXCEPTION = -2,
		};
		SCRoleConductChangeRoleNameRet()
			:header(ROLE_CONDUCT_CHANGE_ROLENAME_RET_SC)
			, res_code(0)
		{

		}
		prot::ProtHeader header;
		Int32 res_code;
	};

	//请求套用阵型信息
	struct CSRoleReqSaveFormat
	{
		CSRoleReqSaveFormat()
			:header(ROLE_REQ_SAVE_FORMAT_CS)
		{
		}
		prot::ProtHeader header;
	};

	//返回套用阵型信息
	struct SCRoleReqSaveFormatRet
	{
		SCRoleReqSaveFormatRet()
			:header(ROLE_REQ_SQVE_FORMAT_RET_SC)
			, format_num(0)
		{
		}
		prot::ProtHeader header;
		Int32 format_num;
		ArmyFormatData formats[MAX_SAVE_FORMAT_NUM];
	};

	//设置套用阵型信息
	struct CSRoleSetSaveFormat
	{
		CSRoleSetSaveFormat()
			:header(ROLE_SET_SAVE_FORMAT_CS)
			, pos(0)
		{
		}
		prot::ProtHeader header;
		Int32 pos;
		ArmyFormatData format_data;
	};

	//设置套用阵型信息返回
	struct SCRoleSetSaveFormatRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_FAIL = -1,
		};
		SCRoleSetSaveFormatRet()
			:header(ROLE_SET_SAVE_FORMAT_RET_SC)
			, res_code(RESCODE_FAIL)
		{
		}
		prot::ProtHeader header;
		Int32 res_code;
	};

}
#pragma pack(pop)

#endif
