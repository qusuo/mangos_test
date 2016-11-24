#ifndef JXS_CSBUILDINGPROT_H_
#define JXS_CSBUILDINGPROT_H_
//进入游戏时初始化协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//事件类型
	enum EventType
	{
		EVENT_TYPE_INVALID,
		EVENT_TYPE_BUILDING_UPGRADED,	//建筑已升级事件
		EVENT_TYPE_RECRUIT_SOLID,		//士兵已招募事件
		EVENT_TYPE_ITEM_ADD_PRODUCE,	//增益道具已生成事件
		EVENT_TYPE_SCHEME_DEVELOP,		//计谋研发已生成事件
		EVENT_TYPE_BUILDING_CONSTRUCT,	//建筑已建造事件
		EVENT_TYPE_ITEM_USED_FINISH,	//增益道具使用时间已过

		EVENT_TYPE_END,
	};

	// 主要用于客户端判断事件类型
	struct SCBaseEvent
	{
		SCBaseEvent()
			:header(EVENT_NOTICE_SC)
			, event_type(EVENT_TYPE_INVALID)
			, reserve(0)
		{

		}
		prot::ProtHeader header;
		Int16 event_type;			// 事件类型
		Int16 reserve;
	};

	//建筑完成升级事件
	struct SCBuildingEvent
	{
		SCBuildingEvent()
			:header(EVENT_NOTICE_SC)
			, event_type(EVENT_TYPE_BUILDING_UPGRADED)
			, building_id(0)
			, level(0)
		{

		}
		prot::ProtHeader header;
		Int16 event_type;			// 事件类型
		Int16 building_id;			// 升级建筑id
		Int16 level;				// 当前等级（已升到的等级）
	};

	//工坊完成升级事件
	struct SCItemAddEvent
	{
		SCItemAddEvent()
			:header(EVENT_NOTICE_SC)
			, event_type(EVENT_TYPE_ITEM_ADD_PRODUCE)
			, item_id(0)
			, item_num(0)
		{

		}
		prot::ProtHeader header;
		Int16 event_type;			// 事件类型
		int32 item_id;				// 增益道具id
		Int16 item_num;				// 增益道具数量
	};

	//计略完成升级事件
	struct SCSchemeDevelopEvent
	{
		SCSchemeDevelopEvent()
			:header(EVENT_NOTICE_SC)
			, event_type(EVENT_TYPE_SCHEME_DEVELOP)
			, scheme_id(0)
			, level(0)
		{

		}
		prot::ProtHeader header;
		Int16 event_type;			// 事件类型
		Int16 scheme_id;			// 增益道具id
		Int16 level;				// 增益道具
	};

	//增益道具使用时间已过
	struct SCItemUsedEvent
	{
		SCItemUsedEvent()
			:header(EVENT_NOTICE_SC)
			, event_type(EVENT_TYPE_ITEM_USED_FINISH)
			, effect_id(0)
			, effect_param(0)
		{

		}
		prot::ProtHeader header;
		Int16 event_type;			// 事件类型
		int32 effect_id;			// 增益道具id
		int32 effect_param;			//效果万分比
		Int16 reserve;
	};

}
#pragma pack(pop)

#endif
