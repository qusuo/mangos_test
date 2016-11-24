#ifndef JXS_ACTIVITYDEF_H_
#define JXS_ACTIVITYDEF_H_
//活动类型定义
#include "game_value_def.h"

//大活动类型 决定了协议、配表、界面格式
enum ActType
{
	ACT_TYPE_INVALID,
	ACT_TYPE_FIRST_CHARGE,		//首充
	ACT_TYPE_GIFT,				//礼包类(有n个项，每个项都有一个条件和一个奖励）
	ACT_TYPE_LIMIT_BUY,			//限购
	ACT_TYPE_RANK,				//排行活动
	ACT_TYPE_STATISTICS,		//全服统计
	ACT_TYPE_SWITCH,			//系统开关类
	ACT_TYPE_CONCHARGE,			//天连续充值
	ACT_TYPE_ADD,				//系统加成
	ACT_TYPE_DRAW,				//抽奖活动
	ACT_TYPE_HOLIDAY,			//度假胜地
	ACT_TYPE_SERVER_CHARGE,		//全服充值（欢度新春)
	ACT_TYPE_DRAWRANK,			//转盘
	ACT_TYPE_WORSHIP,			//拜财神
	ACT_TYPE_LIMIT_BUY_EX,		//加成+限购
	ACT_TYPE_EXCHANGE,			//兑换
	ACT_TYPE_LIMIT_EXCHANGE,	//限时兑换 掉落+兑换
	ACT_TYPE_FAMOUS_GENERAL,	//名将招募
	ACT_TYPE_COMPOS_SOLDIER,	//钢铁洪流 抽奖+合成
	ACT_TYPE_CARD,				//翻牌（武器校场）
	ACT_TYPE_DRAWRANKEXCH,		//抽奖+排行+兑换（萨达姆遗产）
	ACT_TYPE_DRAWCOMPOSE,		//抽奖+合成(貌似跟钢铁洪流重复）
	ACT_TYPE_KILL,				//歼灭
	ACT_TYPE_GATHER_RANK,		//勤劳致富
	ACT_TYPE_BLESSING,			//祝福 掉落+限次机率兑换
	ACT_TYPE_CARD_EXT,			//消灭一对是一对(三消)
	ACT_TYPE_MINE,				//宝藏 掉落 + 合成 + 开启
	ACT_TYPE_DROP,				//掉落
	ACT_TYPE_ALLIANCE_WAR,		//军团混战

	ACT_TYPE_END,
};

enum ActGiftType
{
	ACTGIFT_TYPE_INVALID,
	ACTGIFT_TYPE_INVEST,			//投资计划
	ACTGIFT_TYPE_ROLE_LEVEL,		//等级礼包
	ACTGIFT_TYPE_RECHARGE,			//累计充值
	ACTGIFT_TYPE_ONLINE,			//在线奖励
	ACTGIFT_TYPE_RELOGIN,			//重登日期
	ACTGIFT_TYPE_ONECHARGE,			//一次性充值
	ACTGIFT_TYPE_COMPOS_GENERAL,	//进阶将领
	ACTGIFT_TYPE_ATTACK_ROLE_CITY,	//雷霆计划
	ACTGIFT_TYPE_ARENA,				//疯狂竞技
	ACTGIFT_TYPE_GATHER,			//采集
	ACTGIFT_TYPE_ALLIANCE_GATHER,	//军团募集，N种条件
	ACTGIFT_TYPE_CHAPTER,			//攻打关卡次数
	ACTGIFT_TYPE_CONLOGIN,			//登录次数


	ACTGIFT_TYPE_END,
};

enum ActRankType
{
	ACTRANK_TYPE_INVALID,
	ACTRANK_TYPE_FC,					//战力
	ACTRANK_TYPE_CHAPTER,				//关卡
	ACTRANK_TYPE_HONOR,					//荣誉
	ACTRANK_TYPE_ALLIANCE_LEVEL,		//军团等级
	ACTRANK_TYPE_ALLIANCE_FC,			//军团战力
	ACTRANK_TYPE_ALLIANCE_DONATE,		//军团贡献（因为还会有加成类的功能，所以要做些扩展）
	ACTRANK_TYPE_ALLIANCE_CHAPTER,

	ACTRANK_TYPE_END,
};

