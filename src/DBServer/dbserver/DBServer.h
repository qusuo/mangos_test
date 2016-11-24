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

/// \addtogroup world The World
/// @{
/// \file

#ifndef __GATEWAY_H
#define __GATEWAY_H

#include "Common.h"
#include "Timer.h"
#include "Protocol\SharedDefines.h"
//#include "config/DBServerDef.h"
//#include "CliCommandHolder.h"

#include <set>
#include <list>
#include <deque>
#include <mutex>
#include <functional>
#include <vector>
#include "Protocol/ServerDef.h"
#include "common/DBServerDef.h"
#include "global/ConfigMgr.h"
#include <array>

class Object;
class ObjectGuid;
class WorldPacket;
class DBServerSession;
class Player;
class QueryResult;
class WorldSocket;

/// Storage class for commands issued for delayed execution
//struct CliCommandHolder
//{
//    typedef std::function<void(const char *)> Print;
//    typedef std::function<void(bool)> CommandFinished;
//
//    uint32 m_cliAccountId;                                  // 0 for console and real account id for RA/soap
//    AccountTypes m_cliAccessLevel;
//    std::vector<char> m_command;
//    Print m_print;
//    CommandFinished m_commandFinished;
//
//    CliCommandHolder(uint32 accountId, AccountTypes cliAccessLevel, const char* command, Print print, CommandFinished commandFinished)
//        : m_cliAccountId(accountId), m_cliAccessLevel(cliAccessLevel), m_command(strlen(command) + 1), m_print(print), m_commandFinished(commandFinished)
//    {
//        memcpy(&m_command[0], command, m_command.size() - 1);
//    }
//};

/// The World
class DBServer
{
    public:
		static volatile uint32 m_gatewayLoopCounter;

		DBServer();
		~DBServer();

        void CleanupsBeforeStop();

        //DBServerSession* FindSession(uint32 id) const;
        //void AddSession(DBServerSession* s);
        //bool RemoveSession(uint32 id);
        /// Get the number of current active sessions
        //void UpdateMaxSessionCounters();
        //uint32 GetActiveAndQueuedSessionCount() const { return m_sessions.size(); }
        //uint32 GetActiveSessionCount() const { return m_sessions.size() - m_QueuedSessions.size(); }
        //uint32 GetQueuedSessionCount() const { return m_QueuedSessions.size(); }
        /// Get the maximum number of parallel sessions on the server since last reboot
        //uint32 GetMaxQueuedSessionCount() const { return m_maxQueuedSessionCount; }
        //uint32 GetMaxActiveSessionCount() const { return m_maxActiveSessionCount; }

        /// Get the active session server limit (or security level limitations)
        uint32 GetPlayerAmountLimit() const { return m_playerLimit >= 0 ? m_playerLimit : 0; }
        AccountTypes GetPlayerSecurityLimit() const { return m_playerLimit <= 0 ? AccountTypes(-m_playerLimit) : SEC_PLAYER; }

        /// Set the active session server limit (or security level limitation)
        void SetPlayerLimit(int32 limit, bool needUpdate = false);

        // player Queue
        //typedef std::list<DBServerSession*> Queue;
        //void AddQueuedSession(DBServerSession*);
        //bool RemoveQueuedSession(DBServerSession* session);
        //int32 GetQueuedSessionPos(DBServerSession*);

        /// \todo Actions on m_allowMovement still to be implemented
        /// Is movement allowed?
        bool getAllowMovement() const { return m_allowMovement; }
        /// Allow/Disallow object movements
        void SetAllowMovement(bool allow) { m_allowMovement = allow; }

        /// Set a new Message of the Day
        void SetMotd(const std::string& motd) { m_motd = motd; }
        /// Get the current Message of the Day
        const char* GetMotd() const { return m_motd.c_str(); }

        LocaleConstant GetDefaultDbcLocale() const { return m_defaultDbcLocale; }

        /// Get the path where data (dbc, maps) are stored on disk
        

        /// When server started?
        time_t const& GetStartTime() const { return m_startTime; }
        /// What time is it?
        time_t const& GetGameTime() const { return m_gameTime; }
        /// Uptime (in secs)
        uint32 GetUptime() const { return uint32(m_gameTime - m_startTime); }

        tm* GetLocalTimeByTime(time_t now) const { return localtime(&now); }
        uint32 GetDateByLocalTime(tm* now) const { return ((uint32)(now->tm_year << 16) | (uint32)(now->tm_yday)); }
        uint32 GetDateToday() const {   return GetDateByLocalTime(GetLocalTimeByTime(m_gameTime)); }
        uint32 GetDateThisWeekBegin() const {   return GetDateToday() - GetLocalTimeByTime(m_gameTime)->tm_wday; }
        uint32 GetDateLastMaintenanceDay() const
        {
            uint32 today = GetDateToday();
			uint32 mDay = sConfigMgr.getConfig(G_CFG_UINT32_MAINTENANCE_DAY);
            tm* date     = GetLocalTimeByTime(m_gameTime);
            // formula to find last mDay of gregorian calendary
            return today - ((date->tm_wday - mDay  + 7) % 7);
        }

        /// Get the maximum skill level a player can reach
        uint16 GetConfigMaxSkillValue() const
        {
			uint32 lvl = sConfigMgr.getConfig(G_CFG_UINT32_MAX_PLAYER_LEVEL);
            return lvl > 60 ? 300 + ((lvl - 60) * 75) / 10 : lvl * 5;
        }

