﻿#ifndef JXS_TASKDEF_H_
#define JXS_TASKDEF_H_
//任务类型定义
#include "game_value_def.h"

//任务类型
enum TaskType
{
	TASK_TYPE_INVALID,

	TASK_TYPE_MAINLINE,			//主线任务
	TASK_TYPE_DAILY,			//日常任务
	TASK_TYPE_ACTIVE,			//活跃任务
	TASK_TYPE_ACHIEVEMENT,		//成就任务
	TASK_TYPE_FEEDER,			//支线任务

	TASK_TYPE_END,
};

//主线任务类型
enum MainlineTaskType
{
	MAINLINE_TASK_TYPE_INVALID,

	TASK_SUB_TYPE_BUILDING,		//基地建设
	TASK_SUB_TYPE_ROLE,			//角色人物
	TASK_SUB_TYPE_RESOURCE,		//资源产量
	TASK_SUB_TYPE_BATTLE,		//战斗通关
	TASK_SUB_TYPE_GENERAL,		//将领培养
	TASK_SUB_TYPE_WORLD,		//世界争霸
	TASK_SUB_TYPE_POWER,		//战力提升
	TASK_SUB_TYPE_ALLIANCE,		//军团交互
	
	MAINLINE_TASK_TYPE_END,

};

//成就任务类型
enum AchievementTaskType
{
	ACHIEVEMENT_TASK_TYPE_INVALID,
	ACHIEVEMENT_TASK_TYPE_ROLE,
	ACHIEVEMENT_TASK_TYPE_ALLIANCE,
	ACHIEVEMENT_TASK_TYPE_OTHER,

	ACHIEVEMENT_TASK_TYPE_END,
};

//子任务类型（任务内容）
enum TaskUnitType
{
	TASK_UNIT_TYPE_INVALID,

	TASK_UNIT_TYPE_BUILDING,	//基地建设
	TASK_UNIT_TYPE_ROLE,		//角色人物
	TASK_UNIT_TYPE_YIELDS,		//资源产量
	TASK_UNIT_TYPE_BATTLE,		//战斗通关
	TASK_UNIT_TYPE_GENERAL,		//将领培养
	TASK_UNIT_TYPE_WORLD,		//世界争霸
	TASK_UNIT_TYPE_POWER,		//战力提升
	TASK_UNIT_TYPE_ALLIANCE,	//军团交互

	TASK_UNIT_TYPE_END,
	TASK_UNIT_TYPE_COUNT = TASK_UNIT_TYPE_END -1,
};

enum TaskState
{
	TASK_STATE_INVALID,
	TASK_STATE_ACTIVATED,		//已激活
	TASK_STATE_ACCEPTED,		//已接
	TASK_STATE_COMPLETED,		//已完成
	TASK_STATE_FINISHED,		//已提交

	TASK_STATE_END,
};

//目标类型
enum TaskTargetType
{
	TASK_TARGET_TYPE_INVALID,

	TASK_TARGET_TYPE_BUILDING,			//建筑建造和升级
	TASK_TARGET_TYPE_RECRUIT_SOLID,		//招募士兵
	TASK_TARGET_TYPE_REPUTATION_LEVEL,	//声望等级
	TASK_TARGET_TYPE_MILI_RANK,			//官职等级

	TASK_TARGET_TYPE_ORE,				//世界矿点
	TASK_TARGET_TYPE_BATTLE,			//战役关卡
	TASK_TARGET_TYPE_RESOURCE_YIELDS,	//资源产量
	TASK_TARGET_TYPE_ROLE_ATTR,			//角色属性

	TASK_TARGET_TYPE_FC,				//战力
	TASK_TARGET_TYPE_BOOM,				//繁荣度
	TASK_TARGET_TYPE_SKILL,				//角色技能
	TASK_TARGET_TYPE_GENERAL,			//将领相关

	TASK_TARGET_TYPE_GENERAL_EQUIP,		//将领装备
	TASK_TARGET_TYPE_ALLIANCE,			//军团相关
	TASK_TARGET_TYPE_TIME,				//定时活动
	TASK_TARGET_TYPE_WAR,				//兵法相关

