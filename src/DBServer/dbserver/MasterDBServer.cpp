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

/** \file
	\ingroup mangosd
	*/

#ifndef WIN32
#include "PosixDaemon.h"
#endif

#include "Common.h"
#include "MasterDBServer.h"
#include "network/DBServerSocket.h"
#include "DBServerRunnable.h"
#include "DBServer.h"
#include "Log.h"
#include "Timer.h"
#include "SystemConfig.h"

#include "Util.h"
#include "revision_sql.h"

//#include "MassMailMgr.h"
//#include "DBCStores.h"

#ifdef USE_LUA_CONFIG
#include "Config/ConfigLua.h"
#else
#include "Config/Config.h"
#endif
#include "Database/DatabaseEnv.h"
#include "Policies/Singleton.h"
#include "Network/Listener.hpp"
//#include "Network/Socket.hpp"
#include "Network/connecter.hpp"
//#include "network/LogicSocket.h"

//#include "../LogicServer/network/logicdef.h"


#include <memory>

#ifdef WIN32
#include "ServiceWin32.h"
extern int m_ServiceStatus;
#endif
#include <mbctype.h>



//extern DatabaseType WorldDatabase;
INSTANTIATE_SINGLETON_1(MasterDBServer);

volatile uint32 MasterDBServer::m_masterLoopCounter = 0;

