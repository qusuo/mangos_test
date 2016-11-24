#ifndef JXS_BMPROT_H_
#define JXS_BMPROT_H_
//后台与gm服之间的协议

#include "protocol_def.h"
#include "../include/value_def.h"
#include "../game/include/server_def.h"
#include "../game/structs/global/mail_struct.h"
#include "../game/structs/global/mail_content_struct.h"

#pragma pack(push) 
#pragma pack(4)

namespace bmprot
{
	enum 
	{
		GM_LOGIN_BM = prot::BACKEND_GM_MSG_BEGIN,  //登录验证
		GM_LOGIN_RET_MB,		//登录结果
		CHARGE_GOLD_BM,			//充值
		CHARGE_GOLD_RET_MB,		//充值返回
		GM_COMMAND_BM,			//gm指令
		GM_COMMAND_RET_MB,		//gm指令返回
		KICK_USER_BM,			//踢人(关服时要用到)
		KICK_USER_RET_MB,		//踢人返回
		FORBID_USER_BM,			//封号
		FORBID_USER_RET_MB,		//封号返回
		SILENT_USER_BM,			//禁言
		SILENT_USER_RET_MB,		//禁言返回
		ANNOUNCE_BM,			//发公告
		ANNOUNCE_RET_MB,		//发公告返回
		CANCEL_ANNOUNCE_BM,		//取消定时公告
		CANCEL_ANNOUNCE_RET_MB,	//取消定时公告返回
		POST_MAIL_BM,			//发邮件
		POST_MAIL_RET_MB,		//发邮件返回
		UPDATE_CONFIG_BM,		//热更新配表
		UPDATE_CONFIG_RET_MB,	//热更新配表
	};

	//登录验证
	struct BMGmLogin
	{
		BMGmLogin()
			:header(GM_LOGIN_BM)
		{
			memset(gm_name, 0, sizeof(gm_name));
			memset(md5_code, 0, sizeof(md5_code));
		}
		prot::ProtHeader header;
		char gm_name[MAX_32_CHAR_LEN];
		char md5_code[MAX_32_CHAR_LEN];
	};

