﻿#ifndef JXS_BATTLEDEF_H_
#define JXS_BATTLEDEF_H_
//author
//战斗定义

#include "../../include/basic_type_def.h"

//战斗单元基础属性定义
//部分属性是以万分比的分子值记录
enum NorAttr
{
	NOR_ATTR_INVALID,
	NOR_ATTR_HP,				//血、生命
	NOR_ATTR_AP,				//攻击
	NOR_ATTR_HIT,				//命中（万分比）
	NOR_ATTR_DODGE,				//闪避（万分比）
	NOR_ATTR_BLOW,				//暴击（万分比）
	NOR_ATTR_DEBLOW,			//抗暴（万分比）
	NOR_ATTR_SPUR,				//穿刺
	NOR_ATTR_GUARD,				//防护
	NOR_ATTR_BLURST,			//爆裂（万分比）
	NOR_ATTR_DEBLURST,			//坚韧（万分比）
	NOR_ATTR_HURT,				//伤害
	NOR_ATTR_OFFENSIVE,			//先手
	NOR_ATTR_SUB_HURT,			//减伤
	
	NOR_ATTR_END,
	NOR_ATTR_COUNT = NOR_ATTR_END -1,
	
};

const Int32 MAX_NOR_ATTR_NUM = 20;
//计算属性
enum CalAttr
{
	CALC_ATTR_INVALID = NOR_ATTR_INVALID,

	CALC_ATTR_BATTLE_VALUE_INVALID = NOR_ATTR_INVALID,
	CALC_ATTR_BATTLE_VALUE_END = NOR_ATTR_END,

	CALC_ATTR_BATTLE_RATIO_INVALID = NOR_ATTR_INVALID + MAX_NOR_ATTR_NUM,
	CALC_ATTR_BATTLE_RATIO_END = NOR_ATTR_END + MAX_NOR_ATTR_NUM,

	CALC_ATTR_SYS_VALUEINVALID = NOR_ATTR_INVALID + MAX_NOR_ATTR_NUM * 2,
	CALC_ATTR_SYS_VALUE_END = NOR_ATTR_END + MAX_NOR_ATTR_NUM * 2,

	CALC_ATTR_SYS_RATIO_INVALID = NOR_ATTR_INVALID + MAX_NOR_ATTR_NUM * 3,
	CALC_ATTR_SYS_RATIO_END = NOR_ATTR_END + MAX_NOR_ATTR_NUM * 3,
};


//属性分类
enum AttrAddMode
{
	ATTR_ADD_MODE_INVALID,
	ATTR_ADD_MODE_VALUE,		//增加值或万分比的分子
	ATTR_ADD_MODE_PERCENT,		//以比率增加

	ATTR_ADD_MODE_END,
};

//部队属性
enum ArmyAttr
{
	ARMY_ATTR_INVALID,
	ARMY_ATTR_LEADING_NUM,		//带兵数
	ARMY_ATTR_MOVING_SPEED,		//移动速度
	ARMY_ATTR_OVERLOAD,			//负重

	ARMY_ATTR_END,
	ARMY_ATTR_COUNT = ARMY_ATTR_END - 1,
};


////普通属性加成（玩家、科技、装备、武将通用)
//enum NorAttrAdd
//{
//	NOR_ATTR_INVALID,	
//	NOR_ATTR_HIT,			//命中
//	NOR_ATTR_DODGE,			//闪避
//	NOR_ATTR_BLOW,			//暴击
//	NOR_ATTR_DEBLOW,		//抗暴
//	NOR_ATTR_SPUR,			//穿刺
//	NOR_ATTR_GUARD,			//防护
//	NOR_ATTR_BLURST,		//爆裂
//	NOR_ATTR_DEBLURST,		//坚韧
//	NOR_ATTR_ADD_HURT,		//伤害加成
//	NOR_ATTR_SUBTRACT_HURT,	//减伤
//	NOR_ATTR_HP,			//生命
//	NOR_ATTR_AP,			//攻击
//	NOR_ATTR_AHEARD,		//先手值
//	NOR_ATTR_SHIELD_HP,		//盾兵生命
//	NOR_ATTR_SHIELD_AP,		//盾兵攻击
//	NOR_ATTR_LANCER_HP,		//枪兵生命
//	NOR_ATTR_LANCER_AP,		//枪兵攻击
//	NOR_ATTR_CAVALRY_HP,	//骑兵生命
//	NOR_ATTR_CAVALRY_AP,	//骑兵攻击
//	NOR_ATTR_ARCHER_HP,		//弓兵生命
//	NOR_ATTR_ARCHER_AP,		//弓兵攻击
//
//	NOR_ATTR_END,
//	NOR_ATTR_COUNT = NOR_ATTR_END - 1,
//
//};

