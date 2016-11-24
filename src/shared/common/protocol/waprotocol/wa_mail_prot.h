#ifndef JXS_WAMAILPROT_H_
#define JXS_WAMAILPROT_H_
//gamelogicserver与globalserver之间的邮件相关协议定义

#include "../wa_protocol_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace waprot
{
	//邮件通知
	struct WANewMailNotice
	{
		WANewMailNotice()
			:header(MAIL_NEW_MAIL_NOTICE_WA, SERVER_NODE_TYPE_GLOBAL, prot::SS_MSG_TYPE_SYS)
			, role_id(0)
			, mail_id(0)
			, is_spy_report(0)
		{

		}
		prot::SSHeader header;
		RoleID role_id;
		Int64 mail_id;
		Int32 is_spy_report;
	};

	//提取附件
	struct AWMailFetchAppendex
	{
		AWMailFetchAppendex()
			:header(MAIL_FETCH_APPENDEX_AW, SERVER_NODE_TYPE_GAMELOGIC, prot::SS_MSG_TYPE_SYS)
			, role_id(0)
			, mail_id(0)
		{

		}
		prot::SSHeader header;
		RoleID role_id;
		Int64 mail_id;
	};

	//提取附件返回
	struct WAMailFetchAppendexRet
	{
		WAMailFetchAppendexRet()
			:header(MAIL_FETCH_APPENDEX_RET_WA, SERVER_NODE_TYPE_GLOBAL, prot::SS_MSG_TYPE_SYS)
			, role_id(0)
			, mail_id(0)
			, res_code(-1)
		{

		}
		prot::SSHeader header;
		RoleID role_id;
		Int64 mail_id;
		Int32 res_code; //-1=失败， 0=成功
	};

}
#pragma pack(pop)

#endif
