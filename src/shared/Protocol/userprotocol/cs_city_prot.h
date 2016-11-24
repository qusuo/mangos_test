#ifndef JXS_CITYPROT_H_
#define JXS_CITYPROT_H_
//野外城池协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/structs/building_data.h"
#include "../../game/include/game_value_def.h"
#include "../../game/structs/army_format_data.h"
#include "../../include/value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	struct CityInfo
	{
		CityInfo()
			:x(0)
			, y(0)
			, city_type(0)
			, gender(0)
			, avatar(0)
			, reserved1(0)			
			, military_rank(0)
			, military_position(0)
			, role_id(0)
			, role_level(0)		
			, hat(0)
			, alliance_id(0)
			, boom(0)
			, cur_boom(0)
			, city_shape(0)
			, is_guard(0)
			, reserved2(0)
		{
			memset(role_name, 0, sizeof(role_name));
			memset(alliance_name, 0, sizeof(alliance_name));
		}
		Int16 x;
		Int16 y;
		Int8 city_type;
		Int8 gender;	
		Int8 avatar;
		Int8 reserved1;
		Int16 military_rank; //军衔
		Int16 military_position; //编制职位
		RoleID role_id;
		RoleName role_name;
		Int16 role_level;
		Int16 hat;
		Int64 alliance_id;  //军团id
		char alliance_name[MAX_32_CHAR_LEN]; //军团名字
		int32 boom;
		int32 cur_boom;
		Int8 city_shape;  //城池外形
		Int8 is_guard;
		Int16 reserved2;	
	};

	struct CSCityGetBlockInfo
	{
		CSCityGetBlockInfo()
			:header(CITY_GET_BLOCK_INFO_CS)
			, block_pos(-1)
		{

		}
		prot::ProtHeader header;
		int32 block_pos; //区块位置， 从0开始
	};

	const int32 MAX_CITY_NUM_PER_BLOCK = 100;
	struct SCCityGetBlockInfoRet
	{
		SCCityGetBlockInfoRet()
			:header(CITY_GET_BLOCK_INFO_RET_SC)
			, city_num(0)
		{

		}
		prot::ProtHeader header;
		int32 city_num;
		CityInfo citiy_infos[MAX_CITY_NUM_PER_BLOCK];
	};
	
	//查询目标坐标信息
	struct CSCityQueryPosi
	{
		CSCityQueryPosi()
			:header(CITY_QUERY_POSI_CS)
			, x(0)
			, y(0)
		{

		}
		prot::ProtHeader header;
		Int16 x;
		Int16 y;
	};

	struct CityQueryInfo
	{
		CityQueryInfo()
			:x(0)
			, y(0)
			, city_type(0)
			, ore_type(0)
			, city_level(0)
			, role_id(0)
			, alliance_id(0)
			, army_num(0)
		{
			memset(role_name, 0, sizeof(role_name));
			memset(alliance_name, 0, sizeof(alliance_name));
			memset(resource, 0, sizeof(resource));
		}
		static const int32 MAX_CITY_ARMY_NUM = 10;
		Int16 x;
		Int16 y;
		Int8 city_type;
		Int8 ore_type;
		Int16 city_level;
		RoleID role_id;
		RoleName role_name;
		Int64 alliance_id;
		char alliance_name[MAX_32_CHAR_LEN];
		int32 resource[RESOURCE_TYPE_COUNT];
		int32 army_num;
		ArmyFormatData army_data[MAX_CITY_ARMY_NUM];		
	};

	//查询目标坐标信息返回
	struct SCCityQueryPosiRet
	{
		SCCityQueryPosiRet()
			:header(CITY_QUERY_POSI_RET_SC)
		{
		}
		prot::ProtHeader header;
		CityQueryInfo query_info;
	};

	//城池迁移
	struct CSCityRelocate
	{
		enum MoveType
		{
			MOVE_TYPE_INVALID,
			MOVE_TYPE_USE_GOLD,
			MOVE_TYPE_USE_ITEM,
		};
		CSCityRelocate()
			:header(CITY_RELOCATE_CS)
			, move_type(0)
			, target_x(0)
			, target_y(0)
		{

		}
		prot::ProtHeader header;
		int32 move_type;  // 1= 金币， 2=道具
		Int16 target_x;
		Int16 target_y;
		
	};

	struct SCCityRelocateRet
	{
		enum ResCode
		{
			RES_CODE_INVALID,
			RES_CODE_SUCCEED,
			RES_CODE_TARGET_EXIST,		//目标不可移动
			RES_CODE_HAVE_NO_ITEM,		//消耗品不足
			RES_CODE_MARCHING,
			RES_CODE_GUARDING,
		};
		SCCityRelocateRet()
			:header(CITY_RELOCATE_RET_SC)
			, res_code(0)
			, target_x(0)
			, target_y(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code;
		Int16 target_x;
		Int16 target_y;

	};

	//出兵
	struct CSCitySendArmy
	{
		CSCitySendArmy()
			:header(CITY_SEND_ARMY_CS)
			, target_x(0)
			, target_y(0)
			, world_type(WORLD_TYPE_INVALID)
		{

		}
		prot::ProtHeader header;
		Int16 target_x;
		Int16 target_y;
		ArmyFormatData army_format;
		int32 world_type;
	};

	//加速行军
	struct CSCityQuickenArmy
	{
		CSCityQuickenArmy()
			:header(CITY_QUICKEN_ARMY_CS)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;
	};

	//撤兵
	struct CSCityWithdrawArmy
	{
		CSCityWithdrawArmy()
			:header(CITY_WITHDRAW_ARMY_CS)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;
	};

	//取部队信息
	struct CSCityGetArmyInfo
	{
		CSCityGetArmyInfo()
			:header(CITY_GET_ARMY_INFO_CS)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;
	};

	//返回部队信息
	struct SCCitySendArmyInfo
	{
		SCCitySendArmyInfo()
			:header(CITY_SEND_ARMY_INFO_SC)
			, proc_id(0)
		{

		}
		prot::ProtHeader header; 
		Int64 proc_id;
	};

	//同步主角城池坐标
	struct SCCitySendRoleCityPos
	{
		SCCitySendRoleCityPos()
			:header(CITY_SEND_ROLE_CITY_POS_SC)
			, x(0)
			, y(0)
		{

		}
		prot::ProtHeader header;
		Int16 x;
		Int16 y;
	};
	
	struct CSCityCancelMarchArmy
	{
		CSCityCancelMarchArmy()
			:header(CITY_CANCEL_MARCH_ARMY_CS)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;	
	};

}
#pragma pack(pop)

#endif
