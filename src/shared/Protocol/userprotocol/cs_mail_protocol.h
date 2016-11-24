#ifndef JXS_CSMAILPROTOCOL_H_
#define JXS_CSMAILPROTOCOL_H_
//邮件协议

#include "../cs_global_protocol_def.h"
#include "../../game/include/game_value_def.h"
#include "../../game/include/data_len_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{

//获取邮件列表
struct CSMailGetList
{
	CSMailGetList()
		:header(MAIL_GET_LIST_CS)
	{

	}
	prot::ProtHeader header;
};

struct ProtMailDesc
{
	ProtMailDesc()
		:mail_id(0)
		, create_time(0)
		, mail_type(0)
		, mail_state(0)
		, lock_state(0)
		, fetch_state(0)
		, sub_type(0)
	{
		memset(sender_name, 0, sizeof(sender_name));
		memset(title, 0, sizeof(title));
	}
	Int64 mail_id;
	Int64 create_time;
	Int8 mail_type;
	Int8 mail_state;
	Int8 lock_state;
	Int8 fetch_state;
	int32 sub_type;
	RoleName sender_name;
	char title[MAX_MAIL_TITLE_LEN];
};

//发送邮件列表信息
struct SCMailSendList
{
	const static int32 MAX_SEND_MAIL_NUM = 200;
	SCMailSendList()
		:header(MAIL_SEND_LIST_SC)
		, mail_num(0)
		, report_num(0)
		, spy_num(0)
	{

	}
	prot::ProtHeader header;
	int32 mail_num;
	int32 report_num;
	int32 spy_num;
	ProtMailDesc mail_infos[MAX_SEND_MAIL_NUM];
};

//取邮件详细信息
struct CSMailGetInfo
{
	CSMailGetInfo()
		:header(MAIL_GET_INFO_CS)
		, mail_id(0)
	{

	}
	prot::ProtHeader header;
	Int64 mail_id;
};

struct ProtMailInfo
{
	ProtMailInfo()
		:mail_id(0)
		, create_time(0)
		, mail_type(0)
		, mail_state(0)
		, lock_state(0)
		, fetch_state(0)	
		, content_data_len(0)
	{
		memset(title, 0, sizeof(title));
		memset(content, 0, sizeof(content));
	}
	Int64 mail_id;							// 邮件id
	Int64 create_time;						// 邮件创建时间
	Int8 mail_type;							// 邮件类型
	Int8 mail_state;						// 已读或者未读
	Int8 lock_state;						// 邮件锁定状态
	Int8 fetch_state;						// 附件领取状态
	RoleName sender_name;					// 发送玩家名字
	char title[MAX_MAIL_TITLE_LEN];			// 邮件标题
	int32 content_data_len;					// 邮件正文长度
	char content[MAIL_CONTENT_DATA_LEN];	// 邮件正文
};

//发送邮件详细信息
struct SCMailSendInfo
{
	SCMailSendInfo()
		:header(MAIL_SEND_INFO_SC)
	{

	}
	prot::ProtHeader header;
	ProtMailInfo mail_info;
};

//邮件信息变更同步
struct SCMailSyncInfoChange
{
	enum ChangeType
	{
		CHANGE_TYPE_INVALID,
		CHANGE_TYPE_ADD,
		CHANGE_TYPE_UPDATE,
		CHANGE_TYPE_DELETE,
	};
	SCMailSyncInfoChange()
		:header(MAIL_SYNC_INFO_CHANGE_SC)
	{

	}
	prot::ProtHeader header;
	int32 change_type;
	ProtMailDesc mail_info;
};

//删除邮件
struct CSMailDeleteMail
{
	CSMailDeleteMail()
		:header(MAIL_DELETE_MAIL_CS)
		, mail_id(0)

	{
	}
	prot::ProtHeader header;
	Int64 mail_id;
};

//删除邮件返回
struct SCMailDeleteMailRet
{
	SCMailDeleteMailRet()
		:header(MAIL_DELETE_MAIL_RET_SC)
		, res_code(-1)
		, mail_id(0)

	{
	}
	prot::ProtHeader header;
	int32 res_code;	//0=成功， -1=失败
	Int64 mail_id;
};

//写邮件
struct CSMailPostMail
{
	CSMailPostMail()
		:header(MAIL_POST_MAIL_CS)
		, content_data_len(0)
	{
		memset(receiver_name, 0, sizeof(receiver_name));
		memset(title, 0, sizeof(title));
		memset(content, 0, sizeof(content));
	}
	prot::ProtHeader header;
	RoleName receiver_name;
	char title[MAX_MAIL_TITLE_LEN];
	int32 content_data_len;
	char content[MAIL_CONTENT_DATA_LEN];
};

//写邮件返回
struct SCMailPostMailRet
{
	SCMailPostMailRet()
		:header(MAIL_POST_MAIL_RET_SC)
		, res_code(-1)
		, mail_id(0)

	{
	}
	prot::ProtHeader header;
	int32 res_code;	//0=成功， -1=失败
	Int64 mail_id;
};

//锁定/解锁邮件
struct CSMailLockMail
{
	CSMailLockMail()
		:header(MAIL_LOCK_MAIL_CS)
		, mail_id(0)

	{
	}
	prot::ProtHeader header;
	Int64 mail_id;
};

//锁定/解锁邮件返回
struct SCMailLockMailRet
{
	SCMailLockMailRet()
		:header(MAIL_LOCK_MAIL_RET_SC)
		, res_code(-1)
		, mail_id(0)

	{
	}
	prot::ProtHeader header;
	int32 res_code;	//0=成功， -1=失败
	Int64 mail_id;
};

//提取邮件附件
struct CSMailFetchAppendix
{
	CSMailFetchAppendix()
		:header(MAIL_FETCH_APPENDIX_CS)
		, mail_id(0)

	{
	}
	prot::ProtHeader header;
	Int64 mail_id;
};

//提取邮件附件返回
struct SCMailFetchAppendexRet
{
	SCMailFetchAppendexRet()
		:header(MAIL_FETCH_APPENDIX_RET_SC)
		, res_code(-1)
		, mail_id(0)

	{
	}
	prot::ProtHeader header;
	int32 res_code;	//0=成功， -1=失败
	Int64 mail_id;
};

}
#pragma pack(pop)

#endif
