
#include "CustomLuaScript.h"
#include "lua_include.h"
#include "Platform/Define.h"
#include "llimits.h"
#include "ByteBuffer.h"
#include "Config/DefaultDef.h"

extern "C"
{
#include "lundump.h"
#include "lstate.h"
}

#include "Log.h"

#define _SCRIPT_DEBUG 1
CCustomLuaScript::CCustomLuaScript()
{
	m_pLua = NULL;
	m_nLastError = 0;
	m_sLastFnName[0] = 0;
}

CCustomLuaScript::~CCustomLuaScript()
{
	setScript(NULL);
}

bool CCustomLuaScript::setScript(const char * sText)
{
	if ( m_pLua )
	{
		//调用析构函数
		callFinal();
		//销毁虚拟机
		lua_close(m_pLua);
		m_pLua = NULL;
	}

	if ( sText )
	{
		//跳过UTF-8 BOM, todo:hunter 测试
		if ((*(int32 *)sText & 0x00FFFFFF) == 0xBFBBEF)
			sText += 3;
	}

	if ( sText && sText[0] )
	{
		//创建虚拟机
		m_pLua = createLuaVM();
		//打开基本函数库
		openBaseLibs();
		//注册本地函数库
		registLocalLibs();
		//保存错误函数名称
		strcpy(m_sLastFnName, "<LOADER>");
		//加载脚本		
		if ( !lcCheck(luaL_loadstring( m_pLua, sText )) )
			return false;	

		//初始化脚本		
		if ( !pcall( 0, 0, 0 ) )
		{
#ifdef _SCRIPT_DEBUG
			FILE *fp = fopen("GlobalFunctionDebug.txt","wt");
			size_t count = strlen(sText);
			fwrite(sText, count, 1, fp);
			fclose(fp);
#endif		
			return false;	
		}
		//调用初始化函数
		return callInit();
	}
	return true;
}

int CCustomLuaScript::StreamWriter(lua_State* L, const void* p, size_t size, void* u)
{
	UNUSED(L);
	CDataPacket *packet = (CDataPacket *)u;
	if (!packet)
		return 1;
	packet->append((int8*)p, size);
	return 0;
}

bool CCustomLuaScript::CompileLua(lua_State* L, const char* szContent, CDataPacket &packet)
{
	if (!L || !szContent)
		return false;

	if ( !lcCheck(luaL_loadstring(L, szContent)) )
		return false;

	packet.clear();
	const Proto* proto = clvalue(L->top - 1)->l.p;
	luaU_dump(L, proto, CCustomLuaScript::StreamWriter, &packet, 0);
	return true;
}

bool CCustomLuaScript::resetBinScript(CDataPacket& packet)
{	
	if (!m_pLua)
		return false;
	callFinal();
	if ( !lcCheck( luaL_loadbuffer(m_pLua, (char*)packet.contents(), packet.size(), "") ) )	
		return false;
	if ( !pcall( 0, 0, 0 ) )
		return false;
	bool bRet = callInit();	
	return bRet;
}

bool CCustomLuaScript::setBinScript(const char * szScript, CDataPacket& packet, const char * name, bool bCompile)
{	
	if (m_pLua)
	{
		//调用析构函数		
		callFinal();
		//销毁虚拟机		
		lua_close(m_pLua);
		m_pLua = NULL;
	}

	if ((bCompile && szScript) || (!bCompile) && (packet.size() > 0))
	{
		//创建虚拟机			
		m_pLua = createLuaVM();
		//打开基本函数库		
		openBaseLibs();
		//注册本地函数库		
		registLocalLibs();
		//保存错误函数名称
		strcpy(m_sLastFnName, "<LOADER>");		
		//加载脚本		
		if (bCompile)
		{			
			if (!CompileLua(m_pLua, szScript, packet))
				return false;		
		}		
		if ( !lcCheck( luaL_loadbuffer(m_pLua, (const char*)packet.contents(), packet.size(), name) ) )	
			return false;				
		//初始化脚本		
		if ( !pcall( 0, 0, 0 ) )
			return false;		
		//调用初始化函数		
		return callInit();
	}
	return true;
}

int CCustomLuaScript::getAvaliableMemorySize()
{
	if ( !m_pLua ) return 0;
	int n = lua_gc(m_pLua , LUA_GCCOUNT, 0) * 1024;
	n |= lua_gc( m_pLua, LUA_GCCOUNTB, 0 );
	return n;
}

int CCustomLuaScript::gc()
{
	if ( !m_pLua ) return 0;
	int n = getAvaliableMemorySize();
	lua_gc(m_pLua , LUA_GCCOLLECT, 0);
	return n - getAvaliableMemorySize();
}

//创建虚拟机对象
lua_State* CCustomLuaScript::createLuaVM()
{
	return luaL_newstate();
}

bool CCustomLuaScript::openBaseLibs()
{
	if ( !lcCheck( lua_cpcall( m_pLua, luaopen_base, NULL ) ) )
		return false;
	if ( !lcCheck( lua_cpcall( m_pLua, luaopen_string, NULL ) ) )
		return false;
	if ( !lcCheck( lua_cpcall( m_pLua, luaopen_math, NULL ) ) )
		return false;
	if ( !lcCheck( lua_cpcall( m_pLua, luaopen_table, NULL ) ) )
		return false;
	return true;
}

bool CCustomLuaScript::registLocalLibs()
{
	return true;
}

bool CCustomLuaScript::callInit()
{
	return true;
}

bool CCustomLuaScript::callFinal()
{
	return true;
}

bool CCustomLuaScript::lcCheck(int nError)
{
	if ( !nError )
		return true;
	m_nLastError = nError;
	
	TCHAR sErrDesc[256];
	const char * pErrDesc = NULL;
	std::stringstream ss;

    if ( lua_gettop( m_pLua ) > 0 )
	{
#ifdef UNICODE
		wylib::string::CAnsiString sError = lua_tostring(m_pLua, -1 );
		wylib::string::CWideString swError;
		sError.toWStr(&swError);
		_asncpy(sErrDesc, swError);
#else
		pErrDesc = lua_tostring( m_pLua, -1 );
		ss << pErrDesc;
		ss >> sErrDesc;

		//_asncpy(sErrDesc, pErrDesc);
#endif
		lua_pop( m_pLua, 1 );
	}
    else pErrDesc = "falt system error: lua_gettop <= 0";
    if ( !pErrDesc || !pErrDesc[0] )
		pErrDesc = "undefined error";

	sLog.outError("function:%s,error:%s", m_sLastFnName, pErrDesc );
	return false;
}

bool CCustomLuaScript::pcall(const int args, const int results, const int nErrFunc)
{
	bool result = true;

	int nTop = lua_gettop( m_pLua ) - args - 1;//-1是排除函数名称所占的栈空间
	result = lcCheck(lua_pcall(m_pLua, args, results, nErrFunc));
	int nTop2 = lua_gettop( m_pLua ) - results;

	if ( results != LUA_MULTRET && nTop != nTop2 )
	{
		sLog.outError("function:%s,the stack before call was:%d,the stack after call is:%d,stack difference value is:%d", m_sLastFnName, nTop, nTop2, nTop2 - nTop);
	}

	return result;
}