#ifndef JXS_ALLIANCEPROT_H_
#define JXS_ALLIANCEPROT_H_

//军团协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../include/value_def.h"
#include "../../game/structs/soldier_data.h"
#include "../../game/structs/army_format_data.h"
#include "../../game/structs/item_data.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	// 军团创建
	struct CSAllianceCreate{
		CSAllianceCreate()
			:header(csprot::ALLIANCE_CREATE_REQ_CS)
			, type(0)
		{
			memset(alliance_name, 0, sizeof(alliance_name));
		}
		prot::ProtHeader header;
		Int16 type;									// 1:金币创建, 2:资源创建
		Int16 join_type;							// 1:申请即可加入, 2:需要审批才能加入
		char alliance_name[MAX_32_CHAR_LEN];
	};

	// 军团创建返回
	struct SCAllianceCreate{
		SCAllianceCreate()
			:header(csprot::ALLIANCE_CREATE_RET_SC)
			, ret(0)
		{}
		prot::ProtHeader header;
		Int32 ret;							// 1:创建成功, 2:创建失败(名字已存在) , 3:创建失败(名字不合法), 4:创建失败(金币不足)， 5:创建失败(资源不足), 6:创建失败(等级不足), 7:创建失败(已在军团中)
		Int64 alliance_id;					// 军团id
	};

	// 军团加入申请
	struct CSAllianceApply{
		CSAllianceApply()
			:header(csprot::ALLIANCE_APPLY_REQ_CS)
			, alliance_id(0)
		{
		}
		prot::ProtHeader header;
		Int64 alliance_id;
	};

	// 申请条件检查
	struct SCAllianceApplyReply{
		SCAllianceApplyReply()
			:header(csprot::ALLIANCE_APPLY_REPLY_SC)
			, ret(0)
		{
		}
		prot::ProtHeader header;
		Int32 ret;			// 1:申请成功, 2:申请失败(等级不够), 3:申请失败(战力不够), 4:申请失败(申请军团数量达到上限), 5:申请失败(军团人数达到上限), 6:军团申请数量达到上限, 7:玩家已在军团中, 8:玩家没在申请中
	};

	// 申请处理的结果
	struct SCAllianceApply{
		SCAllianceApply()
			:header(csprot::ALLIANCE_APPLY_RET_SC)
			, ret(0)
		{
		}
		prot::ProtHeader header;
		Int32 ret;			// 1:同意, 2:拒绝
	};

	// 处理军团申请
	struct CSHandleAllianceApplyKick{
		CSHandleAllianceApplyKick()
			:header(csprot::ALLIANCE_APPLY_HANDLE_CS)
			, type(0)
			, role_id(0)
		{
		}

		enum AllianceApplyHandleTYpe {
			ALLIANCE_APPLY_HANDLE_AGREE,
			ALLIANCE_APPLY_HANDLE_DENY,
			ALLIANCE_APPLY_HANDLE_CLEAR_LIST,
			ALLIANCE_APPLY_HANDLE_KICK,
		};
		prot::ProtHeader header;
		Int32 type;		// 1:同意, 2:拒绝, 3:清空申请列表, 4:踢出
		RoleID role_id;	// 角色id
	};

	struct ApplyMember {
		ApplyMember()
			:role_id(0)
			, role_level(0)
			, role_attack_power(0)
		{
			memset(role_name, 0, sizeof(role_name));
		}
		RoleID role_id;
		Int32 role_level;
		Int32 role_attack_power;
		char role_name[MAX_32_CHAR_LEN];
	};

	// 申请列表
	struct SCApplyList{
		SCApplyList()
			:header(csprot::ALLIANCE_APPLY_LIST_SC)
			, num(0)
		{
		}
		prot::ProtHeader header;
		Int32 num;
		ApplyMember list[MAX_ALLIANCE_APPLY_LIST_NUM];
	};

	// 退出军团请求
	struct CSAllianceQuit{
		CSAllianceQuit() :header(csprot::ALLIANCE_QUIT_CS){
		}
		enum AllianceQuitType {
			ALLIANCE_QUIT_APPLY_QUIT,
			ALLIANCE_QUIT_KICK,
			ALLIANCE_QUIT_DISMISS,
		};
		prot::ProtHeader header;
	};

	// 退出军团返回
	struct SCAllianceQuit{
		SCAllianceQuit() :header(csprot::ALLIANCE_QUIT_SC){
		}
		prot::ProtHeader header;
		Int32 ret;			// 1:退出成功, 2:退出失败(团战中不能退出), 3:解散退出, 4:踢出
	};

	struct AllianceInfo{
		AllianceInfo()
			:alliance_id(0)
			, level(0)
			, schedule(0)
			, member_num(0)
			, rank(0)
			, attack_power_limit(0)
			, level_limit(0)
		{
			memset(alliance_declaration, 0, sizeof(alliance_declaration));
			memset(allliance_name, 0, sizeof(allliance_name));
			memset(alliance_leader_name, 0, sizeof(alliance_leader_name));
			memset(alliance_donate_num, 0, sizeof(alliance_donate_num));
		}
		Int64 alliance_id;
		Int32 level;												// 军团等级
		Int32 schedule;												// 军团建设度
		Int32 member_num;											// 成员数量
		Int32 rank;													// 玩家排行
		Int32 attack_power_limit;									// 攻击力限制(加入条件)
		Int32 level_limit;											// 等级限制(加入)
		char alliance_declaration[MAX_32_CHAR_LEN];					// 军团宣言
		char allliance_name[MAX_32_CHAR_LEN];						// 军团名称
		char alliance_leader_name[MAX_32_CHAR_LEN];					// 军团长名称
		Int16 alliance_donate_num[ALLIANCE_DONATE_TYPE_COUNT];		// 军团捐献
	};

	// 军团信息请求
	struct CSAllianceInfo{
		CSAllianceInfo()
			:header(csprot::ALLIANCE_REQ_INFO_CS)
		{}
		prot::ProtHeader header;
	};

	// 请求军团信息返回
	const static Int32 MAX_ALLIANCE_APPLY_NUM = 100;
	struct SCAllianceInfo{
		SCAllianceInfo()
			:header(csprot::ALLIANCE_SEND_INFO_SC)
		{}
		prot::ProtHeader header;
		Int32 apply_num;
		ApplyMember apply_member_list[MAX_ALLIANCE_APPLY_NUM];
		AllianceInfo alliance_info;
	};

	/**
	// 军团排行榜请求
	struct CSAllianceRankListReq{
		CSAllianceRankListReq()
			:header(csprot::ALLIANCE_REQ_RANK_LIST_CS)
		{

		}
		prot::ProtHeader header;
		Int32 page_index;
	};
	*/

	struct simple_alliance_info{
		simple_alliance_info()
			:alliance_id(0)
			, level(0)
			, member_num(0)
			, attack_power(0)
		{
			memset(allliance_name, 0, sizeof(allliance_name));
			memset(allliance_leader_name, 0, sizeof(allliance_leader_name));
		}
		RoleID alliance_id;
		Int32 level;
		Int32 member_num;
		Int64 attack_power;
		char allliance_name[MAX_32_CHAR_LEN];
		char allliance_leader_name[MAX_32_CHAR_LEN];
	};

	// 军团列表请求
	struct CSAllianceListReq {
		CSAllianceListReq()
			:header(csprot::ALLIANCE_REQ_RANK_LIST_CS)
			,page_index(0)
		{
		}
		prot::ProtHeader header;
		Int32 page_index;
	};

	// 军团排行榜返回(排行之后的军团列表)
	const static Int32 ALLIANCE_NUMBER_PER_PAGE = 20;
	struct SCAllianceRankList{
		SCAllianceRankList()
			:header(csprot::ALLIANCE_SEND_RANK_LIST_SC)
			, page_index(0)
			, alliance_num(0)
		{
		}
		prot::ProtHeader header;
		Int32 page_index;
		Int32 alliance_num;
		simple_alliance_info list[MAX_ALLIANCE_NUM];
	};

	// 简单军团信息
	struct SingleAllianceInfo{
		SingleAllianceInfo()
			:alliance_id(0)
			, level(0)
			, member_num(0)
		{
			memset(allliance_name, 0, sizeof(allliance_name));
			memset(allinace_leader_name, 0, sizeof(allinace_leader_name));
		}
		Int64 alliance_id;
		Int32 level;
		Int32 member_num;
		char allliance_name[MAX_32_CHAR_LEN];
		char allinace_leader_name[MAX_32_CHAR_LEN];
	};

	// 查看其它军团信息
	struct CSSingleAllianceInfo{
		CSSingleAllianceInfo()
			:header(csprot::ALLIANCE_SINGLE_INFO_REQ_CS)
			, alliance_id(0)
		{
		}
		prot::ProtHeader header;
		Int64 alliance_id;
	};

	// 查看其它军团信息返回
	struct SCSingelAllianceInfo{
		SCSingelAllianceInfo()
			:header(csprot::ALLIANCE_SINGLE_INFO_RET_SC)
		{
		}
		prot::ProtHeader header;
		SingleAllianceInfo info;
	};

	// 军团捐献
	struct CSAllianceDenote{
		CSAllianceDenote()
			:header(csprot::ALLIANCE_STRUCT_DONATE_REQ_CS)
			, type(0)
			, num(0)
		{
		}
		prot::ProtHeader header;
		Int32 type;			// 资源类型
		Int32 num;			// 捐献次数
	};

	// 军团捐献返回
	struct SCAllianceDenote{
		SCAllianceDenote()
			:header(csprot::ALLIANCE_STRUCT_DONATE_RET_SC)
			, type(0)
			, ret(0)
		{}
		prot::ProtHeader header;
		Int32 ret;		// 1:捐献成功, 2:捐献失败(次数达到上限), 3:捐献失败(资源不足)
		Int32 type;		// 资源类型
		Int32 num;		// 捐献次数
	};

	// 军团建筑升级
	struct CSAllianceUpgrade{
		CSAllianceUpgrade()
			:header(csprot::ALLIANCE_SRUCT_UPGRADE_REQ_CS)
			, type(0)
			, level(0)
		{}
		prot::ProtHeader header;
		Int32 hall_type;	// 大厅类型 1:军团大厅, 2:科技大厅, 3:福利院
		Int32 type;			// 1:一般升级, 2:自动升级直到建设度不足
		Int32 level;		// 一般升级的级数
	};

	// 军团建筑升级返回
	struct SCAllianceUpgrade{
		SCAllianceUpgrade()
			:header(csprot::ALLIANCE_SRUCT_UPGRADE_RET_SC)
			, ret(0)
			, level(0)
			, schedule(0)
		{}
		prot::ProtHeader header;
		Int32 ret;		// 1:升级成功, 2:升级失败(建设度不足) 3:达到等级上限
		Int32 hall_type;	// 大厅类型 1:军团大厅, 2:科技大厅, 3:福利院
		Int32 level;	// 大厅当前等级
		Int32 schedule;	// 当前建设度
	};

	// 军团成员列表请求 
	struct CSAllianceMemberList{
		CSAllianceMemberList() :header(csprot::ALLIANCE_MEMBER_LIST_CS){}
		prot::ProtHeader header;
	};

	struct MemberInfo{
		MemberInfo()
			:role_id(0)
			, attack_power(0)
			, level(0)
			, pos(0)
			, rank(0)
			, gender(0)
			, contrib(0)
			, last_online_time(0)
		{
			memset(name, 0, sizeof(name));
		}
		RoleID role_id;
		Int32 attack_power;
		Int32 level;
		Int32 pos;
		Int32 rank;
		Int32 gender;
		Int32 contrib;
		Int64 last_online_time;
		char name[MAX_32_CHAR_LEN];
	};

	// 军团成员列表返回
	struct SCAllianceMemberList{
		SCAllianceMemberList()
			:header(csprot::ALLIANCE_MEMBER_LIST_SC)
			, num(0)
		{
			memset(memeber_list, 0, sizeof(memeber_list));
		}
		prot::ProtHeader header;
		Int32 num;
		MemberInfo memeber_list[MAX_ALLIANCE_MEMBER_NUM];
	};

	// 军团成员职位变更
	struct CSAlliancePostChange{
		CSAlliancePostChange()
			:header(csprot::ALLIANCE_POST_CHANGE_CS)
			, role_id(0)
			, new_post(0)
		{
		}
		prot::ProtHeader header;
		RoleID role_id;
		Int32 new_post;			// 0:普通成员, 1:军团长, 2:副团长, 3:自定义职位1, 4:自定义职位2, 5:自定义职位3, 6:自定义职位4
	};

	// 军团成员职位变更返回
	struct SCAlliancePostChange{
		SCAlliancePostChange()
			:header(csprot::ALLIANCE_POST_CHANGE_SC)
			, ret(0)
		{}
		prot::ProtHeader header;
		Int32 ret;				// 1:职位任命成功, 0:任命失败(数量达到上限)
	};

	// 军团成员职位变更通知
	struct SCAlliancePostChangeNote{
		SCAlliancePostChangeNote()
			:header(csprot::ALLIANCE_POST_CHANGE_NOTE_SC)
			, role_id(0)
			, origin_post(0)
			, new_post(0)
		{}
		prot::ProtHeader header;
		RoleID role_id;		// 成员角色id
		Int16 origin_post;	// 旧职位
		Int16 new_post;		// 新职位
	};

	// 军团自定义职位设置
	struct CSAlliancePostSet{
		CSAlliancePostSet()
			:header(csprot::ALLIANCE_POST_SET_CS)
			, custom_post(0)
		{
			memset(post_name, 0, sizeof(post_name));
		}
		prot::ProtHeader header;
		Int32 custom_post;		// 自定义职位
		char post_name[MAX_32_CHAR_LEN];		// 自定义职位名称
	};

	// 军团自定义职位设置返回 
	struct SCAlliancePostSet{
		SCAlliancePostSet()
			:header(csprot::ALLIANCE_POST_SET_SC)
			, custom_post(0)
			, ret(0)
		{}
		prot::ProtHeader header;
		Int32 custom_post;		// 自定义职位
		Int32 ret;				// 1:设置成功, 2:设置失败
	};

	// 入团条件设置
	struct CSAllianceConditionSet{
		CSAllianceConditionSet()
			:header(csprot::ALLIANCE_CONDITION_SET_CS)
			, level_limit(0)
			, attack_power_limit(0)
		{
			memset(alliance_declaration, 0, sizeof(alliance_declaration));
		}
		prot::ProtHeader header;
		Int32 level_limit;			// 等级限制
		Int32 attack_power_limit;	// 战力限制
		char alliance_declaration[MAX_128_CHAR_LEN]; // 军团宣言
	};

	// 入团条件设置返回
	struct SCAllianceConditionSet{
		SCAllianceConditionSet()
			:header(csprot::ALLIANCE_CONDITION_SET_SC)
			, ret(0)
		{}
		prot::ProtHeader header;
		Int32 ret;					// 1:设置成功, 2:设置失败
	};

	// 军团招募
	struct CSAllianceRecruit {
		CSAllianceRecruit()
			:header(ALLIANCE_RECRUIT_CS)
		{}
		prot::ProtHeader header;
	};

	/// 军团大厅协议分界 ///

	// 科技大厅信息请求
	struct CSAllianceTechHallInfo{
		CSAllianceTechHallInfo() :header(csprot::ALLIANCE_TECH_HALL_INFO_CS){}
		prot::ProtHeader header;
	};

	struct TechInfoType{
		TechInfoType()
			:type(0)
			, level(0)
			, exp(0)
		{}
		Int16 type;				// 科技类型
		Int16 level;			// 科技等级
		Int32 exp;				// 科技经验
	};

	// 科技大厅信息返回
	struct SCAllianceTechHallInfo{
		SCAllianceTechHallInfo()
			:header(csprot::ALLIANCE_TECH_HALL_INFO_SC)
			, level(0)
			, schedule(0)
			, member_schedule(0)
			, num(0)
		{}
		prot::ProtHeader header;
		Int32 level;			// 大厅等级
		Int32 schedule;			// 总建设度
		Int32 member_schedule;	// 个人贡献
		Int32 num;
		TechInfoType tech_list[ALLIANCE_TECH_TYPE_COUNT];	// 军团科技列表
	};

	// 军团科技捐献
	struct CSAllianceTechDonate{
		CSAllianceTechDonate()
			:header(csprot::ALLIANCE_TECH_DONATE_CS)
			, type(0)
		{}
		prot::ProtHeader header;
		Int16 tech_type;		// 科技类型 
		Int16 type;				// 资源类型
	};

	// 军团科技捐献返回
	struct SCAllianceTechDonate{
		SCAllianceTechDonate()
			:header(csprot::ALLIANCE_TECH_DONATE_SC)
			, ret(0)
			, tech_type(0)
			, tech_level(0)
			, type(0)
			, exp(0)
		{}
		prot::ProtHeader header;
		Int16 ret;			// 1:捐献成功, 2:捐献失败(次数达到上限), 3:捐献失败(资源不足)
		Int16 tech_type;	// 科技类型
		Int16 tech_level;	// 科技等级
		Int16 type;			// 资源类型
		Int32 exp;			// 科技经验
	};

	/// 科技大厅分界 ///

	struct ItemRecord{
		ItemRecord()
			:item_id(0)
			, num(0)
		{}
		Int32 item_id;		// 物品id
		Int32 num;			// 数量
	};

	// 军团商店物品列表请求
	struct CSAllianceShopItemList{
		CSAllianceShopItemList()
			:header(csprot::ALLIANCE_SHOP_ITEM_LIST_CS)
			, type(0)
		{}
		prot::ProtHeader header;
		Int32 type;				// 商店类型 1:普通商店, 2:高级商店
	};

	// 普通商店物品列表
	struct SCAllianceCommonShopItemList{
		SCAllianceCommonShopItemList()
			:header(csprot::ALLIANCE_COMMON_SHOP_ITEM_LIST_SC)
			, num(0)
		{
			memset(item_list, 0, sizeof(item_list));
		}
		prot::ProtHeader header;
		Int32 num;				// 物品列表长度
		ItemRecord item_list[MAX_ALLIANCE_SHOP_ITEM_NUM];
	};

	// 高级商店物品列表
	struct SCAllianceAdvanceShopItemList{
		SCAllianceAdvanceShopItemList()
			:header(csprot::ALLIANCE_ADVANCED_SHOP_ITEM_LIST_SC)
			, item_list_num(0)
			, order_list_num(0)
		{
			memset(item_list, 0, sizeof(item_list));
			memset(order_list, 0, sizeof(order_list));
		}
		prot::ProtHeader header;
		Int32 item_list_num;			// 物品列表长度
		ItemRecord item_list[MAX_ALLIANCE_SHOP_ITEM_NUM];	// 物品列表
		Int32 order_list_num;			// 玩家购买记录列表长度
		ItemRecord order_list[MAX_ALLIANCE_SHOP_ITEM_NUM];	// 玩家购买记录
	};

	// 军团商店物品兑换请求
	struct CSAllianceItemOrder{
		CSAllianceItemOrder() :header(csprot::ALLIANCE_ITEM_ORDER_CS)
		{}
		prot::ProtHeader header;
		Int32 type;				// 商店类型 1:普通商店, 2:高级商店
		Int32 item_id;			// 物品id
		Int32 num;				// 物品数量
	};

	// 军团商店物品兑换返回
	struct SCAllianceItemOrder{
		SCAllianceItemOrder() :header(csprot::ALLIANCE_ITEM_ORDER_SC){}
		prot::ProtHeader header;
		Int32 ret;				// 兑换结果 1:兑换成功, 2:兑换失败(积分不足), 3:兑换失败(次数不够)
		Int32 type;				// 商店类型 1:普通商店, 2:高级商店
		Int32 item_id;			// 物品id
		Int32 num;				// 物品数量
	};

	/// 军团商店分界 ///

	// 测试用
	/*
	struct CSAllianceInfoNote{
	CSAllianceInfoNote() :header(csprot::ALLIANCE_INFO_STATION_NOTE_CS){}
	prot::ProtHeader header;
	};
	*/

	// 情报列表请求
	struct CSAllianceInfoStationLoginSync{
		CSAllianceInfoStationLoginSync() :header(csprot::ALLIANCE_INFO_STATION_LOGIN_SYNC){}
		prot::ProtHeader header;
	};

	struct AllianceInfoType {
		AllianceInfoType() :time(0)
		{
			memset(info, 0, sizeof(info));
		}
		Int64 time;								// 发生时间
		char info[MAX_128_CHAR_LEN];			// 情报
	};

	// 情报更新
	struct SCAllianceInfoNote{
		SCAllianceInfoNote() :header(csprot::ALLIANCE_INFO_STATION_NOTE_SC)
			, type(0)
			, num(0)
		{
		}
		prot::ProtHeader header;
		Int32 type;						// 0:增加, 1:同步所有情报
		Int32 num;
		AllianceInfoType info_list[MAX_ALLIANCE_INFO_NUM];
	};

	/// 情报分界 ///

	// 福利院基本信息请求
	struct CSAllianceWelfareInfo{
		CSAllianceWelfareInfo() :header(csprot::ALLIANCE_WELFARE_INFO_CS){}
		prot::ProtHeader header;
	};

	// 福利院基本信息返回
	struct SCAllianceWelfareInfo{
		SCAllianceWelfareInfo()
			:header(csprot::ALLIANCE_WELFARE_INFO_SC)
			, level(0)
			, donate_num(0)
		{}
		prot::ProtHeader header;
		Int32 level;		// 福利院等级
		Int32 donate_num;	// 个人贡献
	};

	// 日常福利领取请求
	struct CSAllianceDailyWelfare{
		CSAllianceDailyWelfare() :header(csprot::ALLIANCE_DAILY_WELFARE_CS){}
		prot::ProtHeader header;
	};

	// 日常福利领取返回
	struct SCAllianceDailyWelfare{
		SCAllianceDailyWelfare()
			:header(csprot::ALLIANCE_DAILY_WELFARE_SC)
			, ret(0)
		{}
		prot::ProtHeader header;
		Int16 ret;				// 领取状态 1:领取成功, 2:领取失败(已领取过), 3:领取失败(声望不足)
		Int16 level;			// 福利等级
	};

	// 战事福利列表请求
	struct CSAllianceWarWelfareList {
		CSAllianceWarWelfareList()
			:header(ALLIANCE_WAR_WELFARE_ITEM_LIST_CS)
		{}

		prot::ProtHeader header;
	};

	// 战事福利列表返回
	struct SCAllianceWarWelfareList {
		SCAllianceWarWelfareList()
			:header(ALLIANCE_WAR_WELFARE_ITEM_LIST_SC)
			, num(0)
		{}

		prot::ProtHeader header;
		Int32 num;
		ItemParam item_list[MAX_ALLIANCE_WELFARE_ITEM_KIND_NUM];
	};

	// 战事福利处理请求
	struct CSAllianceWarWelfareHandle{
		CSAllianceWarWelfareHandle()
			:header(csprot::ALLIANCE_WAR_WELFARE_HANDLE_CS)
			, role_id(0)
			, item_id(0)
			, num(0)
		{}
		prot::ProtHeader header;
		RoleID role_id;		// 分配成员角色id
		Int32 item_id;		// 物品id
		Int32 num;			// 物品数量
	};

	// 战事福利处理返回
	struct SCAllianceWarWelfareHandleReply{
		SCAllianceWarWelfareHandleReply() :header(csprot::ALLAINCE_WAR_WELFARE_HANDLE_REPLY_SC), ret(0){}
		prot::ProtHeader header;
		Int32 ret;		// 1分配成功, 2:分配失败(不是军团长), 3:分配失败(道具不足)
	};

	// 领取活跃福利
	struct CSAllianceActiveWelfare{
		CSAllianceActiveWelfare() :header(csprot::ALLIANCE_ACTIVE_WELFAER_CS){}
		prot::ProtHeader header;
	};

	// 活跃福利领取返回
	struct SCAllianceActiveWelfare{
		SCAllianceActiveWelfare() :header(csprot::ALLIANCE_ACTIVE_WELFARE_SC), ret(0){}
		prot::ProtHeader header;
		Int32 ret;		// 1:领取成功, 2:领取失败(没有活跃奖励)
	};

	// 活跃福利状态请求
	struct CSAllianceAcitveWelfareState{
		CSAllianceAcitveWelfareState() :header(csprot::ALLIANCE_ACTIVE_WELFARE_SYNC_CS){}
		prot::ProtHeader header;
	};

	// 活跃福利状态返回
	struct SCAllianceActiveWelfareState{
		SCAllianceActiveWelfareState()
			:header(csprot::ALLIANCE_ACTIVE_WELFARE_SYNC_SC)
			, level(0)
			, schedule(0)
		{
			memset(avail_resource_list, 0, sizeof(avail_resource_list));
			memset(got_resource_list, 0, sizeof(got_resource_list));
		}
		prot::ProtHeader header;
		Int32 level;											// 活跃等级
		Int32 schedule;											// 活跃度
		Int32 avail_resource_list[RESOURCE_TYPE_COUNT];			// 可领取活跃福利
		Int32 got_resource_list[RESOURCE_TYPE_COUNT];			// 已领取活跃福利
	};

	// 活跃排名请求
	struct CSAllianceActiveRank{
		CSAllianceActiveRank() :header(csprot::ALLIANCE_ACTIVE_RANK_CS){}
		prot::ProtHeader header;
	};

	struct ActiveRecord{
		ActiveRecord()
			:role_id(0)
			, schedule(0)
			, post(0)
		{
			memset(name, 0, sizeof(name));
		}
		RoleID role_id;
		Int32 schedule;			// 活跃度
		Int32 post;				// 职位
		RoleName name;			// 玩家名
	};

	// 活跃排名返回
	struct SCAllianceActiveRank{
		SCAllianceActiveRank()
			:header(csprot::ALLIANCE_ACTIVE_RANK_SC)
			, num(0)
		{
			//memset(rank_list, 0, sizeof(rank_list));
		}
		prot::ProtHeader header;
		Int32 num;						// 列表长度
		ActiveRecord rank_list[MAX_ALLIANCE_MEMBER_NUM];	// 排行列表
	};

	// 提升福利院等级请求
	struct CSAllianceWelfareHallLeveUp{
		CSAllianceWelfareHallLeveUp() :header(csprot::ALLIANCE_WELFAER_LEVEL_UP_CS){}
		prot::ProtHeader header;
	};

	// 提升福利院等级返回
	struct SCAllianceWelfareHallLevelUp{
		SCAllianceWelfareHallLevelUp() :header(csprot::ALLIANCE_WELFAER_LEVEL_UP_SC), ret(0){}
		prot::ProtHeader header;
		Int32 ret;			// 1:升级成功, 2:升级失败(建设度不足), 3:升级失败(已是最高等级)
	};

	/// 福利院分界 ///

	// 试练场列表请求
	struct CSAllianceChapterList{
		CSAllianceChapterList() :header(csprot::ALLIANCE_CHAPTER_LIST_CS){}
			prot::ProtHeader header;
	};

	struct SCAllianceChapterList{
		SCAllianceChapterList()
			:header(csprot::ALLIANCE_CHAPTER_LIST_SC)
			, num(0)
		{
			memset(state_list, 0, sizeof(state_list));
		}
		prot::ProtHeader header;
		Int32 num;										// 数组长度
		Int8 state_list[MAX_ALLIANCE_CHAPTER_LEVEL];	// 试练场状态列表
		Int8 reward_state[MAX_ALLIANCE_CHAPTER_LEVEL];	// 奖励领取列表
	};

	// 试练场挑战
	struct CSAllianceChapterChallenge{
		CSAllianceChapterChallenge()
			:header(csprot::ALLIANCE_CHAPTER_CHALLENGE_CS)
			, chapter_id(0)
		{
			memset(format.general_format, 0, sizeof format.general_format);
		}
		prot::ProtHeader header;
		Int32 chapter_id;			// 副本id
		ArmyFormatData  format;
	};

	// 试练场挑战返回
	struct SCAllianceChapterChallenge{
		SCAllianceChapterChallenge()
			:header(csprot::ALLIANCE_CHAPTER_CHALLENGE_SC)
			, chapter_id(0)
		{}
		prot::ProtHeader header;
		Int32 ret;				// 1: 挑战成功, 2:挑战失败, 3:挑战次数用完, 4:当前关卡不可挑战
		Int32 chapter_id;		// 副本id
	};

	// 试练场奖励领取
	struct CSAllianceChapterRewardFetch{
		CSAllianceChapterRewardFetch()
			:header(csprot::ALLIANCE_CHAPTER_REWARD_FETCH_CS)
			, chapter_id(0)
		{}

		enum
		{
			REWARD_FETCH_STATE_CAN_NOT_INVALID,
			REWARD_FETCH_STATE_CAN_NOT_FETCH,
			REWARD_FETCH_STATE_CAN_FETCH,
			REWARD_FETCH_STATE_FETCHED,
			REWARD_FETCH_STATE_FETCH_FAILED,
		};

		prot::ProtHeader header;
		Int32 chapter_id;		// 副本id
	};

	// 试练场奖励领取返回
	struct SCAllianceChapterRewardFetch{
		SCAllianceChapterRewardFetch()
			:header(csprot::ALLIANCE_CHAPTER_REWARD_FETCH_SC)
			, chapter_id(0)
		{}
		prot::ProtHeader header;
		Int32 ret;				// 1:领取成功, 2:领取失败(不可领取), 3:领取失败(已领取过), 4:领取失败(其它原因)
		Int32 chapter_id;		// 副本id
	};

