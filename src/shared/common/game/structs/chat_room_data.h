#ifndef JXS_CHATROOMDATA_H_
#define JXS_CHATROOMDATA_H_
#include "../include/server_def.h"
#include "../include/game_value_def.h"
#include "../../include/value_def.h"
#include "../../stream/serializer.h"
#include "../../stream/deserializer.h"


#pragma pack(push) 
#pragma pack(4)

struct ChatMsgData
{
	ChatMsgData()
	{
		lenght = 0;
		memset(msg_buf, 0, sizeof(msg_buf));
		msg_time = 0;
	}

	void ReviseMsg(){
		if (lenght > MAX_CHAT_MSG_LENGTH)
		{
			lenght = MAX_CHAT_MSG_LENGTH;
			memset(msg_buf + MAX_CHAT_MSG_LENGTH, 0, sizeof(msg_buf) - MAX_CHAT_MSG_LENGTH);
		}
	}
	//message
	Int64 msg_time;				//以client 发言时间作为一个消息id，
	Int32 lenght;				//消息长度
	MsgBuf msg_buf; //玩家单次最多输入50个中文字
};

struct ChatRoleInfo
{
	ChatRoleInfo()
	{
		role_id = 0;
		memset(role_name, 0, sizeof(role_name));
		avatar = 0;
		reserve = 0;

		hat = 0;
		military_rank = 0;
		vip_level = 0;
	}

	bool Serialize(jxsstr::Serializer& se)
	{
		bool res = false;

		res = se.PutInt64(role_id)
			&& se.PutInt8(avatar)
			&& se.PutInt16(hat)
			&& se.PutInt16(military_rank)
			&& se.PutInt16(vip_level)
			&& se.PutInt8Vector(role_name, MAX_32_CHAR_LEN);

		return res;
	}

	bool Deserialize(jxsstr::Deserializer& ds)
	{
		bool res = false;
		Int16 tmp_out = 0;
		res = ds.GetInt64(role_id)
			&& ds.GetInt8(avatar)
			&& ds.GetInt16(hat)
			&& ds.GetInt16(military_rank)
			&& ds.GetInt16(vip_level)
			&& ds.GetInt8Vector(role_name, MAX_32_CHAR_LEN, tmp_out);

		return res;
	}

	//role base of speaker 
	RoleID role_id;				//进一步的详细消息可以通过role_id获取
	Int8 avatar;				//头像
	Int8 reserve;
	Int16 hat;					//头像挂件
	Int16 military_rank;		//军衔
	Int16 vip_level;			//vip等级
	RoleName role_name;			//名称
};

struct ChatData
{
	ChatMsgData msg_data;		//消息内容
	ChatRoleInfo msg_role_info;	//消息发送者的信息，与消息内容对应
};

//私人聊天室信息
struct ChatRoomInfo
{
	ChatRoomInfo()
	{
		chat_id = 0;
	}

	Int64 chat_id;					//私聊房间id
	ChatRoleInfo chat_role_info;	//私聊对方角色信息
}; 

#pragma pack(pop)

#endif