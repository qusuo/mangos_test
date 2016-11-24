#ifndef JXS_CSSCHEMEDEVELOPPROT_H_
#define JXS_CSSCHEMEDEVELOPPROT_H_

//计略俯计略研发协议

#include "../cs_protocol_def.h"
#include "../../game/include/server_def.h"
#include "../../game/include/game_value_def.h"

#pragma pack(push) 
#pragma pack(4)

namespace csprot
{
	//请求技能信息
	struct CSSchemeReqInfo
	{
		CSSchemeReqInfo()
			:header(SCHEME_REQ_INFO_CS)
		{
		}
		prot::ProtHeader header;
	};

	//发送技略信息
	struct SCSchemeSendInfo
	{
		SCSchemeSendInfo()
			:header(SCHEME_SEND_INFO_SC)
			, reserve(0)
		{
			memset(scheme_level, 0, sizeof(scheme_level));
		}
		prot::ProtHeader header;
		Int16 scheme_level[SCHEME_TYPE_COUNT];
		Int16 reserve;
	};

	//计略研发,每次一级
	struct CSSchemeDevelop
	{
		CSSchemeDevelop()
			:header(SCHEME_DEVELOP_CS)
			, scheme_id(0)
		{
		}
		prot::ProtHeader header;
		int32 scheme_id;	//研发ID
	};

	struct SCSchemeDevelopRet
	{
		SCSchemeDevelopRet()
			:header(SCHEME_DEVELOP_RET_SC)
			, res_code(K_EXCEPTION)
			, scheme_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code;			//1=等待进度队列完成，0 = 成功，-1=失败	
		int32 scheme_id;		//研发ID
	};


	//取消研发
	struct CSSchemeDevelopCancel
	{
		CSSchemeDevelopCancel()
			:header(SCHEME_DEVELOP_CANCEL_CS)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;		//制作进度ID
	};

	struct SCSchemeDevelopCancelRet
	{
		SCSchemeDevelopCancelRet()
			:header(SCHEME_DEVELOP_CANCEL_RET_SC)
			, res_code(K_EXCEPTION)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code;			//0 = 成功， -1=失败	
		Int64 proc_id;		//制作进度ID
	};

	//加速生产
	struct CSSchemeDevelopQuick
	{
		CSSchemeDevelopQuick()
			:header(SCHEME_DEVELOP_QUICK_CS)
			, proc_id(0)
			, upgrade_type(0)
			, item_id(0)
			, item_num(0)
		{

		}
		prot::ProtHeader header;
		Int64 proc_id;			//制作进度ID
		int32 upgrade_type;		//1 = 消耗金币 PROC_QUICK_TYPE_GOLD， 2 = 使用道具
		int32 item_id;
		int32 item_num;
	};

	//加速生产返回
	struct SCSchemeDevelopQuickRet
	{
		SCSchemeDevelopQuickRet()
			:header(SCHEME_DEVELOP_QUICK_RET_SC)
			, res_code(K_EXCEPTION)
			, proc_id(0)
		{

		}
		prot::ProtHeader header;
		int32 res_code; //0 = 成功， -1=失败	
		Int64 proc_id;		//制作进度ID
	};


}
#pragma pack(pop)

#endif
