#ifndef JXS_GAMELOGDEF_H_
#define JXS_GAMELOGDEF_H_
//游戏日志定义

enum LogType
{
	LOG_TYPE_INVALID,
	 
	LOG_TYPE_ROLE_COMMON,	//主角  
	LOG_TYPE_BATTLE,		//战斗
	LOG_TYPE_BUILDING,		//建筑
	LOG_TYPE_SOLDIER,		//士兵
	LOG_TYPE_KNAPSACK,		//物品背包 强化
	LOG_TYPE_CHAPTER,		//副本
	LOG_TYPE_GENERAL,		//武将
	LOG_TYPE_CITY,			//城池
	LOG_TYPE_ROLESKILL,		//技能
	LOG_TYPE_ROLEEQUIP,		//装备
	LOG_TYPE_TASK,			//任务
	LOG_TYPE_ACT,			//活动
	LOG_TYPE_ALLIANCE,		//军团
	LOG_TYPE_FRIEND,		//好友
	LOG_TYPE_SHOP,			//商店
	LOG_TYPE_ORDNANCE,		//军械
	LOG_TYPE_PRODUCE,		//开采工坊
	LOG_TYPE_WARTECH,		//兵法科技
	LOG_TYPE_TEST,			//测试相关
	LOG_TYPE_SCHEME,		//计略俯计谋研发
	//VIP

	LOG_TYPE_CITY_WORLD,		//世界
	//排行榜
	//邮件
	//聊天
	//跨服
	//LOGTYPE_BARRACK,			//兵营

	LOG_TYPE_END,
};


#endif