// 试练场协议分界

	// 参加活动军团列表
	struct CSActiviyAllianceList{
		CSActiviyAllianceList()
			:header(csprot::ALLIANCE_ACTIVITY_ALLIANCE_LIST_CS)
		{}
		prot::ProtHeader header;
	};

	// 参加活动军团列表返回
	struct SCActiviyAllianceList{
		SCActiviyAllianceList()
			:header(csprot::ALLIANCE_ACTIVITY_ALLIANCE_LIST_SC)
			, num(0)
		{}
		prot::ProtHeader header;
		Int32 num;
		SingleAllianceInfo info[MAX_ALLIANCE_NUM];
	};

	// 群雄报名
	struct CSAllianceActivityApply{
		CSAllianceActivityApply()
			:header(csprot::ALLIANCE_ACTIVITY_APPLY_CS)
		{}
		prot::ProtHeader header;
		ArmyFormatData army_format;
	};

	struct SCAllianceActivityApply{
		SCAllianceActivityApply()
			:header(csprot::ALLIANCE_ACTIVITY_APPLY_SC)
		{}
		prot::ProtHeader header;
		Int32 ret;		// 1:申请成功, 2:活动已报名, 3:活动中
	};

	// 活动播报
	struct ScAllianceActivityNote{
		ScAllianceActivityNote()
			: header(csprot::ALLIANCE_ACTIVITY_NOTE_SC)
			, type(0)
			, time(0)
			, alliance_order(0)
			, win_num(0)
			, win_side(0)
		{}
		prot::ProtHeader header;
		Int64 time;						// 战斗发生时间
		Int16 alliance_order;			// 军团排名
		Int16 win_num;					// 胜利方连胜次数
		Int8 type;						// 1:玩家被淘汰, 2:排名产生
		Int8 win_side;					// 1:攻方胜, 2:守方胜
		Int16 reserved;						
		RoleID attacker_role_id;		// 攻方玩家id
		RoleName attacker;				// 攻方玩家名称
		RoleName attacker_alliance;		// 攻方军团名称
		RoleID defender_role_id;				// 攻方玩家id
		RoleName defender;				// 守方玩家名称
		RoleName defender_alliance;		// 守方军团名称
	};

	struct SCAllianceActivityStart{
		SCAllianceActivityStart():header(csprot::ALLIANCE_ACTIVITY_START_SC){}
		prot::ProtHeader header;
	};

	struct SCAllianceActivityEnd{
		SCAllianceActivityEnd() :header(csprot::ALLIANCE_ACTIVITY_END_SC){}
		prot::ProtHeader header;
	};

	struct CSAllianceTest{
		CSAllianceTest() :header(csprot::ALLIANCE_TEST){}
		prot::ProtHeader header;
		Int32 param1;
		Int32 param2;
		Int32 param3;
		Int32 param4;
		Int32 param5;
	};
}

#pragma pack(pop)

#endif