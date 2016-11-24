#ifndef JXS_GENERAL_PROT_H_
#define JXS_GENERAL_PROT_H_

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/battle_def.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot{
	struct IndexType{
		IndexType() :bag_type(0), index(0){}
		Int32 bag_type;				// 1:背包, 2:装备背包
		Int32 index;				// 如果为背包则是背包中的索引, 如果为装备背包则为装备背包的全局索引
	};

	struct GeneralEquipInfo
	{
		GeneralEquipInfo()
			:unit_pos(0)
			, equip_type(0)
			, level(0)
			, id(0)
			, exp(0)
			, dead_line(0)
		{

		}

		Int8 unit_pos;
		Int8 equip_type;
		Int16 level;
		Int32 id;
		Int32 exp;
		Int64 dead_line;
	};

	struct GeneralSkillRecord{
		GeneralSkillRecord() :skill_id(0), level(0), exp(0)
		{}
		Int32 skill_id;
		Int32 level;
		Int32 exp;
	};

	struct GeneralBaseRecord{
		GeneralBaseRecord() :store_id(0), config_id(0), level(0), exp(0), revival_num(0)
			, leadership(0), yong(0), zhi(0), mei(0), grade(0), loyalty(0){}
		Int32 store_id;
		Int32 config_id;
		Int16 level;
		Int32 exp;
		Int16 revival_num;
		Int32 leadership;
		Int32 yong;
		Int32 zhi;
		Int32 mei;
		Int32 grade;				// 阶数
		Int32 loyalty;				// 忠诚度
		Int8 state;					// 出战状态(0:未出战, 1:出战)
	};

	struct GeneralRecord{
		GeneralRecord() :equip_num(0), skill_num(0){}
		GeneralBaseRecord base_record;
		Int32 equip_num;
		GeneralEquipInfo equip_list[MAX_GENERAL_EQUIP_NUM];	// 武将装备
		Int32 skill_num;
		GeneralSkillRecord skill_list[MAX_GENERAL_SKILL_NUM]; // 武将技能
	};

	// 招募列表请求
	struct CSGeneralRecruitList
	{
		CSGeneralRecruitList()
			:header(csprot::GENERAL_RECRUIT_CS){}
		prot::ProtHeader header;
	};

	// 招募列表返回
	const Int32 MAX_GENERAL_RECUIT_LIST_NUM = MAX_GENERAL_NUM;
	struct SCGeneralRecruitList
	{
		SCGeneralRecruitList()
			:header(csprot::GENERAL_RECRUIT_CS)
			, num(0)
		{
			memset(general_id_list, 0, sizeof(general_id_list));
		}
		prot::ProtHeader header;
		Int32 num;
		Int32 general_id_list[MAX_GENERAL_RECUIT_LIST_NUM];
	};

	// 武将合成
	const Int32 GENERAL_COMPOSE_MATERIAL_NUM = 20;
	const Int32 GENERAL_COMPOSE_GENERAL_NUM = 6;
	struct CSGeneralCompose
	{
		CSGeneralCompose()
			:header(csprot::GENERAL_COMPOSE_CS)
			, general_num(0), material_num(0)
			, compose_id(0)
		{
			memset(store_index_list, 0, sizeof(store_index_list));
			memset(materail_list, 0, sizeof(materail_list));
		}
		prot::ProtHeader header;
		Int32 compose_id;								// 合成id
		Int32 material_num;								// 材料列表长度
		Int32 materail_list[GENERAL_COMPOSE_MATERIAL_NUM];	// 材料列表 (物品在背包中的位置)
		Int32 general_num;								// 武将列表长度						
		Int32 store_index_list[GENERAL_COMPOSE_GENERAL_NUM];	// -1:武将 id 从 1开始
	};

	// 武将合成
	struct SCGeneralCompose
	{
		SCGeneralCompose()
			:header(csprot::GENERAL_COMPOSE_SC)
			, ret(0)
			, compose_id(0)
			,store_index(0)
		{}
		prot::ProtHeader header;
		Int32 ret;						// 1:合成成功, 2:合成失败
		Int32 compose_id;				// 合成id
		Int32 store_index;				// 合成武将id
		Int32 general_id;		// 合成武将的配置id
	};

	// 武将招募
	struct CSGeneralRecruit
	{
		CSGeneralRecruit()
			:header(csprot::GENERAL_RECRUIT_CS){}
		prot::ProtHeader header;
		Int32 recruit_id;					// 招募id
		Int32 cost_type;					// 招募类型 1:金币, 2:资源, 3:免费金币招募, 4:免费资源招募
		Int32 num_type;						// 招募数量类型: 1:1次, 2:10次
	};

	struct GeneralIdRecord{
		GeneralIdRecord() :config_id(0), id(0){}
		Int32 config_id;
		Int32 id;
	};
	const Int32 GENERAL_RECRUIT_LIST_LEN = 10;
	// 武将招募返回
	struct SCGeneralRecruit
	{
		SCGeneralRecruit()
			:header(csprot::GENERAL_RECRUIT_SC)
			, ret(0)
			, recruit_id(0)
			, num(0){}
		prot::ProtHeader header;
		Int32 ret;					// 1:招募成功, 2:招募失败
		Int32 recruit_id;			// 招募id
		Int32 num;
		GeneralIdRecord list[GENERAL_RECRUIT_LIST_LEN];
	};

	// 武将列表请求
	struct CSGeneralListSync
	{
		CSGeneralListSync()
			:header(csprot::GENERAL_LIST_SYNC_CS){}
		prot::ProtHeader header;
	};

	const Int32 GENERAL_NUM_PERPAGE = MAX_GENERAL_NUM_ALL;
	// 武将列表返回
	struct SCGeneralListSync
	{
		SCGeneralListSync()
			:header(csprot::GENERAL_LIST_SYNC_SC)
			, general_num(0)
		{
			memset(record, 0 , sizeof(record));
		}
		prot::ProtHeader header;
		Int32 comm_recuit_num;			// 普通付费招募次数
		Int32 advance_recruit_num;		// 高级付费招募次数
		Int32 free_comm_recuit;			// 免费招募次数
		Int32 free_advance_recruit_num;	// 高级免费招募次数
		Int64 free_comm_recruit_count_left_time;		// 普通招募免费次数计数剩余时间
		Int64 free_advance_recuit_count_left_time;		// 高级招募免费次数计数剩余时间
		Int32 general_num;
		GeneralRecord record[GENERAL_NUM_PERPAGE];		// 每次请求一页
	};

	// 武将同步
	struct CSGeneralSync
	{
		CSGeneralSync()
			:header(csprot::GENERAL_SYNC_CS),general_store_index(0){}
		prot::ProtHeader header;
		Int32 general_store_index;
	};

	// 武将同步返回
	struct SCGeneralSync
	{
		SCGeneralSync()
			:header(csprot::GENERAL_SYNC_SC){}
		prot::ProtHeader header;
		GeneralRecord record;
	};

	// 武将升级
	struct CSGeneralUpgrade
	{
		CSGeneralUpgrade()
			:header(csprot::GENERAL_UPGRADE_CS){}
		prot::ProtHeader header;
		Int32 store_index;				// 武将存储id
	};

	// 武将升级返回
	struct SCGeneralUpgrade
	{
		SCGeneralUpgrade()
			:header(csprot::GENERAL_UPGRADE_SC){}
		prot::ProtHeader header;
		Int32 ret;				// 1:升级成功, 2:升级失败
		Int32 store_index;				// 武将id
	};

	//请求装备信息
	struct CSGeneralEquipListSync
	{
		CSGeneralEquipListSync()
			:header(csprot::GENERALEQUIP_SEND_ALL_INFO_CS){}
		prot::ProtHeader header;
		Int32 store_index;
	};

	//发送所有装备信息
	struct SCGeneralEquipListSync
	{
		SCGeneralEquipListSync()
			:header(csprot::GENERALEQUIP_SEND_ALL_INFO_SC)
			, equip_num(0)
		{
		}
		prot::ProtHeader header;
		Int32 store_index;
		Int32 equip_num;
		GeneralEquipInfo unit_equip_infos[MAX_GENERAL_EQUIP_NUM];
	};

	//同步单个装备
	struct SCGeneralEquipSync
	{
		enum ChangeType
		{
			CHANGE_TYPE_INVALID,
			CHANGE_TYPE_ADD,
			CHANGE_TYPE_UPDATE,
			CHANGE_TYPE_REMOVE,
		};
		SCGeneralEquipSync()
			:header(csprot::GENERALEQUIP_EQUIP_INFO_SYNC_SC)
			, store_index(0)
			, change_type(0)
		{
		}
		prot::ProtHeader header;
		Int32 store_index;
		Int32 change_type;
		GeneralEquipInfo equip_info;
	};

	//穿上或更换装备
	struct CSGeneralEquipPutOn
	{
		enum SrcType
		{
			SRC_TYPE_INALID,
			SRC_TYPE_EQUIP,		//已装备的
			SRC_TYPE_KNAP,		//背包里的
		};
		CSGeneralEquipPutOn()
			:header(csprot::GENERALEQUIP_PUTON_CS)
			, store_index(0)
			, dest_unit_pos(0)
			, equip_type(0)
			, src_type(0)
			, src_pos(0)
		{
		}
		prot::ProtHeader header;
		Int32 store_index;
		Int8 dest_unit_pos;		//目标单元序号
		Int8 equip_type;
		Int16 src_type;
		Int32 src_pos;
	};

	struct CSGeneralEquipTakeOff
	{
		CSGeneralEquipTakeOff()
			:header(csprot::GENERALEQUIP_TAKEOFF_CS)
			, dest_unit_pos(0)
			, dest_equip_type(0)
			, reserved(0)
		{

		}
		prot::ProtHeader header;
		Int32 store_index;
		Int8 dest_unit_pos;		//目标单元序号
		Int8 dest_equip_type;
		Int16 reserved;
	};

	const Int32 MAX_KNAP_EQUIP_NUM1 = 200;
	struct CSGeneralEquipUpgrade
	{
		enum DestType
		{
			DEST_TYPE_INVALID,
			DEST_TYPE_EQUIP,
			DEST_TYPE_KNAP,
		};
		CSGeneralEquipUpgrade()
			:header(csprot::GENERALEQUIP_UPGRADE_CS)
			, equip_type(0)
			, dest_type(0)
			, dest_pos(0)
			, materail_num(0)
		{
			memset(material_list, 0, sizeof(material_list));
		}
		prot::ProtHeader header;
		Int32 store_index;
		Int8 equip_type;
		Int8 dest_type;
		Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
		Int32 materail_num;
		Int32 material_list[MAX_KNAP_EQUIP_NUM1];
	};

	struct SCGeneralEquipUpgrade
	{
		enum Rescode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_FAIL = -1,
		};
		SCGeneralEquipUpgrade()
			:header(csprot::GENERALEQUIP_UPGRADE_SC)
			, res_code(0)
			, equip_type(0)
			, dest_type(0)
			, dest_pos(0)
		{

		}
		prot::ProtHeader header;
		Int32 store_index;
		Int32 res_code;
		Int8 equip_type;
		Int8 dest_type;
		Int16 dest_pos;  //如果是身上装备，表示哪个单元，如果是背包的，表示哪个格子 从0开始
	};

	//交换两个单元的装备
	struct CSGeneralEquipTransPosition
	{
		CSGeneralEquipTransPosition()
			:header(csprot::GENERALEQUIP_TRANSPOSITION_CS)
			, dest_unit_pos(0)
			, src_unit_pos(0)
			, reserved(-1)
		{

		}
		prot::ProtHeader header;
		Int32 dest_store_index;
		Int32 dest_unit_pos;
		Int32 src_store_index;
		Int32 src_unit_pos;
		Int16 reserved;
	};

	//交换两个单元的装备返回
	struct SCGeneralEquipTransPosition
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_FAIL = -1,
		};
		SCGeneralEquipTransPosition()
			:header(csprot::GENERALEQUIP_TRANSPOSITION_SC)
			, res_code(-1)
			, dest_unit_pos(0)
			, src_unit_pos(0)
		{

		}
		prot::ProtHeader header;
		Int32 dest_store_index;
		Int32 dest_unit_pos;
		Int32 src_store_index;
		Int32 src_unit_pos;
		Int8 res_code;
	};

	// 装备合成
	const Int32 GENERAL_EQUIP_COMPOSE_MATERAILL_NUM = 20;
	struct CSGeneralEquipCompose
	{
		CSGeneralEquipCompose()
			:header(csprot::GENERALEQUIP_COMPOSE_CS)
			, store_index(0), compose_equip_id(0), item_num(0){}
		prot::ProtHeader header;
		Int32 type;											// 1:装备合成, 2:神器合成
		Int32 store_index;									// 武将id
		Int32 compose_equip_id;								// 合成装备id
		Int32 item_num;										// 合成材料列表长度
		IndexType item_list[GENERAL_EQUIP_COMPOSE_MATERAILL_NUM];			// 合成材料列表
	};

	// 装备合成返回
	struct SCGeneralEquipCompose
	{
		SCGeneralEquipCompose()
			:header(csprot::GENERALEQUIP_COMPOSE_SC)
		,ret(0), store_index(0), compose_equip_id(0){}
		prot::ProtHeader header;
		Int32 type;				// 1:装备合成, 2:神器合成
		Int32 ret;				// 1:合成成功, 2:合成失败
		Int32 store_index;		// 武将id
		Int32 compose_equip_id;	// 合成装备id
	};

	// 武将技能列表同步请求
	struct CSGeneralSkillListSync{
		CSGeneralSkillListSync() :header(csprot::GENERAL_SKILL_LIST_SYNC_CS){}
		prot::ProtHeader header;
		Int32 store_index;								// 武将id
	};

	// 武将技能列表同步返回
	struct SCGeneralSkillListSync{
		SCGeneralSkillListSync() :header(csprot::GENERAL_SKILL_LIST_SYNC_SC){}
		prot::ProtHeader header;
		Int32 store_index;													// 武将id
		Int32 skill_num;											// 技能数
		GeneralSkillRecord skill_list[MAX_GENERAL_SKILL_NUM];		// 技能列表
	};

	// 武将技能同步请求
	struct CSGeneralSkillSync{
		CSGeneralSkillSync() : header(csprot::GENERAL_SKILL_SYNC_CS){}
		prot::ProtHeader header;
		Int32 store_index;								// 武将id
		Int32 skill_id;							// 技能id
	};

	// 武将技能同步返回
	struct SCGeneralSkillSync{
		SCGeneralSkillSync() : header(csprot::GENERAL_SKILL_SYNC_SC){}
		prot::ProtHeader header;
		Int32 store_index;								// 武将id
		GeneralSkillRecord skill_record;		// 技能数据
	};

	// 武将技能升级请求
	struct CSGeneralSkillUgrade{
		CSGeneralSkillUgrade() : header(csprot::GENERAL_SKILL_UPGRADE_CS){}
		prot::ProtHeader header;
		Int32 store_index;				// 武将id
		Int32 skill_id;			// 技能id
	};

	// 武将技能升级返回
	struct SCGeneralSkillUgrade{
		SCGeneralSkillUgrade() : header(csprot::GENERAL_SKILL_UPGRADE_SC){}
		prot::ProtHeader header;
		Int32 ret;				// 1:升级成功, 2:升级失败
		Int32 store_index;				// 武将id
		Int32 skill_id;			// 技能id
	};

	//技能重置请求
	struct CSGeneralSkillReset
	{
		CSGeneralSkillReset()
			:header(csprot::GENERAL_SKILL_RESET_CS){}
		prot::ProtHeader header;
		Int32 store_index;
	};

	//技能重置,重置后技能等级为0，返还全部技能书
	struct SCGeneralSkillReset
	{
		SCGeneralSkillReset()
			:header(csprot::GENERAL_SKILL_RESET_SC)
			, res_code(K_EXCEPTION)
			, reserve(0)
			, skill_books(0){}
		prot::ProtHeader header;
		Int16 res_code;		// 1:重置成功, 2:重置失败
		Int16 reserve;
		Int32 skill_books;	//重置后获取的技能书数量
	};

	//武将升阶
	const Int32 GENERAL_GRADE_UPGRADE_MATERIAL_NUM = 20;
	const Int32 GENERAL_GRADE_UPGRADE_GENERAL_NUM = 6;
	struct CSGeneralGradeUpgrade
	{
		CSGeneralGradeUpgrade()
			:header(csprot::GENERAL_GRADE_UPGRADE_CS), store_index(0)
			, material_num(0), general_num(0)
		{
			memset(store_index_list, 0, sizeof(store_index_list));
		}
		prot::ProtHeader header;
		Int32 store_index;
		
		
		Int32 material_num;												// 材料列表长度
		Int32 material_list[GENERAL_GRADE_UPGRADE_MATERIAL_NUM];		// 材料列表 (物品在背包中的位置)
		Int32 general_num;												// 武将列表长度
		Int32 store_index_list[GENERAL_GRADE_UPGRADE_GENERAL_NUM];		// 武将列表
	};

	// 武将升阶返回
	struct SCGeneralGradeUpgrade
	{
		SCGeneralGradeUpgrade()
			:header(csprot::GENERAL_GRADE_UPGRADE_SC){}
		prot::ProtHeader header;
		Int32 ret;									// 结果 1:成功, 2:失败
		Int32 store_index;							// 将领id
	};

	struct CSGeneralTest{
		CSGeneralTest()
			:header(csprot::GENERAL_TEST){}
		prot::ProtHeader header;
		Int32 type;
	};

	struct CSTemGive{
		CSTemGive() :header(csprot::TEST_ITEM_GIVE_CS){}
		prot::ProtHeader header;
		Int32 type;			// 1:主角物品, 2:武将物品
		Int32 store_index;
		Int32 item_id;
		Int32 num;
		Int32 unit_pos;
	};

	struct SCTemGive{
		SCTemGive() :header(csprot::TEST_ITEM_GIVE_SC){}
		prot::ProtHeader header;
		Int32 ret;			// 1:成功, 2:失败
		Int32 type;			// 1:主角物品, 2:武将物品
		Int32 store_index;
		Int32 unit_pos;		// 阵型位置
	};
}

#pragma pack(pop)
#endif