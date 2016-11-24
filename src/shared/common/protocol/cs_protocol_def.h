#ifndef JXS_CSPROTOCOLDEF_H_
#define JXS_CSPROTOCOLDEF_H_
//服务器与客户端协议定义

#include "protocol_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//client <---> gamelogicserver
	enum 
	{
		//角色初始化、信息同步
		ROLE_ENTER_GAME_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN,  //进入游戏
		ROLE_ENTER_GAME_RET_SC,			//进入游戏返回
		ROLE_DAYCOUNT_INFO_SC,			//全部每日计数信息
		ROLE_DAYCOUNT_CHANGE_SC,		//每日计数变化通知
		ROLE_ATTR_CHANGE_NOTICE_SC,		//属性变化通知
		PROC_LIST_SC,					//整个进程队列发送到客户端
		PROC_CHANGE_NOTICE_SC,			//某个进程改变时的通知
		EVENT_NOTICE_SC,				//事件通知
		ROLE_ATTR_ADD_CHANGE_NOTICE_SC,		//加成属性变化通知
		ROLE_GET_DEFEND_FORMAT_INFO_CS,		//获取城防阵型
		ROLE_SEND_DEFEND_FORMAT_INFO_SC,	//发送城防阵型
		ROLE_SET_DEFEND_FORMAT_INFO_CS,		//设置城防阵型
		ROLE_ATTR_RESTORE_BOOM_CS,			//请求恢复繁荣度
		ROLE_ATTR_CHANGE_NAME_CS,			//请求修改角色名称
		ROLE_ATTR_CHANGE_NAME_NOTICE_SC,	//角色名修改通知
		ROLE_ATTR_CHANGE_AVATAR_CS,			//请求修改角色头像
		ROLE_ATTR_BUY_ENERGY_CS,			//请求购买军令
		ROLE_GMCMD_CS,						//gm指令
		ROLE_ATTR_LEADERSHIP_UPGRADE_CS,	//升级统率等级
		ROLE_ATTR_LEADERSHIP_UPGRADE_RET_SC,//升级统率等级返回
		ROLE_ATTR_BUY_REPU_CS,				//购买声望值
		ROLE_ATTR_BUY_REPU_RET_SC,			//购买声望值返回
		ROLE_ATTR_MILI_RANK_UPGRADE_CS,		//升级官职
		ROLE_ATTR_MILI_RANK_UPGRADE_RET_SC,	//升级官职返回
		ROLE_INIT_END_SC,					//初始化数据发送完成标志
		ROLE_ANTI_ADDICTION_INFO_SC,		//同步防沉迷信息
		ROLE_ERROR_CODE_SC,					//通用错误提示
		ROLE_SYNC_CONDUCT_STEP_SC,			//同步新手步骤
		ROLE_SET_CONDUCT_STEP_CS,			//设置新手步骤
		ROLE_SYNC_CF_SC,					//同步战力
		ROLE_ENERGY_RESTORE_TIME_SC,		//上次军令恢复时间
		ROLE_CONDUCT_CHANGE_ROLENAME_CS,		//新手改名
		ROLE_CONDUCT_CHANGE_ROLENAME_RET_SC,	//新手改名返回
		ROLE_REQ_SAVE_FORMAT_CS,				//请求套用阵型信息
		ROLE_REQ_SQVE_FORMAT_RET_SC,			//返回套用阵型信息
		ROLE_SET_SAVE_FORMAT_CS,				//设置套用阵型信息
		ROLE_SET_SAVE_FORMAT_RET_SC,			//设置套用阵型信息返回

		//战斗
		BATTLE_TEST_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 500,  //战斗测试
		BATTLE_REPORT_SC,				//战报
		BATTLE_RESULT_SC,				//战斗结算,简报
		BATTLE_OPEN_REPORT_CS,			//查看战报
		BATTLE_OPEN_REPORT_RET_SC,		//查看战报返回


		//建筑
		BUILDING_GET_BUILDING_LIST_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 600,  //获取建筑信息
		BUILDING_SYNC_BUILDING_LIST_SC,		//同步建筑信息
		BUILDING_UPGRADE_CS,				//升级建筑
		BUILDING_QUICK_UPGRADE_CS,			//加速升级
		BUILDING_DESTROY_CS,				//拆除建筑
		BUILDING_CHANGE_NOTICE_SC,			//建筑信息变更
		BUILDING_UPGRADE_RET_SC,			//升级建筑返回
		BUILDING_QUICK_UPGRADE_RET_SC,		//加速升级返回
		BUILDING_DESTROY_RET_SC,			//拆除建筑返回
		BUILDING_UPGRADE_CANCEL_CS,			//取消升级建筑
		BUILDING_UPGRADE_CANCEL_RET_SC,		//取消升级建筑返回

		//士兵
		SOLDIER_SOLDIER_INFO_SC = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 700,		//士兵列表
		SOLDIER_CURE_CS,				//治疗士兵
		SOLDIER_RECRUIT_CS,				//招募士兵
		SOLDIER_CANCEL_RECRUIT_CS,		//取消招募
		SOLDIER_QUICKEN_RECRUIT_CS,		//加速招募
		SOLDIER_REQ_ALL_INFO_CS,		//请求列表
		SOLDIER_TRAIN_CS,				//改装士兵
		SOLDIER_CANCEL_TRAIN_CS,		//取消改装
		SOLDIER_QUICKEN_TRAIN_CS,		//加速改装


		//物品背包 强化
		KNAPSACK_GET_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 800,  //获取全部背包信息
		KNAPSACK_GET_INFO_RET_SC,		//全部物品
		KNAPSACK_GRID_INFO_SC,			//背包格子信息变更
		KNAPSACK_USE_ITEM_CS,			//使用物品
		KNAPSACK_USE_ITEM_RET_SC,		//使用物品返回
		KNAPSACK_COUNT_SC,				//全部物品数量
		KNAPSACK_SALE_ITEM_CS,			//道具出售
		KNAPSACK_SALE_ITEM_RET_SC,		//道具出售返回
		KNAPSACK_BUY_GRID_CS,			//购买背包格子
		KNAPSACK_BUY_GRID_TIME_SC,		//购买背包格子次数

		//副本
		CHAPTER_GET_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 1000,  //获取角色副本列表
		CHAPTER_GET_INFO_RET_SC,		//所有角色副本列表
		CHAPTER_INFO_CHANGE_SC,			//某个副本信息变更
		CHAPTER_CHALLENGE_STAGE_CS,		//挑战关卡
		CHAPTER_FETCH_BOX_REWARD_CS,	//打开奖励箱子
		CHAPTER_RAID_STAGE_RET_SC,		//扫荡关卡返回
		CHAPTER_RAID_STAGE_BATTLE_REPORT_SC,	//扫荡关卡战报
		CHAPTER_ROB_COUNT_ORDER_CS,		//购买掠夺次数
		CHAPTER_ROB_COUNT_ORDER_SC,		//购买掠夺次数返回
		CHAPTER_FETCH_BOX_REWARD_SC,	//打开奖励箱子返回

		//武将
		GENERAL_SYNC_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 1500,  //获取将领信息
		GENERAL_SYNC_SC,					//获取将领信息返回
		GENERAL_LIST_SYNC_CS,				//将领列表请求
		GENERAL_LIST_SYNC_SC,				//将领列表返回
		GENERAL_RECRUIT_CS,					//武将招募
		GENERAL_RECRUIT_SC,					//武将招募返回
		GENERALEQUIP_SEND_ALL_INFO_CS,		//将领装备列表请求
		GENERALEQUIP_SEND_ALL_INFO_SC,		//将领装备列表返回
		GENERALEQUIP_EQUIP_INFO_SYNC_SC,	//将领装备信息同步		
		GENERALEQUIP_PUTON_CS,				//穿装备
		GENERALEQUIP_TAKEOFF_CS,			//脱装备
		GENERALEQUIP_UPGRADE_CS,			//装备升级
		GENERALEQUIP_UPGRADE_SC,			//装备升级返回
		GENERALEQUIP_TRANSPOSITION_CS,		//装备换位
		GENERALEQUIP_TRANSPOSITION_SC,		//装备换位返回
		GENERALEQUIP_COMPOSE_CS,			//装备合成
		GENERALEQUIP_COMPOSE_SC,			//装备合成返回
		GENERAL_SKILL_LIST_SYNC_CS,			//武将技能列表请求
		GENERAL_SKILL_LIST_SYNC_SC,			//武将技能列表返回
		GENERAL_SKILL_SYNC_CS,				//武将技能同步
		GENERAL_SKILL_SYNC_SC,				//武将技能返回
		GENERAL_SKILL_UPGRADE_CS,			//武将技能升级
		GENERAL_SKILL_UPGRADE_SC,			//武将技能升级返回
		GENERAL_SKILL_RESET_CS,				//武将技能重置
		GENERAL_SKILL_RESET_SC,				//武将技能重置返回
		GENERAL_UPGRADE_CS,					//武将升级
		GENERAL_UPGRADE_SC,					//武将升级返回
		GENERAL_GRADE_UPGRADE_CS,			//武将升阶
		GENERAL_GRADE_UPGRADE_SC,			//武将升阶返回
		GENERAL_COMPOSE_CS,					//武将合成
		GENERAL_COMPOSE_SC,					//武将合成返回
		GENERAL_TEST,

		//城池
		CITY_GET_BLOCK_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 2000,  //获取地图块的信息
		CITY_GET_BLOCK_INFO_RET_SC,		//获取地图块的信息返回
		CITY_QUERY_POSI_CS,				//查询目标坐标信息
		CITY_QUERY_POSI_RET_SC,			//查询目标坐标信息返回
		CITY_RELOCATE_CS,				//城池迁移
		CITY_RELOCATE_RET_SC,			//城池迁移返回
		CITY_SEND_ARMY_CS,				//出兵
		CITY_QUICKEN_ARMY_CS,			//加速进军
		CITY_WITHDRAW_ARMY_CS,			//撤兵
		CITY_GET_ARMY_INFO_CS,			//取部队信息
		CITY_SEND_ARMY_INFO_SC,			//返回部队信息
		CITY_SEND_ROLE_CITY_POS_SC,		//同步角色城池坐标
		CITY_CANCEL_MARCH_ARMY_CS,		//取消行军

		//技能
		ROLESKILL_REQ_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 2100, //请求技能信息
		ROLESKILL_SEND_INFO_SC,				//请求技能信息返回
		ROLESKILL_UPGRADE_SKILL_CS,			//技能升级
		ROLESKILL_UPGRADE_SKILL_RET_SC,		//技能升级结果返回
		ROLESKILL_RESET_SKILL_CS,			//技能重置,重置后技能等级为0，返还全部技能书
		ROLESKILL_RESET_SKILL_RET_SC,		//技能重置结果返回

		//装备（奇门遁甲）
		ROLEEQUIP_REQ_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 2200, //请求装备信息
		ROLEEQUIP_SEND_ALL_INFO_SC,			//发送所有装备信息
		ROLEEQUIP_EQUIP_INFO_CHANGE_SC,		//同步单个装备信息变更
		ROLEEQUIP_PUTON_EQUIP_CS,			//穿上或更换装备
		ROLEEQUIP_TAKEOFF_EQUIP_CS,			//卸下装备
		ROLEEQUIP_UPGRADE_EQUIP_CS,			//升级装备
		ROLEEQUIP_UPGRADE_EQUIP_RET_SC,		//升级装备返回
		ROLEEQUIP_TRANSPOSITION_CS,			//交换两个单元的装备
		ROLEEQUIP_TRANSPOSITION_RET_SC,		//交换两个单元的装备返回

		//任务
		TASK_REQ_TASK_LIST_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 2300, //请求任务列表
		TASK_SEND_TASK_LIST_INFO_SC,		//发送任务列表
		TASK_ACCEPT_CS,						//接受任务(日常任务）
		TASK_GIVEUP_CS,						//放弃任务(日常任务）
		TASK_COMMIT_CS,						//提交任务
		TASK_SEND_INFO_CHANGE_SC,			//任务信息变更通知
		TASK_RESET_DAILY_TASK_CS,			//重置日常任务
		TASK_FETCH_ACTIVE_REWARD_CS,		//领取活跃度奖励
		TASK_FETCH_ACTIVE_REWARD_RET_SC,	//领取活跃度奖励返回
		TASK_FETCH_ACHIEVE_REWARD_CS,		//领取成就奖励
		TASK_FETCH_ACHIEVE_REWARD_RET_SC,	//领取成就奖励返回

		//活动
		ACT_REQ_LIST_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 2400, //请求活动列表
		ACT_SEND_LIST_INFO_SC,				//发送活动列表
		ACT_SEND_INFO_CHANGE_SC,			//活动信息变更通知
		ACT_FETCH_GIFT_ACT_REWARD_CS,		//领取礼包类活动奖励

		//军团
		ALLIANCE_REQ_RANK_LIST_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 2600, //请求军团排行榜
		ALLIANCE_SEND_RANK_LIST_SC,			// 军团排行榜返回
		ALLIANCE_REQ_INFO_CS,				// 军团信息请求
		ALLIANCE_SEND_INFO_SC,				// 军团信息返回
		ALLIANCE_SINGLE_INFO_REQ_CS,		// 玩家查看其它军团信息请求
		ALLIANCE_SINGLE_INFO_RET_SC,		// 玩家查看其它军团信息请求返回
		ALLIANCE_SRUCT_UPGRADE_REQ_CS,		// 军团建筑升级
		ALLIANCE_SRUCT_UPGRADE_RET_SC,		// 军团建筑升级返回
		ALLIANCE_STRUCT_DONATE_REQ_CS,		// 军团捐献请求
		ALLIANCE_STRUCT_DONATE_RET_SC,		// 军团捐献返回
		ALLIANCE_CREATE_REQ_CS,				// 军团创建
		ALLIANCE_CREATE_RET_SC,				// 军团创建返回
		ALLIANCE_APPLY_REQ_CS,				// 军团加入申请
		ALLIANCE_APPLY_RET_SC,				// 军团加入申请返回 (通知玩家申请处理结果)
		ALLIANCE_APPLY_REPLY_SC,			// 军团加入申请返回 (通知申请流程)
		ALLIANCE_APPLY_LIST_SC,				// 军团列表
		ALLIANCE_APPLY_HANDLE_CS,			// 处理军团申请
		ALLIANCE_QUIT_CS,					// 退出军团请求
		ALLIANCE_QUIT_SC,					// 退出军团返回
		ALLIANCE_MEMBER_LIST_CS,			// 军团成员列表请求
		ALLIANCE_MEMBER_LIST_SC,			// 军团成员列表返回
		ALLIANCE_POST_CHANGE_CS,			// 军团成员职位变更
		ALLIANCE_POST_CHANGE_SC,			// 军团成员职位变更返回
		ALLIANCE_POST_CHANGE_NOTE_SC,		// 军团成员职位通知
		ALLIANCE_POST_SET_CS,				// 军团职位设置
		ALLIANCE_POST_SET_SC,				// 军团职位设置返回
		ALLIANCE_CONDITION_SET_CS,			// 军团加入条件设置
		ALLIANCE_CONDITION_SET_SC,			// 军团加入条件设置返回
		ALLIANCE_RECRUIT_CS,				// 军团招募
		ALLIANCE_TECH_HALL_INFO_CS,			// 科技大厅信息请求
		ALLIANCE_TECH_HALL_INFO_SC,			// 科技大厅信息返回
		ALLIANCE_TECH_DONATE_CS,			// 军团科技捐献
		ALLIANCE_TECH_DONATE_SC,			// 军团科技捐献返回
		ALLIANCE_SHOP_ITEM_LIST_CS,			// 军团商店物品列表请求
		ALLIANCE_COMMON_SHOP_ITEM_LIST_SC,	// 军团普通商店物品列表返回
		ALLIANCE_ADVANCED_SHOP_ITEM_LIST_SC,// 军团高级商店物品列表返回
		ALLIANCE_ITEM_ORDER_CS,				// 军团商店物品购买
		ALLIANCE_ITEM_ORDER_SC,				// 军团商店物品购买返回
		ALLIANCE_INFO_STATION_LOGIN_SYNC,	// 情报站信息列表
		ALLIANCE_INFO_STATION_NOTE_SC,		// 情报发送
		ALLIANCE_WELFARE_INFO_CS,			// 福利院同步请求
		ALLIANCE_WELFARE_INFO_SC,			// 福利院同步返回
		ALLIANCE_DAILY_WELFARE_CS,			// 日常福利领取
		ALLIANCE_DAILY_WELFARE_SC,			// 日常福利领取返回 
		ALLIANCE_WAR_WELFARE_ITEM_LIST_CS,	// 战事福利物品列表请求
		ALLIANCE_WAR_WELFARE_ITEM_LIST_SC,	// 战事福利物品列表返回
		ALLIANCE_WAR_WELFARE_HANDLE_CS,		// 战事福利发放(军团长操作)
		ALLAINCE_WAR_WELFARE_HANDLE_REPLY_SC,	// 战事福利发放返回
		ALLIANCE_ACTIVE_WELFAER_CS,			// 活跃福利领取
		ALLIANCE_ACTIVE_WELFARE_SC,			// 活跃福利领取返回
		ALLIANCE_ACTIVE_WELFARE_SYNC_CS,	// 活跃福利信息同步请求
		ALLIANCE_ACTIVE_WELFARE_SYNC_SC,	// 活跃福利信息同步返回
		ALLIANCE_ACTIVE_RANK_CS,			// 活跃度排行请求
		ALLIANCE_ACTIVE_RANK_SC,			// 活跃度排行返回
		ALLIANCE_WELFAER_LEVEL_UP_CS,		// 福利院升级
		ALLIANCE_WELFAER_LEVEL_UP_SC,		// 福利院升级返回
		ALLIANCE_CHAPTER_LIST_CS,			// 试练场列表请求
		ALLIANCE_CHAPTER_LIST_SC,			// 试练场列表返回
		ALLIANCE_CHAPTER_CHALLENGE_CS,		// 试练场挑战
		ALLIANCE_CHAPTER_CHALLENGE_SC,		// 试练场挑战返回
		ALLIANCE_CHAPTER_REWARD_FETCH_CS,	// 试练场奖励领取
		ALLIANCE_CHAPTER_REWARD_FETCH_SC,	// 试练场奖励领取返回
		ALLIANCE_ACTIVITY_APPLY_CS,			// 群雄报名
		ALLIANCE_ACTIVITY_APPLY_SC,			// 群雄报名返回
		ALLIANCE_ACTIVITY_ALLIANCE_LIST_CS,	// 群雄参赛列表
		ALLIANCE_ACTIVITY_ALLIANCE_LIST_SC,	// 群雄参赛列表返回
		ALLIANCE_ACTIVITY_START_SC,			// 群雄活动开始通知
		ALLIANCE_ACTIVITY_END_SC,			// 群雄活动结束通知
		ALLIANCE_ACTIVITY_NOTE_SC,			// 群雄战报
		ALLIANCE_TEST,

		//好友
		FRIEND_REQ_LIST_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 3000, //请求好友列表
		FRIEND_SEND_LIST_INFO_SC,
		FRIEND_REQ_SHIELD_LIST_CS,		//获取屏蔽用户列表
		FRIEND_REQ_SHIELD_LIST_RET_SC,	//返回屏蔽用户列表
		FRIEND_SHIELD_ROLE_CS,			//屏蔽用户
		FRIEND_SHIELD_ROLE_RET_SC,		//屏蔽用户返回

		//商店
		SHOP_BUY_GOODS_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 3200, //购买商品
		SHOP_BUY_GOODS_RET_SC,			//购买商品返回
		SHOP_BUY_DAYCOUNT_INFO_SC,		//发送限购信息
		SHOP_BUY_DAYCOUNT_CHANGE_SC,	//限购信息更新通知

		//军械
		ORDNANCE_REQ_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 3400, //请求军械信息
		ORDNANCE_SEND_ALL_INFO_SC,			//发送所有军械信息
		ORDNANCE_INFO_CHANGE_SC,			//同步单个军械信息变更
		ORDNANCE_PUTON_CS,					//穿上军械
		ORDNANCE_TAKEOFF_CS,				//卸下军械
		ORDNANCE_STRENGTHEN_CS,				//强化军械
		ORDNANCE_STRENGTHEN_RET_SC,			//强化军械结果返回
		ORDNANCE_REFORM_CS,					//改造军械
		ORDNANCE_REFORM_RET_SC,				//改造军械结果返回
		ORDNANCE_ADVANCE_CS,				//进阶军械
		ORDNANCE_ADVANCE_RET_SC,			//进阶军械结果返回
		ORDNANCE_BREAK_DOWN_CS,				//分解军械
		ORDNANCE_BREAK_DOWN_RET_SC,			//分解军械结果返回
		ORDNANCE_PUTON_RET_SC,				//穿上军械结果返回
		ORDNANCE_TAKEOFF_RET_SC,			//卸下军械结果返回
		ORDNANCE_SYNTHESIS_CS,				//合成军械
		ORDNANCE_SYNTHESIS_RET_SC,			//合成军械结果返回

		//开采工坊
		PRODUCE_ITEM_ADD_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 3600,	//开采工坊,制作增益道具
		PRODUCE_ITEM_ADD_RET_SC,		//开采工坊,制作增益道具返回
		PRODUCE_ITEM_ADD_CANCEL_CS,		//取消制作增益道具
		PRODUCE_ITEM_ADD_CANCEL_RET_SC,	//取消制作增益道具返回
		PRODUCE_ITEM_ADD_QUICK_CS,		//加速制作增益道具
		PRODUCE_ITEM_ADD_QUICK_RET_SC,	//加速制作增益道具返回

		//兵法科技
		WARTECH_REQ_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 3800, //请求兵法科技信息
		WARTECH_SEND_ALL_INFO_SC,			//发送全部信息
		WARTECH_TECH_INFO_CHANGE_SC,		//科技信息变更同步
		WARTECH_SOLDIER_INFO_CHANGE_SC,		//士兵兵法信息变更同步
		WARTECH_UPGRADE_TECH_CS,			//升级科技
		WARTECH_PUTON_TECH_CS,				//装备科技
		WARTECH_TAKEOFF_TECH_CS,			//卸下科技
		WARTECH_UNLOCK_GRID_CS,				//解锁格子
		WARTECH_REFORM_SOLDIER_CS,			//改造士兵
		WARTECH_REFORM_SOLDIER_RET_SC,		//改造士兵返回
		
		//测试相关
		TEST_PROT_DATA_LEN_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 4000, //协议长度测试
		TEST_ITEM_GIVE_CS,
		TEST_ITEM_GIVE_SC,
		TEST_PROT_STREAM_CS,		//测试流数据

		//计略俯计谋研发
		SCHEME_REQ_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 4200,//请求计略信息
		SCHEME_SEND_INFO_SC,			//发送全部信息
		SCHEME_DEVELOP_CS,				//计略研发
		SCHEME_DEVELOP_RET_SC,			//计略俯计略研发返回
		SCHEME_DEVELOP_CANCEL_CS,		//取消计略研发
		SCHEME_DEVELOP_CANCEL_RET_SC,	//取消计略研发返回
		SCHEME_DEVELOP_QUICK_CS,		//加速计略研发
		SCHEME_DEVELOP_QUICK_RET_SC,	//加速计略研发返回

		//名城 
		FAMOUSCITY_REQ_BASE_INFO_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 4300,	//请求名城基本信息
		FAMOUSCITY_SEND_BASE_INFO_SC,				//返回名城基本信息
		FAMOUSCITY_REQ_STAGE_INFO_CS,				//请求防守点信息
		FAMOUSCITY_SEND_STAGE_INFO_SC,				//返回防守点信息
		FAMOUSCITY_REQ_RANK_INFO_CS,				//请求积分排行信息
		FAMOUSCITY_SEND_RANK_INFO_SC,				//返回积分排行信息
		FAMOUSCITY_REQ_ARMY_LIST_CS,				//请求名城相关的行军列表
		FAMOUSCITY_SEND_ARMY_LIST_SC,				//返回名城相关的行军列表
		FAMOUSCITY_REQ_ARMY_INFO_CS,				//请求名城相关的某个行军具体信息
		FAMOUSCITY_SEND_ARMY_INFO_SC,				//返回名城相关的某个行军具体信息
		FAMOUSCITY_FETCH_RANK_REWARD_CS,			//领取积分排行奖励
		FAMOUSCITY_FETCH_RANK_REWARD_RET_SC,		//领取积分排行奖励结果返回
		FAMOUSCITY_ATTACK_STAGE_CS,					//请求攻击防守点
		FAMOUSCITY_ATTACK_STAGE_RET_SC,				//请求攻击防守点结果返回
		FAMOUSCITY_INTERCEPT_CS,					//请求攻击行军
		FAMOUSCITY_INTERCEPT_SC,					//请求攻击行军返回
		FAMOUSCITY_GET_CITY_LIST_CS,				//请求名城列表
		FAMOUSCITY_SEND_CITY_LIST_SC,				//返回名城列表

		//测试
		

		//VIP
		
		//世界BOSS

		//排行榜

		//邮件

		//聊天
		CHAT_REQ_BROADCAST_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 5200, //用户请求广播，扣除道具

		//跨服
		
		//侦察
		SPY_CS = prot::CLIENT_GAMELOGIC_MSG_BEGIN + 5400,			// 侦察
		SPY_ROLE_SC,												// 侦察玩家返回
		SPY_ORE_SC,													// 侦察矿点返回
	};
}

#pragma pack(pop)

#endif
