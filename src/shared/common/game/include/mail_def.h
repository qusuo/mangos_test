#ifndef JXS_MAILDEF_H_
#define JXS_MAILDEF_H_
//邮件系统定义
#include "../../include/basic_type_def.h"

//邮件类型
enum MailType
{
	MAIL_TYPE_INVALID,
	MAIL_TYPE_MSG,			//邮件	
	MAIL_TYPE_REPORT,		//报告
	MAIL_TYPE_SEND_MSG,		//发件(应该是只保存在用户客户端的）
	MAIL_TYPE_END,
};

enum ReportSubType
{
	REPORT_SUB_TYPE_INVALID,
	REPORT_SUB_TYPE_BATTLE_REPORT,
	REPORT_SUB_TYPE_SPY_REPORT,
};

enum MailMsgType
{
	MAIL_MSG_TYPE_INVALID,
	MAIL_MSG_TYPE_WELCOME,	//欢迎邮件
	MAIL_MSG_TYPE_TEST,		//测试邮件

	MAIL_MSG_TYPE_END,

	
};


//邮件报告类型
enum MailReportType
{
	MAIL_REPORT_TYPE_INVALID,
	MAIL_REPORT_TYPE_ATTACK,		//攻击
	MAIL_REPORT_TYPE_DEFENCE,		//被攻击
	MAIL_REPORT_TYPE_SPY_ORE,		//侦查矿点
	MAIL_REPORT_TYPE_SPY_ROLE,		//侦查玩家城

};


//邮件状态
enum MailState
{
	MAIL_STATE_UNREAD,		//未读
	MAIL_STATE_READ,		//已读
	MAIL_STATE_DELETE,		//已删除
};

enum MailLock
{
	MAIL_LOCK_UNLOCK,
	MAIL_LOCK_LOCKED,
};

//邮件附件（奖励）状态
enum MailRewardState
{
	MAIL_REWARD_STATE_INVALID,
	MAIL_REWARD_STATE_UNFETCH,
	MAIL_REWARD_STATE_FETCHED,

	MAIL_REWARD_STATE_END,
};

enum MailStateType
{
	MAIL_STATE_TYPE_READ,
	MAIL_STATE_TYPE_FETCH,
};

#endif