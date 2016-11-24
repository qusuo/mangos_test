#ifndef JXS_RMICMD_DEF_H_
#define JXS_RMICMD_DEF_H_
//author:hsj

enum RmiCmd
{
	RMI_CMD_INVALID,
	RMI_CMD_REQ_LOGIN,					//请求登录
	RMI_CMD_REQ_CREATE_ROLE,			//请求创建角色
	RMI_CMD_LOAD_ROLE_DATA,				//加载角色在线数据
	RMI_CMD_SAVE_ROLE_DATA,				//保存角色在线数据
	RMI_CMD_SAVE_OFFLINE_ROLE_DATA,		//保存角色离线数据
	RMI_CMD_GET_MAIL_LIST,				//获取邮件列表
	RMI_CMD_GET_MAIL_INFO,				//获取邮件具体信息
	RMI_CMD_SET_MAIL_STATE,				//设置邮件状态
	RMI_CMD_ADD_MAIL,					//添加邮件
	RMI_CMD_CREATE_MAIL,				//创建邮件
	RMI_CMD_CREATE_ALLIANCE,			//创建公会
	RMI_CMD_UPDATE_ALLIANCE,			//更新公会
	RMI_CMD_DELETE_ALLIANCE,			//删除公会
	RMI_CMD_CREATE_ALLIANCE_MEMBER,		//创建公会成员
	RMI_CMD_UPDATE_ALLIANCE_MEMBER,		//更新公会成员
	RMI_CMD_DELETE_ALLIANCE_MEMBER,		//删除公会成员
	RMI_CMD_LOAD_RANK_LIST,				//加载排行数据
	RMI_CMD_SAVE_RAKN_LIST,				//保存排行数据
	RMI_CMD_LOAD_ALLIANCE_DATA,			//加载所有公会数据
	RMI_CMD_UPDATE_GAME_PROC,			//操作进度队列
	RMI_CMD_UPDATE_GAME_EVENT,			//操作事件
	RMI_CMD_UPDATE_LOGIN_INFO,			//更新登录信息
	RMI_CMD_CHANGE_ROLENAME,			//改角色名

	RMI_CMD_END,
};

#endif