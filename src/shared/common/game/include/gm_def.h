#ifndef JXS_GMDEF_H_
#define JXS_GMDEF_H_
//author:hsj
//gm ЦёБо¶ЁТе

enum GmId
{
	GMCMD_INVALID,
	GMCMD_SET_AUTO_TEST,
	GMCMD_GIVE_ITEM,			//发放物品(道具id 道具数量)
	GMCMD_PROGRESS,				//进度队列(队列id, 操作:1取消，2马上完成)
	GMCMD_NORMAL_TEST,			//常用测试
	GMCMD_ANTIADDCTION,			//防沉迷设置
	GMCMD_TIME,					//时间
	GMCMD_SET_BUILDING_LEVEL,
	GMCMD_UNLOCK_WAR_CHAPTER,
	GMCMD_GIVE_WAR_TECH_UPGRADE_ITEM,
	GMCMD_BATTLE_REPORT_TEST,

	GMCMD_END,
	GMCMD_COUNT = GMCMD_END - 1,
};



#endif