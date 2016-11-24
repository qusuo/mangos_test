#ifndef JXS_LOGICDEF_H_
#define JXS_LOGICDEF_H_
//author:hsj
//一些逻辑数据的定义 特别是类型、字段长度这种不可配表的数据
//约定
//pos 位置，从0开始， index 序号，从1开始

#include "server_def.h"

const Int32 FORMATION_GRID_NUM = 6;
const Int32 FORMATION_ROW_GRID_NUM = 3;
const Int32 FORMATION_COLLUMN_GRID_NUM = 2;
const Int32 MAX_BATTLE_ROUND_NUM = 20;
const Int32 MAX_GENERAL_SKILL_NUM = 3;
const Int32 MAX_SOLDIER_BATTLE_SKILL_NUM = 3;	//将领最大被动技能数
const Int32 MAX_CONCURRENT_SKILL_NUM = 6;		//最大同时触发技能数	

const Int32 MAX_BATTLE_REWARD_NUM = 5;
const Int32 MAX_SOLDIER_UNIT_NUM = 150;			//最大士兵种类数
const Int32 MAX_OUTER_SOLDIER_TEAM_NUM = 10;
const Int32 MAX_DAYCOUNT_COUNT_ID = 128;		//数据长度，不能改
const Int32 MAX_BULDING_LEVEL = 200;			//建筑最大等级
const Int32 MAX_BUILDING_NUM = 120;				// 建筑最多个数

const Int32 MAX_SAVE_FORMAT_NUM = 10;

const Int32 MAX_KNAPSACK_GRID_COMM_NUM = 330;
const Int32 MAX_KNAPSACK_GRID_ORDNANCE_NUM = 70;
const Int32 MAX_KNAPSACK_GRID_ROLE_NUM = 300;
const Int32 MAX_KNAPSACK_GRID_NUM = MAX_KNAPSACK_GRID_COMM_NUM + MAX_KNAPSACK_GRID_ORDNANCE_NUM + MAX_KNAPSACK_GRID_ROLE_NUM;

const Int32 MAX_ROLE_CHAPTER_NUM = 28;			//个人副本最大数量
const Int32 ROLE_CHAPTER_FISH_ID = 10000;		//新手副本id
const Int32 MAX_ENERGY_NUM = 20;				//最大军令数
const Int32 MAX_ROLE_LEVEL = 999;				//角色等级
const Int32 MAX_PROC_NUM_PER_LIST = 20;			//每种队列最大数
const Int32 CITY_WORLD_SIZE = 600;
const Int32 MAX_GENERAL_NUM = 300;
const Int32 MAX_GENERAL_NUM_ALL = MAX_GENERAL_NUM + 1;
const Int32 MAX_PROFICIENCY_LEVEL = 6;			//最大相性等级，（注意，相性等级是从0开始的)
const Int32 MAX_SKILL_LEVEL = 100;				
const Int32 MAX_ROLE_FOR_MSG = 32;				//暂定

const Int32 MAX_CARD_NUM_PER_GROUP = 40;
const Int32 INT_PERCENTAGE_RATIO = 10000;		//计算概率用
const float FLOAT_PERCENTAGE_RATIO = 10000.0f;	//计算数据用
const Int8	MAX_ROLE_EQUIP_BUY_TIMES = 10;		//奇门最多可扩充次数
const Int8	ROLE_EQUIP_BUY_TIME_GOLD = 50;		//奇门最多可扩充次数

const Int32 MAX_CHAPTER_STAGE_NUM = 12;
const Int32 CHAPTER_BOX_REWARD_NUM = 3;

const Int32 MAX_SOLDIER_RESTRICT_NUM = 5;	//每个士兵最大克制属性数
const Int32 MAX_SOLDIER_AURA_NUM = 3;		//每个士兵最大光环数
const Int32 MAX_SOLDIER_SPE_ATTR_NUM = 10;
const Int32 MAX_ARMY_AURA_NUM = 20;			//每个战斗部队最大光环数

const Int32 MAX_ROLE_EQUIP_NUM = /*FORMATION_GRID_NUM * */ROLE_EQUIP_COUNT;
const Int32 MAX_GENERAL_EQUIP_NUM = GENERAL_EQUIP_COUNT;
const Int32 MAX_ROLE_ARMY_NUM = 10; //每个角色
const Int32 MAX_VIP_LEVEL = 10;

const Int32 MAX_SOLDIER_REFORM_ITEM_NUM = 3;		//士兵改造消耗道具最大数量

//邮件
const Int32 MAX_MAIL_TITLE_LEN = 128;  //最大邮件标题长度
const Int32 MAX_MAIL_NUM_PER_TYPE = 50; //最大邮件数
const Int32 MAX_MAIL_USER_MSG_LEN = 256; //玩家邮件内容最大长度
const Int32 MAX_MAIL_ITEM_PARAM_NUM = 10;	//玩家邮件附件最多数量

