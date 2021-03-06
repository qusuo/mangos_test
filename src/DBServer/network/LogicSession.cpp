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
    \ingroup u2w
*/

#include "LogicSocket.h"                                    // must be first to make ACE happy with ACE includes in it
#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"
//#include "Opcodes.h"
//#include "WorldPacket.h"
#include "LogicSession.h"
//#include "Player.h"
//#include "ObjectMgr.h"
//#include "Group.h"
//#include "Guild.h"
//#include "GuildMgr.h"
#include "gateway.h"
//#include "ObjectAccessor.h"
//#include "BattleGround/BattleGroundMgr.h"
//#include "SocialMgr.h"
//#include "LootMgr.h"

#include <mutex>
#include <deque>
#include <algorithm>
#include <cstdarg>
#include "Opcodes_g.h"

#pragma warning(disable : 4150)

/// LogicSession constructor
LogicSession::LogicSession(LogicSocket* sock, ServerNodeType sec, uint32 node_index)
	: m_Socket(sock), m_node_type(sec), m_node_index(node_index)
{}

/// LogicSession destructor
LogicSession::~LogicSession()
{
    ///- unload player if not unloaded
	//if (_player)
		//LogoutPlayer(true);

    ///- empty incoming packet queue
    for (auto const packet : m_recvQueue)
        delete packet;
}

void LogicSession::SizeError(WorldPacket const& packet, uint32 size) const
{
	sLog.outError("logicsession (node type %u, node index %u) send packet %s (%u) with size " SIZEFMTD " but expected %u (attempt crash server?), skipped",
		GetNodeType(), GetNodeIndex(), LookupOpcodeName(packet.GetOpcode()), packet.GetOpcode(), packet.size(), size);
	
}

///// Get the player name
//char const* LogicSession::GetPlayerName() const
//{
//    return GetPlayer() ? GetPlayer()->GetName() : "<none>";
//}

///// Send a packet to the client
void LogicSession::SendPacket(WorldPacket const* packet)
{
    if (m_Socket->IsClosed())
        return;

#ifdef MANGOS_DEBUG

    // Code for network use statistic
    static uint64 sendPacketCount = 0;
    static uint64 sendPacketBytes = 0;

    static time_t firstTime = time(nullptr);
    static time_t lastTime = firstTime;                     // next 60 secs start time

    static uint64 sendLastPacketCount = 0;
    static uint64 sendLastPacketBytes = 0;

    time_t cur_time = time(nullptr);

    if ((cur_time - lastTime) < 60)
    {
        sendPacketCount += 1;
        sendPacketBytes += packet->size();

        sendLastPacketCount += 1;
        sendLastPacketBytes += packet->size();
    }
    else
    {
        uint64 minTime = uint64(cur_time - lastTime);
        uint64 fullTime = uint64(lastTime - firstTime);
        DETAIL_LOG("Send all time packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f time: %u", sendPacketCount, sendPacketBytes, float(sendPacketCount) / fullTime, float(sendPacketBytes) / fullTime, uint32(fullTime));
        DETAIL_LOG("Send last min packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f", sendLastPacketCount, sendLastPacketBytes, float(sendLastPacketCount) / minTime, float(sendLastPacketBytes) / minTime);

        lastTime = cur_time;
        sendLastPacketCount = 1;
        sendLastPacketBytes = packet->wpos();               // wpos is real written size
    }

#endif                                                  // !MANGOS_DEBUG

    m_Socket->SendPacket(*packet);
}

/// Add an incoming packet to the queue
void LogicSession::QueuePacket(WorldPacket* new_packet)
{
    std::lock_guard<std::mutex> guard(m_recvQueueLock);
    m_recvQueue.push_back(new_packet);
}

/// Logging helper for unexpected opcodes
void LogicSession::LogUnexpectedOpcode(WorldPacket* packet, const char* reason)
{
    sLog.outError("SESSION: received unexpected opcode %s (0x%.4X) %s",
                  LookupOpcodeName(packet->GetOpcode()),
                  packet->GetOpcode(),
                  reason);
}