//士兵基本属性
enum SoldierNorAttr
{
	SOLDIER_NOR_ATTR_INVALID,		//
	SOLDIER_NOR_ATTR_AP,			//攻击
	SOLDIER_NOR_ATTR_HP,			//生命
	SOLDIER_NOR_ATTR_WORKLOAD,		//负重	
	SOLDIER_NOR_ATTR_SPEED,			//移动
	SOLDIER_NOR_ATTR_AHEARD,		//先手

	SOLDIER_NOR_ATTR_END,
	SOLDIER_NOR_ATTR_COUNT = SOLDIER_NOR_ATTR_END - 1,
};

//攻击方式
enum AttackMode
{
	ATTACK_MODE_INVALID,
	ATTACK_MODE_ROW,			//横排
	ATTACK_MODE_ONE,			//单个
	ATTACK_MODE_COLLUMN,		//一列
	ATTACK_MODE_ALL,			//全体
	ATTACK_MODE_RANDOM,			//随机

	ATTACK_MODE_END,
};

//士兵类型
enum SoldierType
{
	SOLDIER_TYPE_INVALID,

	SOLDIER_TYPE_SHIELD,			//盾兵
	SOLDIER_TYPE_CAVALRY,			//骑兵
	SOLDIER_TYPE_LANCER,			//枪兵
	SOLDIER_TYPE_ARCHER,			//弓兵

	SOLDIER_TYPE_END,
	SOLDIER_TYPE_COUNT = SOLDIER_TYPE_END -1,
};

//士兵分类
enum SoldierClass
{
	SOLDIER_CLASS_INVALID,
	SOLDIER_CLASS_LIGHT,		//轻型
	SOLDIER_CLASS_NORMAL,		//普通
	SOLDIER_CLASS_HEAVY,		//重型

	SOLDIER_CLASS_END,
	SOLDIER_CLASS_COUNT = SOLDIER_CLASS_END - 1,
};

//战场类型
enum BattleType
{
	BATTLE_TYPE_INVALID,
	BATTLE_TYPE_CHAPTER,		//副本
	BATTLE_TYPE_INTERCEPT,
	BATTLE_TYPE_FAMOUSCITY,
	BATTLE_TYPE_CITY,
	BATTLE_TYPE_ALLIANCE,
	BATTLE_TYPE_ALLIANCE_CHAPTER,

	BATTLE_TYPE_END,
};

enum ArmySide
{
	ARMY_SIDE_INVALID,
	ARMY_SIDE_ATTACKER,
	ARMY_SIDE_DEFFENDER,

	ARMY_SIDE_END,
	ARMY_SIDE_COUNT = ARMY_SIDE_END - 1,

};

enum BattleStat
{
	BATTLE_STATE_INVALID,
	BATTLE_STATE_RUNNING,
	BATTLE_STATE_END,
};

//武将类型
enum GeneralType
{
	GENERAL_TYPE_INVALID,
	GENERAL_TYPE_OFFICER,			//武将
	GENERAL_TYPE_SCHEMER,			//谋士
	GENERAL_TYPE_BEAUTY,			//美人

	GENERAL_TYPE_END,
	GENERAL_TYPE_COUNT = GENERAL_TYPE_END -1,
};

//武将属性类型
enum GeneralAttrType
{
	GENERAL_ATTR_TYPE_INVALID,
	GENERAL_ATTR_TYPE_TONG,						// 统御
	GENERAL_ATTR_TYPE_YONG,						// 勇
	GENERAL_ATTR_TYPE_ZHI,						// 智
	GENERAL_ATTR_TYPE_MEI,						// 魅

	GENERAL_ATTR_TYPE_END,
	GENERAL_ATTR_TYPE_COUNT = GENERAL_ATTR_TYPE_END - 1,
};

//
enum BuffOverlapMode
{
	BUFF_OVERLAP_MODE_INVALID,
	BUFF_OVERLAP_MODE_COEXIST,		//共存
	BUFF_OVERLAP_MODE_ADD,			//叠加
	BUFF_OVERLAP_MODE_REPLACE,		//替换
};

//buff效果
enum BuffEffectSide
{
	BUFF_EFFECT_SIDE_INVALID,
	BUFF_EFFECT_SIDE_ATTACKER,		//我方
	BUFF_EFFECT_SIDE_DEFENDER,		//敌方
	BUFF_EFFECT_SIDE_SELF,			//自己

	BUFF_EFFECT_SIDE_END,
};


#endif