/// Main function
int MasterDBServer::Run()
{
	/// DBServerd PID file creation
	std::string pidfile = sConfig.GetStringDefault("PidFile");
	if (!pidfile.empty())
	{
		uint32 pid = CreatePIDFile(pidfile);
		if (!pid)
		{
			sLog.outError("Cannot create PID file %s.\n", pidfile.c_str());
			Log::WaitBeforeContinueIfNeed();
			return 1;
		}

		sLog.outString("Daemon PID: %u\n", pid);
	}

	///- Start the databases
	if (!_StartDB())
	{
		Log::WaitBeforeContinueIfNeed();
		return 1;
	}

	///- Initialize the World
	sDBServer.SetInitialWorldSettings();

#ifndef WIN32
	detachDaemon();
#endif

#ifdef ONLY_NETWORK
	// server loaded successfully => enable async DB requests
	// this is done to forbid any async transactions during server startup!
	WorldDatabase.AllowAsyncTransactions();
#endif

	///- Catch termination signals
	_HookSignals();

	///- Launch DBServerRunnable thread
	MaNGOS::Thread gateway_thread(new DBServerRunnable);
	gateway_thread.setPriority(MaNGOS::Priority_Highest);

#ifdef ONLY_NETWORK
	// set realmbuilds depend on mangosd expected builds, and set server online
	{
		std::string builds = AcceptableClientBuildsListStr();
		WorldDatabase.escape_string(builds);
		WorldDatabase.DirectPExecute("UPDATE realmlist SET realmflags = realmflags & ~(%u), population = 0, realmbuilds = '%s'  WHERE id = '%u'", REALM_FLAG_OFFLINE, builds.c_str(), realmID);
	}
#endif

	MaNGOS::Thread* cliThread = nullptr;

#ifdef WIN32
	if (sConfig.GetBoolDefault("Console.Enable", true) && (m_ServiceStatus == -1)/* need disable console in service mode*/)
#else
	if (sConfig.GetBoolDefault("Console.Enable", true))
#endif
	{
		///- Launch CliRunnable thread
		//cliThread = new MaNGOS::Thread(new CliRunnable);
	}

	///- Handle affinity for multiple processors and process priority on Windows
#ifdef WIN32
	{
		HANDLE hProcess = GetCurrentProcess();

		uint32 Aff = sConfig.GetIntDefault("UseProcessors", 0);
		if (Aff > 0)
		{
			ULONG_PTR appAff;
			ULONG_PTR sysAff;

			if (GetProcessAffinityMask(hProcess, &appAff, &sysAff))
			{
				ULONG_PTR curAff = Aff & appAff;            // remove non accessible processors

				if (!curAff)
				{
					sLog.outError("Processors marked in UseProcessors bitmask (hex) %x not accessible for mangosd. Accessible processors bitmask (hex): %x", Aff, appAff);
				}
				else
				{
					if (SetProcessAffinityMask(hProcess, curAff))
						sLog.outString("Using processors (bitmask, hex): %x", curAff);
					else
						sLog.outError("Can't set used processors (hex): %x", curAff);
				}
			}
			sLog.outString();
		}

		bool Prio = sConfig.GetBoolDefault("ProcessPriority", false);

		//        if(Prio && (m_ServiceStatus == -1)/* need set to default process priority class in service mode*/)
		if (Prio)
		{
			if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
				sLog.outString("mangosd process priority class set to HIGH");
			else
				sLog.outError("Can't set mangosd process priority class.");
			sLog.outString();
		}
	}
#endif

	{
		//register 2 logic server
		const std::string connIP = sConfig.GetStringDefault("LogicServerIP", "127.0.0.1");
		std::map<std::string, uint32> addr_map;
		addr_map[connIP] = sConfigMgr.getConfig(G_CFG_UINT32_PORT_LOGIC);
		MaNGOS::Connecter<DBServerSocket> conn_logic(addr_map);
		while (!conn_logic.IsConnected())
			std::this_thread::sleep_for(std::chrono::seconds(1));

		//listen 2 players
		//MaNGOS::Listener<WorldSocket> listener(sWorld.getConfig(G_CFG_UINT32_PORT_WORLD), 8);
		MaNGOS::Listener<DBServerSocket> listener(sConfigMgr.getConfig(G_CFG_UINT32_PORT_GATEWAY), 2);

		//Regist2Server();
		// wait for shut down and then let things go out of scope to close them down
		while (!DBServer::IsStopped())
			std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	///- Set server offline in realmlist
	//LoginDatabase.DirectPExecute("UPDATE realmlist SET realmflags = realmflags | %u WHERE id = '%u'", REALM_FLAG_OFFLINE, realmID);

	///- Remove signal handling before leaving
	_UnhookSignals();

	// when the main thread closes the singletons get unloaded
	// since worldrunnable uses them, it will crash if unloaded after MasterDBServer
	gateway_thread.wait();

	//close socket server 2 server
	Unregist2Server();

	///- Clean account database before leaving
	clearOnlineAccounts();

	// send all still queued mass mails (before DB connections shutdown)
	//sMassMailMgr.Update(true);

	///- Wait for DB delay threads to end
	//CharacterDatabase.HaltDelayThread();
	WorldDatabase.HaltDelayThread();
	//LoginDatabase.HaltDelayThread();

	sLog.outString("Halting process...");

	if (cliThread)
	{
#ifdef WIN32

		// this only way to terminate CLI thread exist at Win32 (alt. way exist only in Windows Vista API)
		//_exit(1);
		// send keyboard input to safely unblock the CLI thread
		INPUT_RECORD b[5];
		HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
		b[0].EventType = KEY_EVENT;
		b[0].Event.KeyEvent.bKeyDown = TRUE;
		b[0].Event.KeyEvent.uChar.AsciiChar = 'X';
		b[0].Event.KeyEvent.wVirtualKeyCode = 'X';
		b[0].Event.KeyEvent.wRepeatCount = 1;

		b[1].EventType = KEY_EVENT;
		b[1].Event.KeyEvent.bKeyDown = FALSE;
		b[1].Event.KeyEvent.uChar.AsciiChar = 'X';
		b[1].Event.KeyEvent.wVirtualKeyCode = 'X';
		b[1].Event.KeyEvent.wRepeatCount = 1;

		b[2].EventType = KEY_EVENT;
		b[2].Event.KeyEvent.bKeyDown = TRUE;
		b[2].Event.KeyEvent.dwControlKeyState = 0;
		b[2].Event.KeyEvent.uChar.AsciiChar = '\r';
		b[2].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
		b[2].Event.KeyEvent.wRepeatCount = 1;
		b[2].Event.KeyEvent.wVirtualScanCode = 0x1c;

		b[3].EventType = KEY_EVENT;
		b[3].Event.KeyEvent.bKeyDown = FALSE;
		b[3].Event.KeyEvent.dwControlKeyState = 0;
		b[3].Event.KeyEvent.uChar.AsciiChar = '\r';
		b[3].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
		b[3].Event.KeyEvent.wVirtualScanCode = 0x1c;
		b[3].Event.KeyEvent.wRepeatCount = 1;
		DWORD numb;
		BOOL ret = WriteConsoleInput(hStdIn, b, 4, &numb);

		cliThread->wait();

#else
		cliThread->destroy();
#endif
		delete cliThread;
	}

	///- Exit the process with specified return value
	return DBServer::GetExitCode();
}

/// Initialize connection to the databases
bool MasterDBServer::_StartDB()
{
	///- Get world database info from configuration file
	std::string dbstring = sConfig.GetStringDefault("WorldDatabaseInfo");
	int nConnections = sConfig.GetIntDefault("WorldDatabaseConnections", 1);
	if (dbstring.empty())
	{
		sLog.outError("Database not specified in configuration file");
		return false;
	}
	sLog.outString("WorldDatabase total connections: %i", nConnections + 1);

	///- Initialise the world database
	if (!WorldDatabase.Initialize(dbstring.c_str(), nConnections))
	{
		sLog.outError("Cannot connect to DBServer database %s", dbstring.c_str());
		return false;
	}

	if (!WorldDatabase.CheckRequiredField("db_version", REVISION_DB_MANGOS))
	{
		///- Wait for already started DB delay threads to end
		WorldDatabase.HaltDelayThread();
		return false;
	}

	sLog.outString();

	///- Get the realm Id from the configuration file
	realmID = sConfig.GetIntDefault("RealmID", 0);
	if (!realmID)
	{
		sLog.outError("Realm ID not defined in configuration file");

		///- Wait for already started DB delay threads to end
		WorldDatabase.HaltDelayThread();
		return false;
	}

	sLog.outString("Realm running as realm ID %d", realmID);
	sLog.outString();
	return true;
}

/// Clear 'online' status for all accounts with characters in this realm
void MasterDBServer::clearOnlineAccounts()
{
	// Cleanup online status for characters hosted at current realm
	/// \todo Only accounts with characters logged on *this* realm should have online status reset. Move the online column from 'account' to 'realmcharacters'?
	//LoginDatabase.PExecute("UPDATE account SET active_realm_id = 0 WHERE active_realm_id = '%u'", realmID);

	//CharacterDatabase.Execute("UPDATE characters SET online = 0 WHERE online<>0");

	//// Battleground instance ids reset at server restart
	//CharacterDatabase.Execute("UPDATE character_battleground_data SET instance_id = 0");
}

/// Handle termination signals
void MasterDBServer::_OnSignal(int s)
{
	switch (s)
	{
	case SIGINT:
		DBServer::StopNow(RESTART_EXIT_CODE);
		break;
	case SIGTERM:
#ifdef _WIN32
	case SIGBREAK:
#endif
		DBServer::StopNow(SHUTDOWN_EXIT_CODE);
		break;
	}

	signal(s, _OnSignal);
}

/// Define hook '_OnSignal' for all termination signals
void MasterDBServer::_HookSignals()
{
	signal(SIGINT, _OnSignal);
	signal(SIGTERM, _OnSignal);
#ifdef _WIN32
	signal(SIGBREAK, _OnSignal);
#endif
}

/// Unhook the signals before leaving
void MasterDBServer::_UnhookSignals()
{
	signal(SIGINT, 0);
	signal(SIGTERM, 0);
#ifdef _WIN32
	signal(SIGBREAK, 0);
#endif
}

void MasterDBServer::Unregist2Server()
{

}
