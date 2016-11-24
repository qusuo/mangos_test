#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <Windows.h>
#include <TCHAR.h>
#include <_ast.h>
#include <bzhash.h>
#include <Tick.h>
#include <List.h>
#include <Stream.h>
#include <stdarg.h>
#include <wyString.h>
#include <RefClass.hpp>
#include <RefString.hpp>
#include "../include/ShareUtil.h"
#include <new>
#include "../include/CustomHashTable.h"
#include "../include/CustomLuaPreProcess.h"
#include "../include/CustomLuaProfiler.h"

using namespace wylib::stream;
using namespace wylib::container;

const char CCustomLuaProfiler::szPerStaticDebuggerName[] = "#PFD#";

CCustomLuaProfiler::CCustomLuaProfiler()
{
	m_pLua = NULL;
	m_dwIgoreRecordTime = 16;
}

CCustomLuaProfiler::~CCustomLuaProfiler()
{
	SetState(NULL);
}

void __cdecl CCustomLuaProfiler::StaticCallTimeHook(lua_State *L, lua_Debug *ar)
{
	CCustomLuaProfiler *pStatic;
	//读取性能统计对象
	lua_getglobal(L, szPerStaticDebuggerName);
	pStatic = (CCustomLuaProfiler*)lua_touserdata(L, -1);
	lua_pop(L, 1);
	if (pStatic) 
		pStatic->HandleDebugHook(L, ar);
	else DbgAssert(FALSE);
}

bool CCustomLuaProfiler::SetState(lua_State *L, CSourceLineList *pLRList)
{
	if (m_pLua)
	{
		//移除调试器对象
		lua_pushnil(m_pLua);
		lua_setglobal(m_pLua, szPerStaticDebuggerName);
		//清除调用记录表
		m_RootCallTable.clear();
		//清除调用堆栈
		m_CallStack.clear();

		lua_sethook(m_pLua, NULL, 0, 0);
		m_pLua = NULL;
		m_LRList = NULL;
	}
	if (L)
	{
		//已经安装了性能调试器，函数返回
		lua_getglobal(L, szPerStaticDebuggerName);
		if (!lua_isnil(L, -1))
		{
			lua_pop(L, 1);
			return false;
		}
		lua_pop(L, 1);
		//设置调试器对象
		lua_pushlightuserdata(L, this);
		lua_setglobal(L, szPerStaticDebuggerName);
		//设置调试调用钩子
		lua_sethook(L, StaticCallTimeHook, LUA_MASKCALL | LUA_MASKRET, 0);
		m_pLua = L;
		m_LRList = *pLRList;
	}
	return true;
}

void ExpandCallTable(CBaseList<CCustomLuaProfiler::CLuaCallRcd*> &callList, 
	CBaseList<CCustomLuaProfiler::CLuaCallRcd*> &callStack, CCustomLuaProfiler::CLuaCallRcd &callRec, LONGLONG dwWatchTime)
{
	if (!callRec.pSubCallList)
	{
		callList.addList(callStack);
		callList.add(NULL);//空对象作为一个分隔符
	}
	else
	{
		CHashTableIterator<CCustomLuaProfiler::CLuaCallRcd> it(*((CCustomLuaProfiler::CSubCallTable*)callRec.pSubCallList));
		CCustomLuaProfiler::CLuaCallRcd* pCallRec;
		INT_PTR nCount = callStack.count();
		pCallRec = it.first();
		while (pCallRec)
		{
			//总时间小于一定时间的都不记录
			if (pCallRec->dwTotalTime >= dwWatchTime)
			{
				callStack.add(pCallRec);
				ExpandCallTable(callList, callStack, *pCallRec, 0);//为了保证记录完整的调用栈，后续的递归调用应当传递时间忽略参数为0
				callStack.trunc(nCount);
				pCallRec = it.next();
			}
		}
	}
}