/// Logging helper for unexpected opcodes
void LogicSession::LogUnprocessedTail(WorldPacket* packet)
{
    sLog.outError("SESSION: opcode %s (0x%.4X) have unprocessed tail data (read stop at " SIZEFMTD " from " SIZEFMTD ")",
                  LookupOpcodeName(packet->GetOpcode()),
                  packet->GetOpcode(),
                  packet->rpos(), packet->wpos());
}

/// Update the LogicSession (triggered by Logic update)
bool LogicSession::Update(PacketFilter& updater)
{
    std::lock_guard<std::mutex> guard(m_recvQueueLock);

    ///- Retrieve packets from the receive queue and call the appropriate handlers
    /// not process packets if socket already closed
    while (m_Socket && !m_Socket->IsClosed() && !m_recvQueue.empty())
    {
        WorldPacket *packet = m_recvQueue.front();
        m_recvQueue.pop_front();

#if 1
		sLog.outError("MOEP: %s (0x%.4X)",
			LookupOpcodeName(packet->GetOpcode()),
			packet->GetOpcode());
#endif

        OpcodeHandler_g const& opHandle = opcodeTable_g[packet->GetOpcode()];
        try
        {
            switch (opHandle.status)
            {
                //case STATUS_LOGGEDIN:
                //    if (!_player)
                //    {
                //        // skip STATUS_LOGGEDIN opcode unexpected errors if player logout sometime ago - this can be network lag delayed packets
                //        if (!m_playerRecentlyLogout)
                //            LogUnexpectedOpcode(packet, "the player has not logged in yet");
                //    }
                //    else if (_player->IsInWorld())
                //        ExecuteOpcode(opHandle, packet);

                //    // lag can cause STATUS_LOGGEDIN opcodes to arrive after the player started a transfer
                //    break;
                //case STATUS_LOGGEDIN_OR_RECENTLY_LOGGEDOUT:
                //    if (!_player && !m_playerRecentlyLogout)
                //    {
                //        LogUnexpectedOpcode(packet, "the player has not logged in yet and not recently logout");
                //    }
                //    else
                //        // not expected _player or must checked in packet hanlder
                //        ExecuteOpcode(opHandle, packet);
                //    break;
                //case STATUS_TRANSFER:
                //    if (!_player)
                //        LogUnexpectedOpcode(packet, "the player has not logged in yet");
                //    else if (_player->IsInWorld())
                //        LogUnexpectedOpcode(packet, "the player is still in Logic");
                //    else
                //        ExecuteOpcode(opHandle, packet);
                //    break;
                case STATUS_AUTHED:
                    // prevent cheating with skip queue wait
                    if (m_inQueue)
                    {
                        LogUnexpectedOpcode(packet, "the player not pass queue yet");
                        break;
                    }

                    // single from authed time opcodes send in to after logout time
                    // and before other STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT opcodes.
                    m_playerRecentlyLogout = false;

                    ExecuteOpcode(opHandle, packet);
                    break;
                case STATUS_NEVER:
                    sLog.outError("SESSION: received not allowed opcode %s (0x%.4X)",
                                  LookupOpcodeName(packet->GetOpcode()),
                                  packet->GetOpcode());
                    break;
                case STATUS_UNHANDLED:
                    DEBUG_LOG("SESSION: received not handled opcode %s (0x%.4X)",
                              LookupOpcodeName(packet->GetOpcode()),
                              packet->GetOpcode());
                    break;
                default:
                    sLog.outError("SESSION: received wrong-status-req opcode %s (0x%.4X)",
                                  LookupOpcodeName(packet->GetOpcode()),
                                  packet->GetOpcode());
                    break;
            }
        }
        catch (ByteBufferException&)
        {
            sLog.outError("LogicSession::Update ByteBufferException occured while parsing a packet (opcode: %u) from client %s, accountid=%i.",
                          packet->GetOpcode(), GetRemoteAddress().c_str(), GetNodeIndex());
            if (sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG))
            {
                DEBUG_LOG("Dumping error causing packet:");
                packet->hexlike();
            }

            if (sConfigMgr.getConfig(G_CFG_BOOL_KICK_PLAYER_ON_BAD_PACKET))
            {
                DETAIL_LOG("Disconnecting session [account id %u / address %s] for badly formatted packet.",
					GetNodeIndex(), GetRemoteAddress().c_str());

                //KickPlayer();
            }
        }

        delete packet;
    }

    // check if we are safe to proceed with logout
    // logout procedure should happen only in Logic::UpdateSessions() method!!!
    if (updater.ProcessLogout())
    {
        ///- If necessary, log the player out
        /*const time_t currTime = time(nullptr);

        if (m_Socket->IsClosed() || (ShouldLogOut(currTime) && !m_playerLoading))
            LogoutPlayer(true);*/

        // finalize the session if disconnected.
        if (m_Socket->IsClosed())
            return false;
    }

    return true;
}
 