	//登录验证返回
	struct MBGmLoginRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};

		MBGmLoginRet()
			:header(GM_LOGIN_RET_MB)
			, res_code(RESCODE_MD5_ERROR)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//充值
	struct BMChargeGold
	{
		BMChargeGold()
			:header(CHARGE_GOLD_BM)
			, gold(0)
		{
			memset(role_id, 0, sizeof(role_id));
			memset(gm_name, 0, sizeof(gm_name));
			memset(order_code, 0, sizeof(order_code));
		}
		prot::ProtHeader header;
		char role_id[MAX_32_CHAR_LEN];
		int32 gold;
		char gm_name[MAX_32_CHAR_LEN];		
		char order_code[MAX_128K_BUFFER_LEN]; //订单号
	};

	//充值返回
	struct MBChargeGoldRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};

		MBChargeGoldRet()
			:header(CHARGE_GOLD_RET_MB)
			, res_code(RESCODE_MD5_ERROR)
		{
			memset(order_code, 0, sizeof(order_code));
		}
		prot::ProtHeader header;
		int32 res_code;
		char order_code[MAX_128_CHAR_LEN]; //订单号
	};

	//gm指令
	struct BMGmCommand
	{
		BMGmCommand()
			:header(GM_COMMAND_BM)
			, cmd_id(0)
			, param1(0)
			, param2(0)
		{
			memset(role_id, 0, sizeof(role_id));
		}
		prot::ProtHeader header;
		char role_id[MAX_32_CHAR_LEN];
		int32 cmd_id;
		int32 param1;
		int32 param2;
	};

	//gm指令返回
	struct MBGmCommandRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		MBGmCommandRet()
			:header(GM_COMMAND_RET_MB)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};
		
	//踢人(关服时要用到)
	struct BMKickUser
	{
		BMKickUser()
			:header(KICK_USER_BM)
			, opt_type(0)
		{
			memset(role_id, 0, sizeof(role_id));
		}
		prot::ProtHeader header;
		char role_id[MAX_32_CHAR_LEN];
		int32 opt_type;   //2=全部，1=玩家
	};

	//踢人返回
	struct MBKickUserRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		MBKickUserRet()
			:header(KICK_USER_RET_MB)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//封号
	struct BMForbidUser
	{
		BMForbidUser()
			:header(FORBID_USER_BM)
			, end_time(0)
		{
			memset(role_id, 0, sizeof(role_id));
		}
		prot::ProtHeader header;
		char role_id[MAX_32_CHAR_LEN];
		int32 end_time;
	};

	//封号返回
	struct MBForbidUserRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		MBForbidUserRet()
			:header(FORBID_USER_RET_MB)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//禁言
	struct BMSilentUser
	{
		BMSilentUser()
			:header(SILENT_USER_BM)
			, end_time(0)
		{
			memset(role_id, 0, sizeof(role_id));
		}
		prot::ProtHeader header;
		char role_id[MAX_32_CHAR_LEN];
		int32 end_time;
	};

	//禁言返回
	struct MBSilentUserRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		MBSilentUserRet()
			:header(SILENT_USER_RET_MB)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//发公告
	struct BMAnnounce
	{
		enum 
		{
			ANNOUNCE_TYPE_IMMEDIATELY = 1,	//即时发送
			ANNOUNCE_TYPE_TIMMING,			//定时发送
			ANNOUNCE_TYPE_REPEAT,			//重复发送
		};

		BMAnnounce()
			:header(ANNOUNCE_BM)
			, send_type(0)
			, send_interval(0)
			, send_start_time(0)
			, send_end_time(0)
			, msg_len(0)
		{
			memset(msg, 0, sizeof(msg));
		}
		prot::ProtHeader header;
		int32 send_type;	//1 = 即时发送，2 = 定时发送，3 = 重复发送
		int32 send_interval;
		int32 send_start_time;
		int32 send_end_time;
		int32 msg_len;
		char msg[MAX_512_CHAR_LEN];
	};

	//发公告返回
	struct MBAnnounceRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		MBAnnounceRet()
			:header(ANNOUNCE_RET_MB)
			, res_code(0)
			, announce_id(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
		int32 announce_id;
	};

	//取消公告
	struct BMCancelAnnounce
	{
		BMCancelAnnounce()
			:header(CANCEL_ANNOUNCE_BM)
			, announce_id(0)
		{
			memset(role_id, 0, sizeof(role_id));
		}
		prot::ProtHeader header;
		char role_id[MAX_32_CHAR_LEN];
		int32 announce_id;
	};

	//取消公告返回
	struct MBCancelAnnounceRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		MBCancelAnnounceRet()
			:header(CANCEL_ANNOUNCE_RET_MB)
			, res_code(0)
		{
		}
		prot::ProtHeader header;
		int32 res_code;
	};

	//发邮件
	struct BMPostMail
	{
		BMPostMail()
			:header(POST_MAIL_BM)
			, msg_len(0)
			, item_param_num(0)
		{
			memset(sender_name, 0, sizeof(sender_name));
			memset(receiver_role_id, 0, sizeof(receiver_role_id));
			memset(title, 0, sizeof(title));
			memset(msg, 0, sizeof(msg));
		}
		prot::ProtHeader header;
		RoleName sender_name;
		char receiver_role_id[MAX_32_CHAR_LEN];
		char title[MAX_MAIL_TITLE_LEN];
		int32 msg_len;									//公告长度
		char msg[MAX_256_CHAR_LEN];						//公告内容
		int32 item_param_num;							//道具列表长度
		ItemParam item_params[MAX_MAIL_ITEM_PARAM_NUM];	//道具列表
	};

	//发邮件返回
	struct MBPostMailRet
	{
		enum ResCode
		{
			RESCODE_SUCCEED = 0,
			RESCODE_MD5_ERROR = -1,
			RESCODE_EXCEPTION = -2,
		};
		MBPostMailRet()
			:header(POST_MAIL_RET_MB)
			, res_code(0)
		{
			memset(mail_id, 0, sizeof(mail_id));
		}
		prot::ProtHeader header;
		int32 res_code;
		char mail_id[MAX_32_CHAR_LEN];
	};
}
#endif