        void SetInitialWorldSettings();
        

        void SendWorldText(int32 string_id, ...);
        void SendGlobalMessage(WorldPacket* packet);
        void SendServerMessage(ServerMessageType type, const char* text = "", Player* player = nullptr);
        void SendZoneUnderAttackMessage(uint32 zoneId, Team team);
        void SendDefenseMessage(uint32 zoneId, int32 textId);

        /// Are we in the middle of a shutdown?
        bool IsShutdowning() const { return m_ShutdownTimer > 0; }
        void ShutdownServ(uint32 time, uint32 options, uint8 exitcode);
        void ShutdownCancel();
        void ShutdownMsg(bool show = false, Player* player = nullptr);
        static uint8 GetExitCode() { return m_ExitCode; }
        static void StopNow(uint8 exitcode) { m_stopEvent = true; m_ExitCode = exitcode; }
        static bool IsStopped() { return m_stopEvent; }

        void Update(uint32 diff);
		void AddHeartTime(uint32 diff);
        //void UpdateSessions(uint32 diff);

        
        

        /// Are we on a "Player versus Player" server?
		bool IsPvPRealm() { return (sConfigMgr.getConfig(G_CFG_UINT32_GAME_TYPE) == REALM_TYPE_PVP || sConfigMgr.getConfig(G_CFG_UINT32_GAME_TYPE) == REALM_TYPE_RPPVP || sConfigMgr.getConfig(G_CFG_UINT32_GAME_TYPE) == REALM_TYPE_FFA_PVP); }
		bool IsFFAPvPRealm() { return sConfigMgr.getConfig(G_CFG_UINT32_GAME_TYPE) == REALM_TYPE_FFA_PVP; }

        //void KickAll();
        void KickAllLess(AccountTypes sec);
        BanReturn BanAccount(BanMode mode, std::string nameOrIP, uint32 duration_secs, std::string reason, const std::string& author);
        bool RemoveBanAccount(BanMode mode, std::string nameOrIP);

        
        void InitServerMaintenanceCheck();
        void ServerMaintenanceStart();

        void ProcessCliCommands();
        //void QueueCliCommand(const CliCommandHolder* commandHolder) { std::lock_guard<std::mutex> guard(m_cliCommandQueueLock); m_cliCommandQueue.push_back(commandHolder); }

        void UpdateResultQueue();
        void InitResultQueue();

        void UpdateRealmCharCount(uint32 accid);

        LocaleConstant GetAvailableDbcLocale(LocaleConstant locale) const { if (m_availableDbcLocaleMask & (1 << locale)) return locale; else return m_defaultDbcLocale; }

        // used World DB version
        void LoadDBVersion();
        char const* GetDBVersion() { return m_DBVersion.c_str(); }
        char const* GetCreatureEventAIVersion() { return m_CreatureEventAIVersion.c_str(); }


        /**
        * \brief: force all client to request player data
        * \param: ObjectGuid guid : guid of the specified player
        * \returns: void
        *
        * Description: InvalidatePlayerDataToAllClient force all connected clients to clear specified player cache
        * FullName: World::InvalidatePlayerDataToAllClient
        * Access: public
        **/
        void InvalidatePlayerDataToAllClient(ObjectGuid guid);

    protected:
        void _UpdateGameTime();
        // callback for UpdateRealmCharacters
        void _UpdateRealmCharCount(QueryResult* resultCharCount, uint32 accountId);
    
        static volatile bool m_stopEvent;
        static uint8 m_ExitCode;
        uint32 m_ShutdownTimer;
        uint32 m_ShutdownMask;

        uint32 m_NextMaintenanceDate;
        uint32 m_MaintenanceTimeChecker;

        time_t m_startTime;
        time_t m_gameTime;
        
        uint32 mail_timer;
        uint32 mail_timer_expires;

        //typedef std::unordered_map<uint32, DBServerSession*> SessionMap;
        //SessionMap m_sessions;
        //uint32 m_maxActiveSessionCount;
        //uint32 m_maxQueuedSessionCount;

       
        int32 m_playerLimit;
        LocaleConstant m_defaultDbcLocale;                  // from config for one from loaded DBC locales
        uint32 m_availableDbcLocaleMask;                    // by loaded DBC
        void DetectDBCLang();
        bool m_allowMovement;
        std::string m_motd;

        // CLI command holder to be thread safe
        std::mutex m_cliCommandQueueLock;
        //std::deque<const CliCommandHolder *> m_cliCommandQueue;

        // Player Queue
        //Queue m_QueuedSessions;

        // sessions that are added async
        //void AddSession_(DBServerSession* s);

		//先更新到些队列，再次心跳时再添加取用户列表
        //std::mutex m_sessionAddQueueLock;
        //std::deque<DBServerSession *> m_sessionAddQueue;

        // used versions
        std::string m_DBVersion;
        std::string m_CreatureEventAIVersion;
		std::array<uint32, 10> m_heartTime; //delay time for server heart
        

		//gateway to logic socket 
		//std::unique_ptr<NetworkThread<SocketType>>(new NetworkThread<SocketType>)
};

extern uint32 realmID;

#define sDBServer MaNGOS::Singleton<DBServer>::Instance()
#endif
/// @}
