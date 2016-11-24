#pragma once

/************************************************************************/
/*
/*                    LUA 脚本函数调用性能统计类
/*
/*    函数通过lua_hook的方式实现了记录并统计脚本中函数调用时间消耗的功能。对函数调用时间
/* 消耗的默认计算方式为计算函数调用前与调用好的时间差。因此误差可能比较大，但足以找出调用
/* 消耗的时间较大的函数。同时实现了将统计信息保存为文本流的功能，默认的输出格式为树形统计信
/* 息。
/*
/*    可以通过派生此类并覆盖HandleDebugHook方法来重新实现对函数调用信息的记录和性能的统
/* 计规则。
/*
/*    可以通过派生此类并覆盖Save方法来重新实现对统计信息保存为文本流的输出格式。
/*
/************************************************************************/

#include "ObjectCounter.h"
class CCustomLuaProfiler
{
public:
	typedef wylib::misc::CRefObject<CCustomLuaPreProcessor::CLineRangeList> CSourceLineList;

public:
	CCustomLuaProfiler();
	~CCustomLuaProfiler();

	/* 
	* Comments:设置性能统计对象
	* Param lua_State * L:
	* Param CSourceLineList * pLRList: 用于取得
	* @Return bool:如果目标对象已经被安装过性能统计，则函数返回false
	*/
	bool SetState(lua_State *L, CSourceLineList *pLRList = NULL);
	/*
	* 将函数调用性能统计保存到流中
	* 函数返回向流中写入多少字节
	*/
	size_t Save(wylib::stream::CBaseStream &stm);
	/*
	* Comments: 获取调用时间最长的一个函数栈的时间
	* @Return LONGLONG:
	*/
	LONGLONG GetMaxTotalTime();

protected:
	//在lua中存储当前性能统计调试器对象的值名称
	static const char szPerStaticDebuggerName[];
	//静态hook处理函数
	static void __cdecl StaticCallTimeHook(lua_State *L, lua_Debug *ar);

protected:
	/** 处理调试HOOK回调 **/
	virtual void HandleDebugHook(lua_State *L, lua_Debug *ar);
public:
	/** Lua函数调用性能记录 **/
	class CLuaCallRcd : public Counter<CLuaCallRcd>
	{
	public:
		char	 sName[40];		//函数名称
		LONG	 nLineNo;		//行号
		LONGLONG dwLastTime;	//上次调用时间
		LONGLONG dwMaxTime;		//最大调用时间
		LONGLONG dwMinTime;		//最小调用时间
		LONGLONG dwCallCount;	//调用次数
		LONGLONG dwTotalTime;	//本函数总计调用耗时
		LPVOID	 pSubCallList;	//子级调用函数表，类型为SubCallTable
	public:
		~CLuaCallRcd();
	};
	//子级调用函数表
	typedef CCustomHashTable<CLuaCallRcd> CSubCallTable;
	//当前调用堆栈表
	typedef wylib::container::CBaseList<CLuaCallRcd*> CCallStack;

protected:
	lua_State*		m_pLua;
	CCallStack		m_CallStack;		//当前调用堆栈
	CSubCallTable	m_RootCallTable;	//根调用记录表
	LONGLONG		m_dwIgoreRecordTime;//在保存函数调用性能记录时忽略总时间低于此值的记录
	CSourceLineList m_LRList;//行范围记录器，用于格式化函数统计时取得代码的文件名和行号
};