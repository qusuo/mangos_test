#ifndef JXS_CSITEMPROT_H_
#define JXS_CSITEMPROT_H_
//物品背包协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../game/structs/item_data.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	// 获取背包信息
	struct CSKnapsackGetInfo
	{
		CSKnapsackGetInfo()
			:header(KNAPSACK_GET_INFO_CS)
		{

		}
		prot::ProtHeader header;
	};
	
	struct ProtItemInfo
	{	
		ProtItemInfo()
			:grid_index(0)
			,item_id(0)
			, item_num(0)
		{

		}
		Int32 grid_index;		//背包格子索引
		Int32 item_id;			//物品id
		Int32 item_num;			//物品数量
		EquipAttr equip_attr;
	};

	struct SCKnapsackGetInfoRet
	{
		SCKnapsackGetInfoRet()
			:header(KNAPSACK_GET_INFO_RET_SC)
			, grid_num(0)
		{
			//memset(item_list, 0, sizeof(item_list));
		}
		prot::ProtHeader header;
		Int32 grid_num;
		ProtItemInfo item_list[MAX_KNAPSACK_GRID_NUM];
	};

	// 返回背包物品数量
	struct SCKnapsackCountRet
	{
		SCKnapsackCountRet()
			:header(KNAPSACK_COUNT_SC)
			, knapsack_item_num(0)
		{

		}
		prot::ProtHeader header;
		Int32 knapsack_item_num;					//背包全部物品的数量
	};
	
	//背包格子信息变更
	struct SCKnapsackGridInfo
	{
		enum ChangeType
		{
			CHANGE_TYPE_INVALID,
			CHANGE_TYPE_DELETE,
			CHANGE_TYPE_ADD,
			CHANGE_TYPE_UPDATE,
		};
		SCKnapsackGridInfo()
			:header(KNAPSACK_GRID_INFO_SC)
			, change_type(0)
		{

		}
		prot::ProtHeader header;
		Int32 change_type ; //1 = 删除， 2 = 增加， 3 = 更新
		ProtItemInfo info;
	};

	//使用物品
	struct CSKnapsackUseItem
	{
		CSKnapsackUseItem()
			:header(KNAPSACK_USE_ITEM_CS)
			, grid_index(-1)
			, use_num(0)
			, type(0)
			, store_index(0)
		{

		}
		prot::ProtHeader header;
		Int32 grid_index;
		Int32 use_num;
		Int32 type;				//	1:角色使用, 2:武将使用
		Int32 store_index;				// type==1:角色id，type==2:武将id
	};

	//使用物品返回
	struct SCKnapsackUseItemRet
	{
		enum UseRes
		{
			USE_RES_INVALID,
			USE_RES_SUCCEED,
			USE_RES_EXCEPTION,
		};
		SCKnapsackUseItemRet()
			:header(KNAPSACK_USE_ITEM_RET_SC)
			, res_code(USE_RES_INVALID)
			, grid_index(USE_RES_INVALID)
			, item_id(0)
			, use_num(0)
			, type(0)
			, store_index(0)
		{

		}
		prot::ProtHeader header;
		Int32 res_code;   //
		Int32 grid_index;
		Int32 item_id;
		Int32 use_num;
		Int32 type;				//	1:角色使用, 2:武将使用
		Int32 store_index;		// type==1:角色id，type==2:武将store_index
	};

	//物品出售
	struct CSKnapsackSaleItem
	{
		CSKnapsackSaleItem()
			:header(KNAPSACK_SALE_ITEM_CS)
			, grid_index(0)
			, use_num(0)
		{

		}
		prot::ProtHeader header;
		Int32 grid_index;
		Int32 use_num;
	};

	//物品出售返回
	struct CSKnapsackSaleItemRet
	{
		CSKnapsackSaleItemRet()
			:header(KNAPSACK_SALE_ITEM_RET_SC)
			, res_code(K_EXCEPTION)
			, grid_index(-1)
			, item_id(0)
			, use_num(0)
		{

		}
		prot::ProtHeader header;
		Int32 res_code;   //
		Int32 grid_index;
		Int32 item_id;
		Int32 use_num;
	};

	//购买背包格子
	struct CSKnapsackBuyGrid
	{
		CSKnapsackBuyGrid()
			:header(KNAPSACK_BUY_GRID_CS)
			, knap_type(0)
		{
		}

		prot::ProtHeader header;
		Int32 knap_type; //背包类型， 1=普通背包，2=军械背包，3=奇门遁甲背包
	};

	//发送购买背包格子次数
	struct SCKnapsackBuyGridTimes
	{
		SCKnapsackBuyGridTimes()
			:header(KNAPSACK_BUY_GRID_TIME_SC)
			, knap_type(0)
			, times(0)
		{
		}

		prot::ProtHeader header;
		Int16 knap_type;//背包类型， 1=普通背包，2=军械背包，3=奇门遁甲背包
		Int16 times;    //购买次数
	};
	

}
#pragma pack(pop)

#endif
