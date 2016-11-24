#ifndef JXS_MAILCONTENTSTRUCT_H_
#define JXS_MAILCONTENTSTRUCT_H_
//邮件正文结构

#include "../../include/server_def.h"
#include "../../include/data_len_def.h"
#include "../../../stream/serializer.h"
#include "../../../stream/deserializer.h"
#include "../../include/mail_def.h"
#include "../../include/game_value_def.h"
#include "../../include/posi.h"
#include "../army_format_data.h"
#include "../item_data.h"

//注意， 这些结构参与了协议，不要用变长数组打包

//战报邮件
struct MailBattleContent
{
	struct SideData
	{
		SideData()
			:obj_type(0)
			, role_id(0)
			, vip_level(0)
			, cur_boom(0)
			, max_boom(0)
			, boom_change(0)
			, soldier_unit_num(0)
			, general_num(0)
			, is_ahead(0)
		{
			memset(role_name, 0, sizeof(role_name));
			memset(alliance_name, 0, sizeof(alliance_name));
			memset(soldiers, 0, sizeof(soldiers));
			memset(generals, 0, sizeof(generals));
			
		}
		Int8 obj_type;								//类型 1 = 角色， 2=怪物
		RoleName role_name;							//角色名
		RoleID role_id;								//角色id
		Posi city_pos;								//城池坐标
		Int16 vip_level;							//vip等级
		char alliance_name[MAX_32_CHAR_LEN];		//军团
		Int32 cur_boom;								//当前繁荣度
		Int32 max_boom;								//最大繁荣度
		Int32 boom_change;							//繁荣度变化
		Int8 soldier_unit_num;						//损兵单元个数
		SoldierFormat soldiers;						//具体损兵
		Int8 general_num;							//出战将领数量
		Int32 generals[GENERAL_TYPE_COUNT];			//出战将领id
		Int8 is_ahead;								//是否先手
	};

	MailBattleContent();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	bool SerializeSideData(jxsstr::Serializer& se, SideData& side_data);
	bool DeserializeSideData(jxsstr::Deserializer& ds, SideData& side_data);
	void Reset();

	Int16 content_type;
	Int64 battle_time;								//战斗时间
	char target_name[MAX_32_CHAR_LEN];				//攻击目标名字
	Posi target_pos;								//目标城池坐标
	char target_alliance[MAX_32_CHAR_LEN];			//目标军团名字
	char target_pos_name[MAX_32_CHAR_LEN];			//目标城池名字

	SideData attaker;
	SideData defender;
	char garrison_name[MAX_32_CHAR_LEN];			//驻军
	Int8 battle_result;								//战斗结果
	Int32 resource[RESOURCE_TYPE_COUNT];			//资源掠夺
	Int64 battle_id;								//战报id
	Int32 item_param_num;							//道具列表长度 //战利品
	ItemParam item_params[MAX_MAIL_ITEM_PARAM_NUM];	//道具列表
};

//侦查玩家邮件
struct MailSpyRoleContent
{
	MailSpyRoleContent();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	Int16 content_type;
	char city_name[MAX_32_CHAR_LEN];		//城池名字（玩家名字）
	Int64 spy_time;							//侦察时间
	Posi city_pos;							//城池位置
	char alliance_name[MAX_32_CHAR_LEN];	//军团
	char garrison_name[MAX_32_CHAR_LEN];	//驻军
	Int32 cur_boom;							//当前繁荣
	Int32 max_boom;							//最大繁荣
	Int32 resource[RESOURCE_TYPE_COUNT];	//可掠夺资源
	ArmyFormatData army_foramt;				//阵型
};

//侦查矿点邮件
struct MailSpyOreContent
{
	MailSpyOreContent();
	bool Serialize(jxsstr::Serializer& se);
	bool Deserialize(jxsstr::Deserializer& ds);
	void Reset();

	Int16 content_type;
	char city_name[MAX_32_CHAR_LEN];		//城池名字（玩家名字）
	Int64 spy_time;							//侦察时间
	Posi city_pos;							//城池位置
	char alliance_name[MAX_32_CHAR_LEN];	//军团
	char garrison_name[MAX_32_CHAR_LEN];	//驻军
	Int8 ore_type;							//矿类型
	Int16 level;							//矿等级
	Int32 resource;							//已采集资源
	ArmyFormatData army_foramt;				//阵型
};

//消息邮件
struct MailMsgContent
{
	MailMsgContent();
	bool Serialize(jxsstr::Serializer& se)
	{
		bool res = se.PutInt16(content_type)
			&& se.PutInt16(msg_len)
			&& se.PutInt8s(msg, msg_len)
			&& se.PutInt16(item_param_num)
			&& se.PutInt32s((Int32*)item_params, item_param_num * sizeof(item_params[0]));

		return res;
	}
	bool Deserialize(jxsstr::Deserializer& ds)
	{
		bool res = ds.GetInt16(content_type)
			&& ds.GetInt16(msg_len)
			&& ds.GetInt8s(msg, msg_len)
			&& ds.GetInt16(item_param_num)
			&& ds.GetInt32s((Int32*)item_params, item_param_num * sizeof(item_params[0]));

		return res;
	}
	void Reset();

	Int16 content_type;
	Int16 msg_len;									//公告长度
	char msg[MAX_256_CHAR_LEN];						//公告内容
	Int16 item_param_num;							//道具列表长度
	ItemParam item_params[MAX_MAIL_ITEM_PARAM_NUM];		//道具列表
};
#endif