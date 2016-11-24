#ifndef JXS_CSPRODUCEITEMADDPROT_H_
#define JXS_CSPRODUCEITEMADDPROT_H_

//开采工坊，制作增益道具协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
//#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"
//#include "../../game/structs/army_format_data.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//增益道具制作
	struct CSProduceItemAdd
	{
		CSProduceItemAdd()
			:header(PRODUCE_ITEM_ADD_CS)
			, produce_id(0)
			, item_num(0)
		{

		}
		prot::ProtHeader header;
		Int32 produce_id;	//制作ID
		Int32 item_num;		//制作数量
	};

	struct SCProduceItemAddRet
	{
		SCProduceItemAddRet()
			:header(PRODUCE_ITEM_ADD_RET_SC)
			, res_code(K_EXCEPTION)
			, produce_id(0)
		{

		}
		prot::ProtHeader header;
		Int32 res_code;			//0 = 成功， -1=失败	
		Int32 produce_id;		//制作ID
	};

	//加速生产,扣除金币
	struct CSProduceItemAddQuick
	{
		CSProduceItemAddQuick()
			:header(PRODUCE_ITEM_ADD_QUICK_CS)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;		//制作进度ID
	};

	struct SCProduceItemAddQuickRet
	{
		SCProduceItemAddQuickRet()
			:header(PRODUCE_ITEM_ADD_QUICK_RET_SC)
			, res_code(K_EXCEPTION)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int32 res_code; //0 = 成功， -1=失败	
		Int64 proc_id;		//制作进度ID
	};

	//取消道具制作
	struct CSProduceItemAddCancel
	{
		CSProduceItemAddCancel()
			:header(PRODUCE_ITEM_ADD_CANCEL_CS)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;		//制作进度ID
	};

	struct SCProduceItemAddCancelRet
	{
		SCProduceItemAddCancelRet()
			:header(PRODUCE_ITEM_ADD_CANCEL_RET_SC)
			, res_code(K_EXCEPTION)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int32 res_code;			//0 = 成功， -1=失败	
		Int64 proc_id;			//制作ID
	};

}
#pragma pack(pop)

#endif