const Int32 MAX_ALLIANCE_HALL_LEVEL = 30;				//军团最大等级
const Int32 MAX_ALLIANCE_CHAPTER_STAGE_NUM = 8; //军团副本最大关卡数
const Int32 ALLIANCE_CHAPTER_NUM = 5;			//军团副本数量
const Int32 MAX_ALLIANCE_NUM = 200;				// 最大军团数量
const Int8 MAX_ALLIANCE_APPLY_NUM = 5;			// 玩家申请军团数量上限
const Int32 MAX_ALLIANCE_MEMBER_NUM = 100;		// 军团成员数上限
const Int32 MAX_ALLIANCE_APPLY_LIST_NUM = 20;	// 军团申请列表上限
const Int32 MAX_ALLIANCE_SHOP_ITEM_NUM = 30;	// 军团商店物品数量上限
const Int32 MAX_ALLIANCE_WELFARE_ITEM_KIND_NUM = 20;	// 军团战事福利物品类型上限
const Int32 MAX_ALLIANCE_WELFARE_ACTIVE_LEVEL = 5;		// 军团最大活跃等级
const Int32 MAX_ALLIANCE_TECH_LEVEL = 100;				// 军团科技最高等级
const Int8	MAX_ALLIANCE_CHAPTER_MAIN_LEVEL = 5;		// 军团试练场大关卡数
const Int8	MAX_ALLIANCE_CHAPTER_SUB_LEVEL = 5;			// 军团试练场每个大关卡中的小关卡数
const Int8 MAX_ALLIANCE_CHAPTER_CHANLLENGE_NUM = 5;		// 每个玩家每天最多挑战次数
const Int8 MAX_ALLIANCE_CHAPTER_LEVEL = MAX_ALLIANCE_CHAPTER_MAIN_LEVEL*MAX_ALLIANCE_CHAPTER_SUB_LEVEL;
const Int8 MAX_ALLIANCE_INFO_NUM = 50;

const Int32 MAX_ROLE_TASK_NUM = 120;	//同时进行任务最大数量
const Int32 ROLE_TASK_OPEN_LEVEL = 3;	//任务开放等级
const Int32 MAX_REWARDS_NUM = 5;		//升级所需条件数量
const Int32 MAX_PRIVATE_CHAT_NUM = 100;	//最大房间个数


//刷新
const Int32 REFRESH_DAILY_TASK_NUM = 5;  //每次刷新日常任务数量
const Int32 AUTO_SAVE_ONLINE_DATA_TIME_LEN = 3 * 60;
const Int32 AUTO_SAVE_OFFLINE_DATA_TIME_LEN = 5 * 60;
const Int32 MINI_REFRESH_RESOURCE_TIME = 60;	//只有大于1分钟时才会刷新

const Int32 MAX_ACTIVITY_NUM = 50;

const Int32 MAX_EQUIP_LEVEL = 80; //最大装备等级

const Int32 MAX_ORDNANCE_STRENTHEN_LEVEL = 80;		//军械最大
const Int32 MAX_ORDNANCE_REFORM_LEVEL = 10;			//军械最大改造等级 
const Int32 MAX_ROLE_ORDNANCE_NUM = 32;				//穿戴军械最大数量（兵种 * 8)
const Int32 TOTAL_ORDNANCE_GRID_NUM = 35;			//批量选择最大值
const Int32 MAX_ORDNANCE_KNAPSACK_GRID_NUM = 70;	//军械在背包中的最大格子数

//兵法科技
const Int32 MAX_WARTECH_GRID_NUM = 10;	//每种士兵最大兵法位
const Int32 MAX_WARTECH_NUM = 1000;		//最大兵法数量
const Int32 MAX_SOLDIER_WARTECH_NUM = 100;
const Int32 MAX_WARTECH_EFFECT_SOLDIER_ID_NUM = 5;
const Int32 MAX_WARTECH_LEVEL = 10;		//最高等级
const Int32 MAX_WARTECH_OPEN_COND_NUM = 2;
const Int32 MAX_WARTECH_CONSUME_PARAM_NUM = 5;
const Int32 MIN_WARTECH_OPEN_LEVEL = 30;
const Int32 MAX_WARTECH_EFFECT_NUM = 5;	//科技最大效果数量
const double RETURN_RESOURCE_RATIO = 0.7;

const Int16 BOOM_BETWEEN = 1200;
const Int16 BOOM_UNDERLINE = 600;
const Int16 BOOM_RATIO = 2;

const Int32 MAX_RANK_NUM = 100;	// 排行榜

//聊天系统
const Int8 MAX_CACHE_DAY = 3;			//单个区服所有聊天信息服务器保存3天
const Int8 MAX_CACHE_MSG_NUM = 20;		//各个频道会一直显示最新的20条信息
const Int8 LIMIT_LEVEL_FOR_SPEAK = 18;	//角色18级以上才可以发送私聊、世界聊天
const Int8 INTERVAL_MSG_TIME = 5;		//所有频道聊天有5秒发言间隔
const Int8 MAX_CHAT_MEM_PRIVATE = 2;	//所有频道聊天有5秒发言间隔
const Int8 CHAT_ROOM_WORLD_ID = 1;		//世界频道id
const Int8 CHAT_ROOM_SERVICE_ID = 1;	//客服频道id
const Int8 MAX_CHAT_MSG_LENGTH = 100;	//客服频道id

//商城
const Int32 MAX_POS_GOOD_LEN = 128;		//商城商品最大位置数量

//军械
const Int8 MAX_ITEM_FOR_REFORM = 4;
const Int8 MAX_ITEM_FOR_BREAKDOWN = 2;
const Int8 REDUCE_STRENGTHEN_LEVEL = 3;

//名城
const Int32 MAX_FAMOUSCITY_STAGE_NUM = 10;


//活动
static const Int32 MAX_ACT_GIFT_REWARD_NUM = 50;
const Int32 MAX_RESOURCE_GATHER_NUM = 1024 * 1024 * 100; //100M记录资源采集最大值，超过此值不再计算




#endif