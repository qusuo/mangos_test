#include "army_data.h"

ArmyItemData::ArmyItemData()
{
	Reset();
}

ArmyItemData::~ArmyItemData()
{

}

void ArmyItemData::Reset()
{
	item_pos = 0;
	proficiency = 0;
	soldier_cfg_id = 0;
	soldier_num = 0;
	soldier_type = 0;
	soldier_class = 0;
	soldier_level = 0;
	atk_mode = 0;
	memset(nor_attr, 0, sizeof(nor_attr));
	memset(nor_attr_add, 0, sizeof(nor_attr_add));
	restrict_ids.RemoveAll();
	aura_ids.RemoveAll();
}

ArmyData::ArmyData()
{
	Reset();
}

ArmyData::~ArmyData()
{

}

void ArmyData::Reset()
{
	obj_id = 0;		//角色id或怪物id
	obj_level = 0;
	memset(obj_name, 0, sizeof(obj_name));
	army_type = 0;	//角色，怪物，机器人
	for (Int32 i = 0; i < FORMATION_GRID_NUM; ++i)
	{
		item_data[i].Reset();
	}
	
	for (Int32 i = 0; i < GENERAL_TYPE_COUNT; ++i)
	{
		general_data[i].Reset();
	}
	memset(general_add, 0, sizeof(general_add));
	battle_type_hurt_add = 0;
	battle_type_hurt_substract = 0;

}
ArmyGeneralData::ArmyGeneralData()
{
	Reset();
}

ArmyGeneralData::~ArmyGeneralData()
{
}

void ArmyGeneralData::Reset()
{
	general_id = 0;
	store_index = -1;
	level = 0;
	grade = 0;
	memset(general_attr, 0, sizeof(general_attr));
	memset(proficiency, 0, sizeof(proficiency));
	general_skill = 0;
	soldier_skill = 0;
}

