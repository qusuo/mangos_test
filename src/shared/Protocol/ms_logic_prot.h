#ifndef JXS_MSLOGICPROT_H_
#define JXS_MSLOGICPROT_H_
//gm服与其他服之间的逻辑协议

#include "protocol_def.h"
#include "../include/value_def.h"
#include "../game/include/server_def.h"
#include "../game/structs/global/mail_struct.h"
#include "../game/structs/global/mail_content_struct.h"

#pragma pack(push) 
#pragma pack(4)

namespace msprot
{
	enum
	{
		CHARGE_GOLD_MS = prot::GM_SERVER_LOGIC_MSG_BEGIN, //充值
		CHARGE_GOLD_RET_SM,		//充值返回
		GM_COMMAND_MS,			//gm指令
		GM_COMMAND_RET_SM,		//gm指令返回
		KICK_USER_MS,			//踢人(关服时要用到)
		KICK_USER_RET_SM,		//踢人返回
		FORBID_USER_MS,			//封号
		FORBID_USER_RET_SM,		//封号返回
		SILENT_USER_MS,			//禁言
		SILENT_USER_RET_SM,		//禁言返回
		POST_MAIL_MS,			//发邮件
		POST_MAIL_RET_SM,		//发邮件返回
		UPDATE_CONFIG_MS,		//热更新配表
		UPDATE_CONFIG_RET_SM,	//热更新配表
	};


	//充值
	struct MSChargeGold
	{
		MSChargeGold()
			:header(CHARGE_GOLD_MS)
			, role_id(0)
			, gold(0)
		{
			memset(gm_name, 0, sizeof(gm_name));
			memset(order_code, 0, sizeof(order_code));
		}
		prot::ProtHeader header;
		RoleID role_id;
		int32 gold;
		char gm_name[MAX_32_CHAR_LEN];
		char order_code[MAX_128K_BUFFER_LEN]; //订单号
	};

	//充值返回
	struct SMChargeGoldRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};

		SMChargeGoldRet()
			:header(CHARGE_GOLD_RET_SM)
			, res_code(RESCODE_MD5_ERROR)
		{
			memset(order_code, 0, sizeof(order_code));
		}
		prot::ProtHeader header;
		int32 res_code;
		char order_code[MAX_128_CHAR_LEN]; //订单号
	};

	//gm指令
	struct MSGmCommand
	{
		MSGmCommand()
			:header(GM_COMMAND_MS)
			, role_id(0)
			, cmd_id(0)
			, param1(0)
			, param2(0)
		{
		}
		prot::ProtHeader header;
		RoleID role_id;
		int32 cmd_id;
		int32 param1;
		int32 param2;
	};

	//gm指令返回
	struct SMGmCommandRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		SMGmCommandRet()
			:header(GM_COMMAND_RET_SM)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//踢人
	struct MSKickUser
	{
		MSKickUser()
			:header(KICK_USER_MS)
			, role_id(0)
			, opt_type(0)
		{
		}
		prot::ProtHeader header;
		RoleID role_id;
		int32 opt_type;   //2=全部，1=玩家
	};

	//踢人返回
	struct SMKickUserRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		SMKickUserRet()
			:header(KICK_USER_RET_SM)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//封号
	struct MSForbidUser
	{
		MSForbidUser()
			:header(FORBID_USER_MS)
			, role_id(0)
			, end_time(0)
		{
		}
		prot::ProtHeader header;
		RoleID role_id;
		int32 end_time;
	};

	//封号返回
	struct SMForbidUserRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		SMForbidUserRet()
			:header(FORBID_USER_RET_SM)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//禁言
	struct MSSilentUser
	{
		MSSilentUser()
			:header(SILENT_USER_MS)
			, role_id(0)
			, end_time(0)
		{
		}
		prot::ProtHeader header;
		RoleID role_id; 
		int32 end_time;
	};

	//禁言返回
	struct SMSilentUserRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		SMSilentUserRet()
			:header(SILENT_USER_RET_SM)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//发邮件
	struct MSPostMail
	{
		MSPostMail()
			:header(POST_MAIL_MS)
			, receiver_role_id(0)
			, msg_len(0)
			, item_param_num(0)
		{
			memset(sender_name, 0, sizeof(sender_name));
			memset(title, 0, sizeof(title));
			memset(msg, 0, sizeof(msg));
		}
		prot::ProtHeader header;
		RoleName sender_name;
		RoleID receiver_role_id;
		char title[MAX_MAIL_TITLE_LEN];
		int32 msg_len;									//公告长度
		char msg[MAX_256_CHAR_LEN];						//公告内容
		int32 item_param_num;							//道具列表长度
		ItemParam item_params[MAX_MAIL_ITEM_PARAM_NUM];	//道具列表
	};

	//发邮件返回
	struct SMPostMailRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		SMPostMailRet()
			:header(POST_MAIL_RET_SM)
			, res_code(0)
			, mail_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code;
		Int64 mail_id;
	};

}

#pragma pack(pop)
#endif