#ifndef JXS_CSSHOPPROT_H_
#define JXS_CSSHOPPROT_H_
//商场协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"
#include "../../include/value_def.h"
#include "../../../gamelogicserver/gamelogicserver/config/shop_config.h"


#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//购买商品 
	struct CSShopBuyGoods
	{
		CSShopBuyGoods()
			:header(SHOP_BUY_GOODS_CS)
			, goods_id(0)
			, buy_num(0)
		{

		}
		prot::ProtHeader header;
		int32 goods_id;
		int32 buy_num;
	};

	//购买商品返回
	struct SCShopBuyGoodsRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_FAIL = -1,
		};
		SCShopBuyGoodsRet()
			:header(SHOP_BUY_GOODS_RET_SC)
			, res_code(RESCODE_FAIL)
			, goods_id(0)
			, buy_num(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
		int32 goods_id;
		int32 buy_num;
	};

	//限购信息，金币商城没有限购
	struct SCShopDayCountInfo
	{
		SCShopDayCountInfo()
			:header(SHOP_BUY_DAYCOUNT_INFO_SC)
			, shop_type(SHOP_TYPE_ORGAN)
			, limit_type(0)
		{
			memset(day_count_data, 0, sizeof(day_count_data));
		}
		prot::ProtHeader	header;
		Int16 shop_type;			//商城类型
		Int16 limit_type;			//限购类型，1=个人、2=全服
		Int16 day_count_data[MAX_POS_GOOD_LEN];
	};

	struct SCShopDayCountInfoChangedNotice
	{
		SCShopDayCountInfoChangedNotice()
			:header(SHOP_BUY_DAYCOUNT_CHANGE_SC)
			, shop_type(SHOP_TYPE_ORGAN)
			, day_count_id(0)
			, day_count_data(0)
		{
		}
		prot::ProtHeader header;
		Int16		shop_type;			//商城类型
		Int16		day_count_id;
		Int16		day_count_data;
	};
}
#pragma pack(pop)

#endif
