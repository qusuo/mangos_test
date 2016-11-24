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

#ifndef CONFIGLUA_H
#define CONFIGLUA_H

#include "Common.h"
#include "Policies/Singleton.h"
#include "Platform/Define.h"
#include "lua/CustomLuaScript.h"

#include <mutex>
#include <string>
#include <unordered_map>


#if PLATFORM == PLATFORM_WINDOWS
#pragma warning( disable: 4251 4275 )
#endif

class MANGOS_DLL_SPEC ConfigLua : public CCustomLuaScript
{
public:
	ConfigLua()
		: m_filename("logicserver.conf") {
		//m_entries.reserve(512);
	};

	~ConfigLua(){};
private:
	std::string m_filename;
	typedef std::unordered_map<std::string, std::string> CONFIG_MAP;

	CONFIG_MAP m_entries; //keys are converted to lower case.  values cannot be.

protected:
	bool openGlobalTable(const char* sTableName);
	void closeTable();

public:
	bool SetSource(const std::string &file);
	bool Reload();
	//读取服务器服务配置
	bool readServerConfig(/*CLogicServer *lpLogicServer*/);
	bool IsSet(const std::string &name) const;

	const std::string GetStringDefault(const std::string &name, const std::string &def = "") const;
	bool GetBoolDefault(const std::string &name, bool def) const;
	int32 GetIntDefault(const std::string &name, int32 def) const;
	float GetFloatDefault(const std::string &name, float def) const;

	const std::string &GetFilename() const { return m_filename; }
	

public:
	std::mutex m_configLock;
};

#ifdef USE_LUA_CONFIG
#define sConfig MaNGOS::Singleton<ConfigLua>::Instance()
#else
#define sConfigLua MaNGOS::Singleton<ConfigLua>::Instance()
#endif

#endif