void WriteCallList(CBaseStream &stm, CBaseList<CCustomLuaProfiler::CLuaCallRcd*> &callList)
{
	char sBuffer[1024];
	CCustomLuaProfiler::CLuaCallRcd *pRec, **pRecList = callList;
	INT_PTR nLen;

	//write columns
	nLen = _sntprintf(sBuffer, ArrayCount(sBuffer), "%-40s %-8s %-8s %-8s %-8s SOURCE\r\n",
		"FUNCTION", "MIN", "MAX", "COUNT", "AVG");
	stm.write(sBuffer, sizeof(sBuffer[0]) * nLen);

	//write call records
	for (INT_PTR i=callList.count()-1; i>-1; --i)
	{
		pRec = pRecList[i];
		if (pRec)
		{
			nLen = _sntprintf(sBuffer, ArrayCount(sBuffer), "%-40s %-8I64d %-8I64d %-8I64d %-8I64d\r\n",
				pRec->sName, pRec->dwMinTime, pRec->dwMaxTime, pRec->dwCallCount, pRec->dwTotalTime/pRec->dwCallCount);
		}
		else
		{
			nLen = 2;
			memcpy(sBuffer, "\r\n\0", sizeof(sBuffer[0]) * nLen);
		}
		stm.write(sBuffer, sizeof(sBuffer[0]) * nLen);
	}
}

void WriteCallNode(CBaseStream &stm, CCustomLuaProfiler::CLuaCallRcd *pRec, INT_PTR nLevel, 
	CCustomLuaPreProcessor::CLineRangeList *pLRList)
{
	char sBuffer[512];
	INT_PTR i, nLen, nChildWriten = 0;

	/*
	aaaaa
	+--bbbbb
	|  +--ccccc
	+--ggggg
	*/
	if (nLevel > 0)
	{
		if (nLevel > 1)
		{
			for (i=1; i<nLevel; ++i)
			{
				stm.write("|  ", 3);
			}
		}
		stm.write("+--", 3);
	}

	nLen = strlen(pRec->sName);
	stm.write(pRec->sName, nLen * sizeof(pRec->sName[0]));
	nLen = 40 - nLen - ((nLevel > 0) ? (nLevel - 1) * 3 + 3 : 0);
	if (nLen > 0)
	{
		memset(sBuffer, ' ', nLen);
		stm.write(sBuffer, nLen * sizeof(sBuffer[0]));
	}

	nLen = _sntprintf(sBuffer, ArrayCount(sBuffer), " %-8I64d %-8I64d %-8I64d %-8I64d ",
		pRec->dwMinTime, pRec->dwMaxTime, pRec->dwCallCount, pRec->dwTotalTime/pRec->dwCallCount);
	stm.write(sBuffer, nLen * sizeof(sBuffer[0]));

	//获取原始代码以及行号
	if (pLRList && pRec->nLineNo >= 0)
	{
		INT_PTR nSrcNo;
		LPCTSTR sSrcFile = pLRList->getSourceLineNumber(pRec->nLineNo, nSrcNo);
		if (sSrcFile)
		{
			stm.write(sSrcFile, strlen(sSrcFile) * sizeof(sSrcFile[0]));
			nLen = sprintf(sBuffer, ":%d", nSrcNo);
			stm.write(sBuffer, nLen * sizeof(sBuffer[0]));
		}
	}
	stm.write("\r\n", 2);

	//write child nodes
	if (pRec->pSubCallList)
	{
		CHashTableIterator<CCustomLuaProfiler::CLuaCallRcd> it(*((CCustomLuaProfiler::CSubCallTable*)pRec->pSubCallList));
		pRec = it.first();
		while (pRec)
		{
			if (pRec->dwTotalTime > 0)
			{
				WriteCallNode(stm, pRec, nLevel + 1, pLRList);
				nChildWriten++;
			}
			pRec = it.next();
		}
		//调整流指针，以便接下来的写入中能够覆盖最后三个字符"|  "。
		stm.seek(-5, 1);
		stm.write("\r\n", 2);
	}
	
	if (nChildWriten <= 0)
	{
		if (nLevel > 0)
		{
			for (i=0; i<nLevel; ++i)
			{
				stm.write("|  ", 3);
			}
		}
		stm.write("\r\n", 2);
	}
}

void WriteCallTree(CBaseStream &stm, CCustomLuaProfiler::CLuaCallRcd *pCallRec, LONGLONG dwWatchTime, CCustomLuaPreProcessor::CLineRangeList *pLRList)
{
	char sBuffer[512];
	INT_PTR nLen;

	//write columns
	nLen = _sntprintf(sBuffer, ArrayCount(sBuffer), "%-40s %-8s %-8s %-8s %-8s SOURCE\r\n",
		"FUNCTION", "MIN", "MAX", "COUNT", "AVG");
	stm.write(sBuffer, sizeof(sBuffer[0]) * nLen);

	//write tree
	CHashTableIterator<CCustomLuaProfiler::CLuaCallRcd> it(*((CCustomLuaProfiler::CSubCallTable*)pCallRec->pSubCallList));
	pCallRec = it.first();
	while (pCallRec)
	{
		if (pCallRec->dwTotalTime >= dwWatchTime)
		{
			WriteCallNode(stm, pCallRec, 0, pLRList);
		}
		pCallRec = it.next();
	}
}

