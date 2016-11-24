#ifndef JXS_DATAVERSIONDEF_H_
#define JXS_DATAVERSIONDEF_H_
//数据版本定义
//用于上线后的数据版本控制和维护

#include "../../include/basic_type_def.h"

enum DataVersion
{
	DATA_VERSION_INVALID,
	DATA_VERSION_1,				//上线数据版本

	DATA_VERSION_END,
	DATA_VERSION_CURRENT = DATA_VERSION_END - 1,

};

enum SaveFileName
{
	SAVE_FILE_NAME_PROC,	//进度队列数据
	SAVE_FILE_NAME_EVENT,	//事件数据
	SAVE_FILE_NAME_CITY,	//世界地图数据
	SAVE_FILE_NAME_ROLE,	//离线角色数据
	SAVE_FILE_NAME_GLOBAL,	//全服数据保存
	SAVE_FILE_NAME_SYS_ACT,	//公共活动数据保存
	SAVE_FILE_NAME_CHAT,	//聊天信息数据保存
	SAVE_FILE_NAME_GLOBAL_OFFLINE_USER,		//globalserver 的角色数据
	SAVE_FILE_NAME_ANNOUNCE,	//定时公告

	SAVE_FILE_NAME_END,
};

const std::string DATA_FILE_NAME[SAVE_FILE_NAME_END][2] = {
	{
		"../offlinedata/offline_procs1.dat",
		"../offlinedata/offline_procs2.dat"
	},
	{
		"../offlinedata/offline_events1.dat",
		"../offlinedata/offline_events2.dat"
	},
	{
		"../offlinedata/offline_citys1.dat",
		"../offlinedata/offline_citys2.dat"
	},
	{
		"../offlinedata/offline_users1.dat",
		"../offlinedata/offline_users2.dat"
	},
	{
		"../offlinedata/offline_global1.dat",
		"../offlinedata/offline_global2.dat"
	},
	{
		"../offlinedata/offline_act1.dat",
		"../offlinedata/offline_act2.dat"
	},
	{
		"../offlinedata/offline_chat1.dat",
		"../offlinedata/offline_chat2.dat"
	},
	{
		"../offlinedata/global_user1.dat",
		"../offlinedata/global_user2.dat"
	},
	{
		"../offlinedata/announce1.dat",
		"../offlinedata/announce2.dat"
	}
};



#endif