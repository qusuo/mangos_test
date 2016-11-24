/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ConfigLua.h"
#include "lua\lua_include.h"
#include <boost/algorithm/string.hpp>
#include <fstream>


//获取数组长度
#ifndef ArrayCount
#define	ArrayCount(a) (sizeof(a)/sizeof((a)[0]))
#endif



INSTANTIATE_SINGLETON_1(ConfigLua);

bool ConfigLua::SetSource(const std::string &file)
{
    m_filename = file;

    return Reload();
}

bool ConfigLua::Reload()
{
    std::ifstream in(m_filename, std::ifstream::in);
    
    if (in.fail())
        return false;

    std::unordered_map<std::string, std::string> newEntries;
	newEntries.reserve(512);
    std::lock_guard<std::mutex> guard(m_configLock);

	//std::ifstream t("file.txt");
	std::string strFile((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	
	if ( !setScript(strFile.c_str()) )
		return false;

	if (!openGlobalTable("ServerConfig"))
		return false;

	//lua_State * lua_state = getLuaState();
	//if (m_pLua == NULL) return false;

	std::string entry, value;
	entry.reserve(200);
	value.reserve(200);

	int t_idx = lua_gettop(m_pLua);// 取 table 索引值 
	lua_pushnil(m_pLua);  // nil 入栈作为初始 key 

	while (lua_next(m_pLua, t_idx) != 0) {
		/* 此时栈上 -1 处为 value, -2 处为 key */
		entry = lua_tostring(m_pLua, -2); //value
		boost::algorithm::trim(entry);
		if (!entry.length()){
			lua_pop(m_pLua, 1);
			continue;
		}

		boost::algorithm::to_lower(entry);

		value = lua_tostring(m_pLua, -1); //key
		boost::algorithm::trim(value);
		if (!value.length()){
			lua_pop(m_pLua, 1);
			continue;
		}

		boost::algorithm::to_lower(value);

		newEntries[entry] = value;
		lua_pop(m_pLua, 1);
	}

	closeTable();

	m_entries = std::move(newEntries);
    return true;
}


bool ConfigLua::readServerConfig(/*CLogicServer *lpLogicServer*/)
{
	//LogicServer property
	//if (!openGlobalTable("LogicServer"))
	//	return false;

	//lua_State * lua_state = getLuaState();
	//if (lua_state == NULL) return false;
	//
	//std::string line;

	//while (lua_next(lua_state, -2)) {
	//	/* 此时栈上 -1 处为 value, -2 处为 key */
	//	lua_tostring(lua_state, -1); //value
	//	lua_tostring(lua_state, -2); //key

	//	lua_pop(lua_state, 1);
	//}

	//do
	//{
	//	std::string line;
	//	std::getline(in, line);

	//	boost::algorithm::trim(line);
	//	boost::algorithm::to_lower(line);

	//	boost::algorithm::trim_left(line);

	//	if (!line.length())
	//		continue;

	//	if (line[0] == '#' || line[0] == '[')
	//		continue;

	//	auto const equals = line.find('=');
	//	if (equals == std::string::npos)
	//		return false;

	//	auto const entry = boost::algorithm::trim_copy(boost::algorithm::to_lower_copy(line.substr(0, equals)));
	//	auto const value = boost::algorithm::trim_copy_if(boost::algorithm::trim_copy(line.substr(equals + 1)), boost::algorithm::is_any_of("\""));

	//	newEntries[entry] = value;
	//} while (in.good());

	//m_entries = std::move(newEntries);

	//LPCSTR sVal;
	//INT nVal;
	//char sServerName[64]; // 

	//bool isCommonServer = false;
	//isCommonServer = getFieldBoolean("IsCommonServer", &isCommonServer);
	////GetLogicServer()->SetCommonServer(isCommonServer);

	////服务器名字和index
	//getFieldStringBuffer("ServerName", sServerName, ArrayCount(sServerName)); //name
	/*
	SYSTEMTIME startSysTime; //服务器的开启时间
	CMiniDateTime nStart;

	BOOL IsValid = TRUE;
	LPCTSTR sName = getFieldString("ServerOpenTime", "", &IsValid);
	if (IsValid &&  sName != NULL && strlen(sName) >10) //是个基本合法的名字
	{
		sscanf(sName, "%d-%d-%d %d:%d:%d", &startSysTime.wYear, &startSysTime.wMonth, &startSysTime.wDay, &startSysTime.wHour, &startSysTime.wMinute, &startSysTime.wSecond);
	}
	else
	{
		GetLocalTime(&startSysTime);
	}
	nStart.encode(startSysTime);


	SYSTEMTIME combineTime; //服务器的合服时间
	CMiniDateTime nCombine;

	BOOL boValid = TRUE;
	LPCTSTR sTime = getFieldString("ServerCombineTime", "", &boValid);
	if (boValid &&  sTime != NULL && strlen(sTime) >10) //是个基本合法的名字
	{
		sscanf(sTime, "%d-%d-%d %d:%d:%d", &combineTime.wYear, &combineTime.wMonth, &combineTime.wDay, &combineTime.wHour, &combineTime.wMinute, &combineTime.wSecond);
		nCombine.encode(combineTime);
	}
	else
	{
		nCombine = 0;
	}

	nVal = getFieldInt("ServerIndex"); //serverindex

	INT nServerIndex = nVal;

	bool boForceAdmin = false;

	boForceAdmin = getFieldBoolean("ForceAdmin", &boForceAdmin);

	BOOL fiValid = TRUE;
	char strFilePath[64] = "./data/runtime";
	//sprintf_s(strFilePath,sizeof(strFilePath),"./data/runtime",nServerIndex);
	if (lpLogicServer)
	{
		lpLogicServer->SetServerIndex(nVal);
		lpLogicServer->SetServerName(sServerName);
		lpLogicServer->SetServerOpenTime(nStart); //设置开区时间
		lpLogicServer->SetServerCombineTime(nCombine);	// 设置合区时间
		lpLogicServer->SetStrFilePath(strFilePath);
		lpLogicServer->SetForceAdmin(boForceAdmin);//是否强制所有玩家都为管理员
	}

	OutputMsg(rmTip, "是否所有玩家都为管理员 %d", boForceAdmin);
	nVal = 0;
	INT_PTR nSpid = getFieldInt("spguid", &nVal); //读取spid
	lpLogicServer->SetSpid(nSpid); //设置spid

	/*
	bool bStartCommonServer = true;
	bStartCommonServer = getFieldBoolean("IsStartCommonServer", &bStartCommonServer);

	nVal = 0;
	nVal = getFieldInt("CommonServerId", &nVal);
	if (lpLogicServer)
	lpLogicServer->SetCommonServerId(nVal);
	if ((!isCommonServer) && (nVal == 0 || bStartCommonServer == false))
	lpLogicServer->SetStartCommonServer(false);

	*/

	/*
	//网关服务配置
	if (openFieldTable("GateService"))
	{
		sVal = getFieldString("Address");
		nVal = getFieldInt("Port");
		if (NULL != sVal)
		{
			CLogicGateManager *pLogicManager = lpLogicServer->GetGateManager();
			if (pLogicManager)
			{
				pLogicManager->SetServiceHost(sVal);
				pLogicManager->SetServicePort(nVal);
			}
		}
		closeTable();
	}

	//会话服务配置
	if (openFieldTable("SessionServer"))
	{
		sVal = getFieldString("Address");
		nVal = getFieldInt("Port");

		CLogicSSClient * pSessionClient = lpLogicServer->GetSessionClient();
		if (pSessionClient)
		{
			pSessionClient->SetServerHost(sVal);
			pSessionClient->SetServerPort(nVal);
		}
		closeTable();
	}

	//日志服务器地址配置
	if (openFieldTable("LogServer"))
	{
		sVal = getFieldString("Address");
		nVal = getFieldInt("Port");

		LogSender * pLogClient = lpLogicServer->GetLogClient();
		if (pLogClient)
		{
			pLogClient->SetServerHost(sVal);
			pLogClient->SetServerPort(nVal);
			pLogClient->SetServerIndex(nServerIndex);
			pLogClient->SetServerName(sServerName);
			pLogClient->SetServerType(jxSrvDef::GameServer);
		}
		closeTable();
	}

	//公共日志服务器地址配置
	if (openFieldTable("LocalLogServer"))
	{
		sVal = getFieldString("Address");
		nVal = getFieldInt("Port");

		CLocalSender * pLocalLogClient = lpLogicServer->GetLocalClient();
		if (pLocalLogClient)
		{
			pLocalLogClient->SetServerHost(sVal);
			pLocalLogClient->SetServerPort(nVal);
			pLocalLogClient->SetServerIndex(nServerIndex);
			pLocalLogClient->SetServerName(sServerName);
			pLocalLogClient->SetServerType(jxSrvDef::GameServer);
		}
		closeTable();
	}

	//数据client配置
	if (openFieldTable("DbServer"))
	{
		sVal = getFieldString("Address");
		nVal = getFieldInt("Port");
		CDataClient * pDbClient = lpLogicServer->GetDbClient();
		if (pDbClient)
		{
			pDbClient->SetServerHost(sVal);
			pDbClient->SetServerPort(nVal);
		}
		closeTable();
	}
	//好友服务器配置
	//if (openFieldTable("FriendServer"))
	//{
	//	CFriendClient *pClient = lpLogicServer->GetFriendClient();
	//	//内部服务器地址配置
	//	if (openFieldTable("Server"))
	//	{
	//		sVal = getFieldString("Host");
	//		nVal = getFieldInt("Port");
	//		pClient->SetServerHost(sVal);
	//		pClient->SetServerPort(nVal);
	//		closeTable();
	//	}
	//	//用户连接的好友服务器网关地址配置
	//	if (openFieldTable("Gate"))
	//	{
	//		sVal = getFieldString("Host");
	//		nVal = getFieldInt("Port");
	//		pClient->SetFriendGateHost(sVal);
	//		pClient->SetFriendGatePort(nVal);
	//		closeTable();
	//	}
	//	closeTable();
	//}
	if (openFieldTable("MgrServer"))
	{
		sVal = getFieldString("Host");
		nVal = getFieldInt("Port");
		//CMgrServClient *pClient = lpLogicServer->GetMgrClient();
		//pClient->SetServerHost(sVal);
		//pClient->SetServerPort(nVal);

		CBackStageSender * pBackClient = lpLogicServer->GetBackStageSender();
		if (pBackClient)
		{
			pBackClient->SetServerHost(sVal);
			//pBackClient->SetServerHost("127.0.0.1");
			pBackClient->SetServerPort(nVal);
			pBackClient->SetServerIndex(nServerIndex);
			pBackClient->SetServerName(sServerName);
			pBackClient->SetServerType(jxSrvDef::GameServer);
		}
		closeTable();
	}

	//if (feildTableExists("CommonServer") && openFieldTable("CommonServer"))
	//{
	//	char szServerIP[32];
	//	WORD wPort;
	//	LPCSTR szField[2] = {"Server", "Client"};
	//	int nIndex = isCommonServer ? 0 : 1;
	//	if (feildTableExists(szField[nIndex]) && openFieldTable(szField[nIndex]))
	//	{
	//		getFieldStringBuffer("Address", szServerIP, ArrayCount(szServerIP));
	//		wPort = (WORD)getFieldInt("Port");
	//		GetLogicServer()->SetCommonServerAddr(szServerIP, wPort);
	//		closeTable();
	//	}
	//	closeTable();
	//}
	*/
	closeTable();

	return true;
}

bool ConfigLua::IsSet(const std::string &name) const
{
    auto const nameLower = boost::algorithm::to_lower_copy(name);
    return m_entries.find(nameLower) != m_entries.cend();
}

const std::string ConfigLua::GetStringDefault(const std::string &name, const std::string &def) const
{
    auto const nameLower = boost::algorithm::to_lower_copy(name);

    auto const entry = m_entries.find(nameLower);

    return entry == m_entries.cend() ? def : entry->second;
}

bool ConfigLua::GetBoolDefault(const std::string &name, bool def) const
{
    auto const value = GetStringDefault(name, def ? "true" : "false");

    std::string valueLower;
    std::transform(value.cbegin(), value.cend(), std::back_inserter(valueLower), ::tolower);

    return valueLower == "true" || valueLower == "1" || valueLower == "yes";
}

int32 ConfigLua::GetIntDefault(const std::string &name, int32 def) const
{
    auto const value = GetStringDefault(name, std::to_string(def));

    return std::stoi(value);
}

float ConfigLua::GetFloatDefault(const std::string &name, float def) const
{
    auto const value = GetStringDefault(name, std::to_string(def));

    return std::stof(value);
}

void ConfigLua::closeTable()
{
	if (lua_gettop(m_pLua) > 0){
		lua_pop(m_pLua, 1);
	}
}

bool ConfigLua::openGlobalTable(const char* sTableName)
{
	lua_getglobal(m_pLua, sTableName);
	bool result = lua_istable(m_pLua, -1);
	if (!result)
	{
		lua_pop(m_pLua, 1);
	}
	return result;
}