enum ActStatisticsType
{
	ACTSTATISTICS_TYPE_INVALID,
	ACTSTATISTICS_TYPE_VIP,			//全服充V

	ACTSTATISTICS_TYPE_END,

};

enum ActSwitchType
{
	ACTSWITCH_TYPE_INVALID,
	ACTSWITCH_TYPE_PROSPECT,	//探宝
	ACTSWITCH_TYPE_DRAW_EQUIP,	//抽装备

	ACTSWITCH_TYPE_END,
};

enum ActDrawRankType
{
	ACTDRAWRANK_TYPE_INVALID,
	ACTDRAWRANK_TYPE_CONSUME_GOLD,	//消费转盘
	ACTDRAWRANK_TYPE_GOLD_BUY,		//极限单兵,军工科技探索,军工转盘

	ACTDRAWRANK_TYPE_END,
};

//note(hsj)可以考虑单个加成项通过配表控制
enum ActAddType
{
	ACTADD_TYPE_INVALID,
	ACTADD_TYPE_CHARGE,					//充值加成
	ACTADD_TYPE_DRAW_EQUIP,				//抽装折扣
	ACTADD_TYPE_GENERAL,				//将领招募折扣
	ACTADD_TYPE_BIRTHDAY,				//周年庆，n样加成
	ACTADD_TYPE_HAPPY,					//全民狂欢，n样加成
	ACTADD_TYPE_TECH,					//科技优惠
	ACTADD_TYPE_DONATE,					//军团捐献金币反还
	ACTADD_TYPE_CHAPTER_EQUIP,			//装备关卡次数
	ACTADD_TYPE_CHAPTER_ORDANCE,		//配件关卡次数
	ACTADD_TYPE_CHAPTER_ORDANCE_EX,		//配件关卡伤害+购买折扣
	ACTADD_TYPE_CHAPTER_EQUIP_EX,		//装备关卡伤害+购买折扣


	ACTADD_TYPE_END,
};

enum ActDrawType
{
	ACTDRAW_TYPE_INVALID,
	ACTDRAW_TYPE_LUCKY,		//幸运转盘
	ACTDRAW_TYPE_BET,		//充值下注
	ACTDRAW_TYPE_SOLDIER,	//坦克拉霸(这个可能要用另一种抽奖方式）

	ACTDRAW_TYPE_END,
};

enum ActExchangeType
{
	ACTEXCHANGE_TYPE_INVALID,
	ACTEXCHANGE_TYPE_ORDANCE,	//配件材料兑换
	ACTEXCHANGE_TYPE_TECH,		//技术革新

	ACTEXCHANGE_TYPE_END,
};

enum ActLimitExchangeType
{
	ACTLIMITEXCHANGE_TYPE_INVALID,	
	ACTLIMITEXCHANGE_TYPE_ORDANCE,  //限时兑换（碎片）
	ACTLIMITEXCHANGE_TYPE_EQUIP,	//限时兑换（装备）

	ACTLIMITEXCHANGE_TYPE_END,
};


enum ActClassify
{
	ACT_CLASSIFY_INVALID,
	ACT_CLASSIFY_ROLE,		//角色相关活动（只有角色数据）
	ACT_CLASSIFY_SYS,		//全服相关活动（全服或者全服加角色数据）

	ACTIVITY_TYPE_END,
};

//
//
//
////子任务类型（任务内容）
//enum ActivityUnitType
//{
//	ACTIVITY_UNIT_TYPE_INVALID,
//	ACTIVITY_UNIT_TYPE_ROLE_LEVEL,		//等级礼包
//	ACTIVITY_UNIT_TYPE_FIRST_CHARGE,	//首充奖励
//	ACTIVITY_UNIT_TYPE_DAILY_CHARGE,	//每日充值奖励
//	ACTIVITY_UNIT_LIMIT_MALL,			//限购
//
//
//	ACTIVITY_UNIT_TYPE_END,
//	ACTIVITY_UNIT_TYPE_COUNT = ACTIVITY_UNIT_TYPE_END - 1,
//
//};

//奖励状态
enum ActivityRewardState
{
	ACTIVITY_REWARD_STATE_INVALID,		//不可领取
	ACTIVITY_REWARD_STATE_AVAILABLE,	//可领取
	ACTIVITY_REWARD_STATE_FETCHED,		//已领取
};

#endif