	TASK_TARGET_TYPE_NAVIGATE,			//其他引导
	TASK_TARGET_TYPE_ROLE_EQUIP,		//奇门

	TASK_TARGET_TYPE_END,
};

//2角色属性任务类型
enum TaskBuildingType
{
	TASK_BUILDING_CONSTRUCT = 1,		//建筑创建
	TASK_BUILDING_UPDATE,				//建筑升级

};


//2角色属性任务类型
enum TaskRoleAttrType
{
	TASK_ROLE_ATTR_REPU_ADD = 25,		//声望捐献次数
	TASK_ROLE_ATTR_LEADERSHIP_ADD,		//统率提升次数
	TASK_ROLE_ATTR_QUICK_BUILDING,		//加速建筑升级次数
	TASK_ROLE_ATTR_SOLDIER_RECRUIT,		//加速士兵招募次数
	TASK_ROLE_ATTR_SOLDIER_TRAIN,		//加速士兵训练次数
	TASK_ROLE_ATTR_FRIENDS,				//添加好友个数
	TASK_ROLE_ATTR_BLESSING,			//祝福好友次数
};

//4战役关卡任务类型
enum TaskBattleType
{
	TASK_BATTLE_TYPE_INVALID,
	TASK_BATTLE_TYPE_PASS,				//通关战役
	TASK_BATTLE_TYPE_BATTLE_TIMES,		//关卡扫荡次数
	TASK_BATTLE_TYPE_ARENA_TIMES,		//竞技场挑战次数
	TASK_BATTLE_TYPE_WORLD_TIME,		//挑战世界boss次数
	TASK_BATTLE_TYPE_EXTREME,			//通关极限探险

};

//5将领培养任务类型
enum TaskGeneralType
{
	TASK_GENERAL_TYPE_INVALID,
	TASK_GENERAL_TYPE_RECRUIT,			//招募将领
	TASK_GENERAL_TYPE_EQUIP_PUTON,		//将领穿上整套装备
	TASK_GENERAL_TYPE_EQUIP_COMPOSE,	//装备合成
	TASK_GENERAL_TYPE_UPGRADE,			//武将升级
	TASK_GENERAL_TYPE_ADVANCED,			//武将升阶
	TASK_GENERAL_TYPE_COMPOSE,			//武将合成
	TASK_GENERAL_TYPE_EXP,				//使用武将经验卡
};

//6世界争霸任务类型
enum TaskWorldType
{
	TASK_WORLD_TYPE_GATHER = 1,		//下矿一次
	TASK_WORLD_TYPE_GATHER_NUM,		//世界采集资源量
	TASK_WORLD_TYPE_GATHER_ATTACK,	//撞矿
	TASK_WORLD_TYPE_CITY_ATTACK,	//掠夺玩家城池
};

//7战力提升任务类型
enum TaskPowerType
{
	TASK_POWER_TYPE_SOLDIER_NUM = 1,	//士兵总数 
	TASK_POWER_TYPE_SKILL_LEVEL,		//技能等级
	TASK_POWER_TYPE_EQUIP,				//装备奇门个数
	TASK_POWER_TYPE_EQUIP_SUIT,			//激活奇门套装
	TASK_POWER_TYPE_ART_OF_WAR,			//提升兵法
	TASK_POWER_TYPE_SCHEME,				//计略技能提升
};

//8军团任务类型
enum TaskAllianceType
{
	TASK_ALLIANCE_TYPE_BOSS = 1,	//参加1次军团boss战
	TASK_ALLIANCE_TYPE_DOGFIGHT,	//参加1次军团百团混战
	TASK_ALLIANCE_TYPE_JOIN,		//加入1个军团
	TASK_ALLIANCE_TYPE_DONATE,		//军团捐献次次数
	TASK_ALLIANCE_TYPE_CHAPTER,		//挑战一次军团副本
};


#endif