size_t CCustomLuaProfiler::Save(CBaseStream &stm)
{
	CBaseList<CLuaCallRcd*> callList, callStack;
	CLuaCallRcd callRec;
	ZeroMemory(&callRec, sizeof(callRec));
	callRec.pSubCallList = &m_RootCallTable;
	
	LONGLONG dwPos = stm.getPosition();

	/*
	ExpandCallTable(callList, callStack, callRec, m_dwIgoreRecordTime);
	WriteCallList(stm, callList);
	*/
	WriteCallTree(stm, &callRec, m_dwIgoreRecordTime, m_LRList);

	callRec.pSubCallList = NULL;
	return (size_t)(stm.getPosition() - dwPos);
}

LONGLONG CCustomLuaProfiler::GetMaxTotalTime()
{
	CHashTableIterator<CLuaCallRcd> it(m_RootCallTable);
	CLuaCallRcd* pRec = it.first();
	LONGLONG result = 0;

	while (pRec)
	{
		if (pRec->dwTotalTime >= result)
			result = pRec->dwTotalTime;
		pRec = it.next();
	}
	return result;
}

CCustomLuaProfiler::CLuaCallRcd::~CLuaCallRcd()
{
	if (pSubCallList)
	{
		delete (CSubCallTable*)pSubCallList;
		pSubCallList = NULL;
	}
}

void CCustomLuaProfiler::HandleDebugHook(lua_State *L, lua_Debug *ar)
{
	CSubCallTable *pCallTable = NULL;
	CLuaCallRcd *pCallRec = NULL;

	//获取当前的调用记录
	if (m_CallStack.count() > 0)
		pCallRec = m_CallStack[m_CallStack.count()-1];
	
	if (ar->event == LUA_HOOKCALL)
	{
		char sName[512];
		int nNameLen;
		//生成函数名称，格式为function:行号
		lua_getinfo(L, "Sn", ar);
		nNameLen = _sntprintf(sName, ArrayCount(sName) - 1, "%s:%s:%s", ar->what, ar->namewhat, ar->name ? ar->name : "<NA>");
		 _sntprintf(&sName[nNameLen], ArrayCount(sName) - 1 - nNameLen, ":%d", ar->linedefined);
		//TRACE("CI %s\n", sName);
		//取得新的调用记录
		if (pCallRec)
		{
			if (!pCallRec->pSubCallList)
				pCallRec->pSubCallList = new CSubCallTable();
			pCallTable = (CSubCallTable*)pCallRec->pSubCallList;
		}
		else pCallTable = &m_RootCallTable;
		pCallRec = pCallTable->get(sName);
		//新函数被调用，没有调用记录则创建
		if (!pCallRec)
		{
			pCallRec = pCallTable->put(sName);
			pCallRec->dwMinTime = INT_MAX;
			//保存函数名称时只拷贝名字，不包含行号
			sName[nNameLen] = 0;
			_asncpytA(pCallRec->sName, sName);
			pCallRec->nLineNo = ar->linedefined;
		}

		pCallRec->dwCallCount++;
		pCallRec->dwLastTime = _getTickCount();
		m_CallStack.add(pCallRec);
	}
	else if (ar->event == LUA_HOOKRET || ar->event == LUA_HOOKTAILRET)
	{
		if (pCallRec)
		{
			//TRACE("RO %s\n", pCallRec->sName);
			pCallRec->dwLastTime = _getTickCount() - pCallRec->dwLastTime;
			pCallRec->dwTotalTime += pCallRec->dwLastTime;
			if (pCallRec->dwLastTime > pCallRec->dwMaxTime)
				pCallRec->dwMaxTime = (LONG)pCallRec->dwLastTime;
			if (pCallRec->dwLastTime < pCallRec->dwMinTime)
				pCallRec->dwMinTime = (LONG)pCallRec->dwLastTime;

			m_CallStack.trunc(m_CallStack.count()-1);
		}
		else DbgAssert(FALSE);
	}
}