/// %Log the player out
//void LogicSession::LogoutPlayer(bool save)
//{
//    // if the player has just logged out, there is no need to do anything here
//    if (m_playerRecentlyLogout)
//        return;
//
//    std::lock_guard<std::mutex> guard(m_logoutMutex);
//
//    // finish pending transfers before starting the logout
//    while (_player && _player->IsBeingTeleportedFar())
//		HandleMoveWorldportAckOpcode();
//
//    m_playerLogout = true;
//    m_playerSave = save;
//
//    if (_player)
//    {
//        sLog.outChar("Account: %d (IP: %s) Logout Character:[%s] (guid: %u)", GetAccountId(), GetRemoteAddress().c_str(), _player->GetName() , _player->GetGUIDLow());
//
//        if (Loot* loot = sLootMgr.GetLoot(_player))
//            loot->Release(_player);
//
//        ///- If the player just died before logging out, make him appear as a ghost
//        // FIXME: logout must be delayed in case lost connection with client in time of combat
//        if (_player->GetDeathTimer())
//        {
//            _player->getHostileRefManager().deleteReferences();
//            _player->BuildPlayerRepop();
//            _player->RepopAtGraveyard();
//        }
//        else if (!_player->getAttackers().empty())
//        {
//            _player->CombatStop();
//            _player->getHostileRefManager().setOnlineOfflineState(false);
//            _player->RemoveAllAurasOnDeath();
//
//            // build set of player who attack _player or who have pet attacking of _player
//            std::set<Player*> aset;
//            for (Unit::AttackerSet::const_iterator itr = _player->getAttackers().begin(); itr != _player->getAttackers().end(); ++itr)
//            {
//                Unit* owner = (*itr)->GetOwner();           // including player controlled case
//                if (owner)
//                {
//                    if (owner->GetTypeId() == TYPEID_PLAYER)
//                        aset.insert(static_cast<Player*>(owner));
//                }
//                else if ((*itr)->GetTypeId() == TYPEID_PLAYER)
//                    aset.insert(static_cast<Player*>(*itr));
//            }
//
//            _player->SetPvPDeath(!aset.empty());
//            _player->KillPlayer();
//            _player->BuildPlayerRepop();
//            _player->RepopAtGraveyard();
//
//            // give honor to all attackers from set like group case
//            for (std::set<Player*>::const_iterator itr = aset.begin(); itr != aset.end(); ++itr)
//                (*itr)->RewardHonor(_player, aset.size());
//
//            // give bg rewards and update counters like kill by first from attackers
//            // this can't be called for all attackers.
//            if (!aset.empty())
//                if (BattleGround* bg = _player->GetBattleGround())
//                    bg->HandleKillPlayer(_player, *aset.begin());
//        }
//        else if (_player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
//        {
//            // this will kill character by SPELL_AURA_SPIRIT_OF_REDEMPTION
//            _player->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);
//            //_player->SetDeathPvP(*); set at SPELL_AURA_SPIRIT_OF_REDEMPTION apply time
//            _player->KillPlayer();
//            _player->BuildPlayerRepop();
//            _player->RepopAtGraveyard();
//        }
//        // drop a flag if player is carrying it
//        if (BattleGround* bg = _player->GetBattleGround())
//            bg->EventPlayerLoggedOut(_player);
//
//        ///- Teleport to home if the player is in an invalid instance
//        if (!_player->m_InstanceValid && !_player->isGameMaster())
//        {
//            _player->TeleportToHomebind();
//            // this is a bad place to call for far teleport because we need player to be in Logic for successful logout
//            // maybe we should implement delayed far teleport logout?
//        }
//
//        // FG: finish pending transfers after starting the logout
//        // this should fix players beeing able to logout and login back with full hp at death position
//        while (_player->IsBeingTeleportedFar())
//			HandleMoveWorldportAckOpcode();
//
//        for (int i = 0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
//        {
//            if (BattleGroundQueueTypeId bgQueueTypeId = _player->GetBattleGroundQueueTypeId(i))
//            {
//                _player->RemoveBattleGroundQueueId(bgQueueTypeId);
//                sBattleGroundMgr.m_BattleGroundQueues[ bgQueueTypeId ].RemovePlayer(_player->GetObjectGuid(), true);
//            }
//        }
//
//        ///- Reset the online field in the account table
//        // no point resetting online in character table here as Player::SaveToDB() will set it to 1 since player has not been removed from Logic at this stage
//        // No SQL injection as AccountID is uint32
//        static SqlStatementID id;
//
//        SqlStatement stmt = LoginDatabase.CreateStatement(id, "UPDATE account SET active_realm_id = ? WHERE id = ?");
//        stmt.PExecute(uint32(0), GetAccountId());
//
//        ///- If the player is in a guild, update the guild roster and broadcast a logout message to other guild members
//        if (Guild* guild = sGuildMgr.GetGuildById(_player->GetGuildId()))
//        {
//            if (MemberSlot* slot = guild->GetMemberSlot(_player->GetObjectGuid()))
//            {
//                slot->SetMemberStats(_player);
//                slot->UpdateLogoutTime();
//            }
//
//            guild->BroadcastEvent(GE_SIGNED_OFF, _player->GetObjectGuid(), _player->GetName());
//        }
//
//        ///- Remove pet
//        _player->RemovePet(PET_SAVE_AS_CURRENT);
//
//        ///- empty buyback items and save the player in the database
//        // some save parts only correctly work in case player present in map/player_lists (pets, etc)
//        if (save)
//            _player->SaveToDB();
//
//        ///- Leave all channels before player delete...
//        _player->CleanupChannels();
//
//        ///- If the player is in a group (or invited), remove him. If the group if then only 1 person, disband the group.
//        _player->UninviteFromGroup();
//
//        // remove player from the group if he is:
//        // a) in group; b) not in raid group; c) logging out normally (not being kicked or disconnected)
//        if (_player->GetGroup() && !_player->GetGroup()->isRaidGroup() && !m_Socket->IsClosed())
//            _player->RemoveFromGroup();
//
//        ///- Send update to group
//        if (_player->GetGroup())
//            _player->GetGroup()->SendUpdate();
//
//        ///- Broadcast a logout message to the player's friends
//        sSocialMgr.SendFriendStatus(_player, FRIEND_OFFLINE, _player->GetObjectGuid(), true);
//        sSocialMgr.RemovePlayerSocial(_player->GetGUIDLow());
//
//        ///- Remove the player from the Logic
//        // the player may not be in the Logic when logging out
//        // e.g if he got disconnected during a transfer to another map
//        // calls to GetMap in this case may cause crashes
//        if (_player->IsInWorld())
//        {
//            Map* _map = _player->GetMap();
//            _map->Remove(_player, true);
//        }
//        else
//        {
//            _player->CleanupsBeforeDelete();
//            Map::DeleteFromWorld(_player);
//        }
//
//        SetPlayer(nullptr);                                    // deleted in Remove/DeleteFromLogic call
//
//        ///- Send the 'logout complete' packet to the client
//        WorldPacket data(SMSG_LOGOUT_COMPLETE, 0);
//        SendPacket(&data);
//
//        ///- Since each account can only have one online character at any given time, ensure all characters for active account are marked as offline
//        // No SQL injection as AccountId is uint32
//
//        static SqlStatementID updChars;
//
//        stmt = CharacterDatabase.CreateStatement(updChars, "UPDATE characters SET online = 0 WHERE account = ?");
//        stmt.PExecute(GetAccountId());
//
//        DEBUG_LOG("SESSION: Sent SMSG_LOGOUT_COMPLETE Message");
//    }
//
//    m_playerLogout = false;
//    m_playerSave = false;
//    m_playerRecentlyLogout = true;
//
//    LogoutRequest(0);
//}
//
///// Kick a player out of the Logic
//void LogicSession::KickPlayer()
//{
//    if (!m_Socket->IsClosed())
//        m_Socket->Close();
//}
//
///// Cancel channeling handler
//
//void LogicSession::SendAreaTriggerMessage(const char* Text, ...)
//{
//    va_list ap;
//    char szStr [1024];
//    szStr[0] = '\0';
//
//    va_start(ap, Text);
//    vsnprintf(szStr, 1024, Text, ap);
//    va_end(ap);
//
//    uint32 length = strlen(szStr) + 1;
//    WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 4 + length);
//    data << length;
//    data << szStr;
//    SendPacket(&data);
//}
//
//void LogicSession::SendNotification(const char* format, ...)
//{
//    if (format)
//    {
//        va_list ap;
//        char szStr [1024];
//        szStr[0] = '\0';
//        va_start(ap, format);
//        vsnprintf(szStr, 1024, format, ap);
//        va_end(ap);
//
//        WorldPacket data(SMSG_NOTIFICATION, (strlen(szStr) + 1));
//        data << szStr;
//        SendPacket(&data);
//    }
//}
//
//void LogicSession::SendNotification(int32 string_id, ...)
//{
//    char const* format = GetMangosString(string_id);
//    if (format)
//    {
//        va_list ap;
//        char szStr [1024];
//        szStr[0] = '\0';
//        va_start(ap, string_id);
//        vsnprintf(szStr, 1024, format, ap);
//        va_end(ap);
//
//        WorldPacket data(SMSG_NOTIFICATION, (strlen(szStr) + 1));
//        data << szStr;
//        SendPacket(&data);
//    }
//}
//
//const char* LogicSession::GetMangosString(int32 entry) const
//{
//    return sObjectMgr.GetMangosString(entry, GetSessionDbLocaleIndex());
//}
//
//void LogicSession::Handle_NULL(WorldPacket& recvPacket)
//{
//    DEBUG_LOG("SESSION: received unimplemented opcode %s (0x%.4X)",
//              recvPacket.GetOpcodeName(),
//              recvPacket.GetOpcode());
//}
//
//void LogicSession::Handle_EarlyProccess(WorldPacket& recvPacket)
//{
//    sLog.outError("SESSION: received opcode %s (0x%.4X) that must be processed in LogicSocket::OnRead",
//                  recvPacket.GetOpcodeName(),
//                  recvPacket.GetOpcode());
//}
//
//void LogicSession::Handle_ServerSide(WorldPacket& recvPacket)
//{
//    sLog.outError("SESSION: received server-side opcode %s (0x%.4X)",
//                  recvPacket.GetOpcodeName(),
//                  recvPacket.GetOpcode());
//}
//
//void LogicSession::Handle_Deprecated(WorldPacket& recvPacket)
//{
//    sLog.outError("SESSION: received deprecated opcode %s (0x%.4X)",
//                  recvPacket.GetOpcodeName(),
//                  recvPacket.GetOpcode());
//}
//
//void LogicSession::SendAuthWaitQue(uint32 position)
//{
//    if (position == 0)
//    {
//        WorldPacket packet(SMSG_AUTH_RESPONSE, 1);
//        packet << uint8(AUTH_OK);
//        SendPacket(&packet);
//    }
//    else
//    {
//        WorldPacket packet(SMSG_AUTH_RESPONSE, 1 + 4);
//        packet << uint8(AUTH_WAIT_QUEUE);
//        packet << uint32(position);
//        SendPacket(&packet);
//    }
//}
//
//void LogicSession::LoadTutorialsData()
//{
//    for (int aX = 0 ; aX < 8 ; ++aX)
//        m_Tutorials[ aX ] = 0;
//
//    QueryResult* result = CharacterDatabase.PQuery("SELECT tut0,tut1,tut2,tut3,tut4,tut5,tut6,tut7 FROM character_tutorial WHERE account = '%u'", GetAccountId());
//
//    if (!result)
//    {
//        m_tutorialState = TUTORIALDATA_NEW;
//        return;
//    }
//
//    do
//    {
//        Field* fields = result->Fetch();
//
//        for (int iI = 0; iI < 8; ++iI)
//            m_Tutorials[iI] = fields[iI].GetUInt32();
//    }
//    while (result->NextRow());
//
//    delete result;
//
//    m_tutorialState = TUTORIALDATA_UNCHANGED;
//}
//
//void LogicSession::SendTutorialsData()
//{
//    WorldPacket data(SMSG_TUTORIAL_FLAGS, 4 * 8);
//    for (uint32 i = 0; i < 8; ++i)
//        data << m_Tutorials[i];
//    SendPacket(&data);
//}
//
//void LogicSession::SaveTutorialsData()
//{
//    static SqlStatementID updTutorial ;
//    static SqlStatementID insTutorial ;
//
//    switch (m_tutorialState)
//    {
//        case TUTORIALDATA_CHANGED:
//        {
//            SqlStatement stmt = CharacterDatabase.CreateStatement(updTutorial, "UPDATE character_tutorial SET tut0=?, tut1=?, tut2=?, tut3=?, tut4=?, tut5=?, tut6=?, tut7=? WHERE account = ?");
//            for (int i = 0; i < 8; ++i)
//                stmt.addUInt32(m_Tutorials[i]);
//
//            stmt.addUInt32(GetAccountId());
//            stmt.Execute();
//        }
//        break;
//
//        case TUTORIALDATA_NEW:
//        {
//            SqlStatement stmt = CharacterDatabase.CreateStatement(insTutorial, "INSERT INTO character_tutorial (account,tut0,tut1,tut2,tut3,tut4,tut5,tut6,tut7) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
//
//            stmt.addUInt32(GetAccountId());
//            for (int i = 0; i < 8; ++i)
//                stmt.addUInt32(m_Tutorials[i]);
//
//            stmt.Execute();
//        }
//        break;
//        case TUTORIALDATA_UNCHANGED:
//            break;
//    }
//
//    m_tutorialState = TUTORIALDATA_UNCHANGED;
//}
//
//// Send chat information about aborted transfer (mostly used by Player::SendTransferAbortedByLockstatus())
//void LogicSession::SendTransferAborted(uint32 mapid, uint8 reason, uint8 arg)
//{
//    WorldPacket data(SMSG_TRANSFER_ABORTED, 4 + 2);
//    data << uint32(mapid);
//    data << uint8(reason);                                  // transfer abort reason
//    data << uint8(0);                                       // arg. not used
//    SendPacket(&data);
//}

void LogicSession::ExecuteOpcode(OpcodeHandler_g const& opHandle, WorldPacket* packet)
{
    // need prevent do internal far teleports in handlers because some handlers do lot steps
    // or call code that can do far teleports in some conditions unexpectedly for generic way work code
    //if (_player)
    //    _player->SetCanDelayTeleport(true);

    //(this->*opHandle.handler)(*packet);

    //if (_player)
    //{
    //    // can be not set in fact for login opcode, but this not create porblems.
    //    _player->SetCanDelayTeleport(false);

    //    // we should execute delayed teleports only for alive(!) players
    //    // because we don't want player's ghost teleported from graveyard
    //    if (_player->IsHasDelayedTeleport())
    //        _player->TeleportTo(_player->m_teleport_dest, _player->m_teleport_options);
    //}

    if (packet->rpos() < packet->wpos() && sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG))
        LogUnprocessedTail(packet);
}

//void LogicSession::SendPlaySpellVisual(ObjectGuid guid, uint32 spellArtKit)
//{
//    WorldPacket data(SMSG_PLAY_SPELL_VISUAL, 8 + 4);        // visual effect on guid
//    data << guid;
//    data << spellArtKit;                                    // index from SpellVisualKit.dbc
//    SendPacket(&data);
//}
