#ifndef JXS_RETURNCODE_H_
#define JXS_RETURNCODE_H_



//服务器返回码定义
#include "game_value_def.h"

enum CommentReturnCode
{
	RETURN_CODE_EXCEPTION = -1,
	RETURN_CODE_SUC = 0,
};

//建筑错误码返回
enum ErrorCode
{
	ERROR_CODE_INVALID = 0,
	ERROR_CODE_UPGRADING,			//正在升级中
	ERROR_CODE_ROLE,				//角色信息出错
	ERROR_CODE_RESOURCE,			//资源不足
	ERROR_CODE_PROC_FULL,			//队列已满
	ERROR_CODE_OPEN,				//未开放
	ERROR_CODE_BUILDING_INFO,		//建筑信息不正确，如id不存在等
	ERROR_CODE_NO_LEVEL,			//没有下一级等级信或没有等级信息
	ERROR_CODE_PALACE_LEVEL,		//主城等级条件不满足
	ERROR_CODE_MAX_ROLE_EQUIP_TIMES,//奇门背包格子扩充次数已达最大


	//下面无效
	ERROR_CODE_EQUIPMENT_MANU,		//装备制造所
	ERROR_CODE_MINING_WORKSHOP,		//开采工场
	ERROR_CODE_WAREHOUSE,			//仓库
	ERROR_CODE_TALENTS,				//人才府
	ERROR_CODE_MILITARY,			//军事府
	ERROR_CODE_CORPS,				//军团
	ERROR_CODE_ARSENAL,				//兵器制造所
	ERROR_CODE_STRATEGICS,			//兵法研究所
	ERROR_CODE_ARMED_ORGANIZATION,	//兵种编制所
	ERROR_CODE_TRAINING,				//练兵所
	ERROR_CODE_STATION_TROOPS,		//屯所
	ERROR_CODE_DRILL_GROUND,			//校场
	ERROR_CODE_CORPS_HALL,			//军团大厅
	ERROR_CODE_TECH_HALL,			//科技大厅
	ERROR_CODE_WELFARE_HOUSE,		//福利院
	ERROR_CODE_INFO_STATION,			//情报站
	ERROR_CODE_SHOP,					//军团商店
	ERROR_CODE_OPERATIONS_CENTER,	//作战中心
	ERROR_CODE_MARKET,				//市场
	ERROR_CODE_FARMLAND,				//农田
	ERROR_CODE_LUMBERYARD,			//伐木场
	ERROR_CODE_COPPER_ORE,			//铜矿
	ERROR_CODE_IRON_ORE,				//铁矿
	ERROR_CODE_SILVER_ORE_SPACE,		//银矿空地,特殊空地 
	ERROR_CODE_NORMAL_ORE,			//普通空地
	ERROR_CODE_IRON_ORE_SPACE,		//铁矿空地,特殊空地 
	ERROR_CODE_END,
	ERROR_CODE_COUNT = ERROR_CODE_END - 1,

};

#endif