﻿#ifndef JXS_SKILLDEF_H_
#define JXS_SKILLDEF_H_
#include "../../include/basic_type_def.h"


enum SkillType
{
	SKILL_TYPE_INVALID,

	SKILL_TYPE_END,
	SKILL_TYPE_COUNT = SKILL_TYPE_END - 1,
};

//激活条件
enum SkillCond
{
	SKILL_COND_INVALID,
	SKILL_COND_DEFAULT,					//默认
	SKILL_COND_SINGLE_TARGET,			//目标为单个时
	SKILL_COND_KILL_UNIT,				//击杀一个敌方部队后
	SKILL_COND_BE_AOE_HURT,				//受范围伤害时
	SKILL_COND_NO_BLOW,					//不产生暴击时
	SKILL_COND_SOLDIER_NUM_LESS,		//敌方兵数大于自己时
	SKILL_COND_TARGET_LESS,				//目标数目小于一个数时

	SKILL_COND_END,
	SKILL_COND_NUM = SKILL_COND_END - 1,
};

enum EffectTiming
{
	EFFECT_TIMING_INVALID,
	EFFECT_TIMING_DEFAULT,
	EFFECT_TIMING_BEFOR_SELECT_TARGET,	//目标选择前
	EFFECT_TIMING_AFTER_SELECT_TARGET,	//目标选择后
	EFFECT_TIMING_AFTER_CAL_BLOW,		//计算暴击后
	EFFECT_TIMING_BEFOR_BE_HUT,			//被击计算伤害前
	EFFECT_TIMING_AFTER_ATTACK_ALL,		//攻击完成后

	EFFECT_TIMING_END,
	EFFECT_TIMING_COUNT = EFFECT_TIMING_END - 1,
};

enum SkillEffectType
{
	SKILL_EFFECT_INVALID,
	SKILL_EFFECT_SUBTRACT_ARMY_ATTR,					//抵消部队属性
	SKILL_EFFECT_SETOFF_SHEMMER_SKILL,					//抵消谋士技
	SKILL_EFFECT_SETOFF_OFFICER_SKILL,					//抵消武将技
	SKILL_EFFECT_CHANGE_SOLDIER_ATTR,					//改变全体士兵属性
	SKILL_EFFECT_CHANGE_ARMY_PROFICIENCY,				//改变部队相性
	SKILL_EFFECT_SETOFF_SOLDIER_AURA,					//清除士兵光环
	SKILL_EFFECT_SETOFF_SOLDIER_RESTRICTION,			//清除士兵克制
	SKILL_EFFECT_CHANGE_SOLDIER_SKILL_PARAM,			//改变士兵技能参数
	SKILL_EFFECT_SUBTRACT_OFFICER_SKILL_HURT,			//减少武将伤害
	SKILL_EFFECT_ADD_ARMY_SHIELD_VALUE,					//增加护盾值
	SKILL_EFFECT_ADD_ARMY_SHIELD_RATIO,					//增加护盾比率
	SKILL_EFFECT_OFFICER_ATTACK,						//武将单人技
	SKILL_EFFECT_OFFICER_COMMAND,						//武将指挥技
	SKILL_EFFECT_CHANGE_CALCUL_SOLDIER_ATTR_VALUE,		//改变结算时士兵的属性值	
	SKILL_EFFECT_CHANGE_CALCUL_SOLDIER_ATTR_RATIO,		//改变结算时士兵的属性比率
	SKILL_EFFECT_ADD_ATTACK,							//增加一次攻击
	SKILL_EFFECT_CHANGE_ATTACK_MODE,					//改变攻击方式
	SKILL_EFFECT_CHANGE_NEXT_ATTACK_MODE,				//改变第二次攻击的攻击方式
	SKILL_EFFECT_CHANGE_NEXT_CALCUL_SOLDIER_ATTR_RATIO,	//改变第二次的参数


	SKILL_EFFECT_END,
	SKILL_EFFECT_COUNT = SKILL_EFFECT_END -1,
};

enum EffectTarget
{
	EFFECT_TARGET_INVALID,
	EFFECT_TARGET_DEFAULT,
	EFFECT_TARGET_ARMY,
	EFFECT_TARGET_SOLDIER,

	EFFECT_TARGET_END,
	EFFECT_TARGET_COUNT = EFFECT_TARGET_END -1,
};

enum SkillClassify
{
	SKILL_CLASSIFY_INVALID,
	SKILL_CLASSIFY_SOLDIDER_BASE,
	SKILL_CLASSIFY_SOLDIER_OFFICER,
	SKILL_CLASSIFY_SOLDIER_SCHEMER,
	SKILL_CLASSIFY_SOLDIER_BEAUTY,
	SKILL_CLASSIFY_OFFICER,
	SKILL_CLASSIFY_SCHEMER,
	SKILL_CLASSIFY_BEAUTY,

	SKILL_CLASSIFY_END,
	SKILL_CLASSIFY_COUNT = SKILL_CLASSIFY_END - 1,
};


#endif

