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

/// \addtogroup u2w
/// @{
/// \file

#ifndef __DBSERVERSESSION_H
#define __DBSERVERSESSION_H

#include "Common.h"
//#include "SharedDefines.h"
//#include "ObjectGuid.h"
//#include "AuctionHouseMgr.h"
//#include "Item.h"
#include "DBServerSocket.h"

#include <deque>
#include <mutex>
#include "Network\NetDef.h"
#include "Network\BaseSession.h"

class Item;
class Player;
class WorldPacket;
class QueryResult;
class LoginQueryHolder;
class PacketFilter;
struct OpcodeHandler_l;


/// Player session in the World
class MANGOS_DLL_SPEC DBServerSession : public BaseSession
{
	friend class CharacterHandler;

public:
	DBServerSession(uint32 id, DBServerSocket* sock, ServerNodeType sec);
	~DBServerSession();

	//主动
	bool PlayerLoading() const { return m_playerLoading; }
	bool PlayerLogout() const { return m_playerLogout; }
	bool PlayerLogoutWithSave() const { return m_playerLogout && m_playerSave; }

	/// Session in auth.queue currently
	void SetInQueue(bool state) { m_inQueue = state; }

	/// Is the user engaged in a log out process?
	bool isLogingOut() const { return _logoutTime || m_playerLogout; }

	/// Engage the logout process for the user
	void LogoutRequest(time_t requestTime)
	{
		_logoutTime = requestTime;
	}

	/// Is logout cooldown expired?
	bool ShouldLogOut(time_t currTime) const
	{
		return (_logoutTime > 0 && currTime >= _logoutTime + 20);
	}

	//被动
	void LogoutPlayer(bool save);
	void KickPlayer();

	void QueuePacket(WorldPacket* new_packet);

	bool Update(PacketFilter& updater);
	/// Handle the authentication waiting queue (to be completed)
	void SendAuthWaitQue(uint32 position);


	// marks this session as finalized in the socket which owns it.  this lets
	// the socket handling code know that the session can be safely deleted
	void Finalize() { m_Socket->FinalizeSession(); }

	void SizeError(WorldPacket const& packet, uint32 size) const;

	void SendPacket(WorldPacket const* packet);
	void SendNotification(const char* format, ...) ATTR_PRINTF(2, 3);
	void SendNotification(int32 string_id, ...);
	void SendPetNameInvalid(uint32 error, const std::string& name);
	void SendPartyResult(PartyOperation operation, const std::string& member, PartyResult res);
	void SendAreaTriggerMessage(const char* Text, ...) ATTR_PRINTF(2, 3);
	void SendTransferAborted(uint32 mapid, uint8 reason, uint8 arg = 0);
	void SendQueryTimeResponse();

	//get
	virtual BasePlayer* GetPlayer() const { return (BasePlayer*)_player; }
	char const* GetPlayerName() const;

	uint32 GetNodeType() const { return m_node_type; }
	uint32 GetNodeIndex() const { return m_node_index; }

	/// Get the active session server limit (or security level limitations)
	//uint32 GetPlayerAmountLimit() const { return m_playerLimit >= 0 ? m_playerLimit : 0; }
	//AccountTypes GetPlayerSecurityLimit() const { return m_playerLimit <= 0 ? AccountTypes(-m_playerLimit) : SEC_PLAYER; }

	//set
	//void SetSecurity(ServerNodeType security) { m_node_type = security; }
	const std::string &GetRemoteAddress() const { return m_Socket->GetRemoteAddress(); }
	void SetPlayer(Player* plr) { _player = plr; }
	const char* GetMangosString(int32 entry) const;
	void SetNodeType(ServerNodeType node_type) { m_node_type = node_type; }
	void SetNodeIndex(uint32 node_index) { m_node_index = node_index; }

	// opcodes handlers
	void Handle_NULL(WorldPacket& recvPacket);          // not used
	void Handle_EarlyProccess(WorldPacket& recvPacket); // just mark packets processed in DBServerSocket::OnRead
	void Handle_ServerSide(WorldPacket& recvPacket);    // sever side only, can't be accepted from client
	void Handle_Deprecated(WorldPacket& recvPacket);    // never used anymore by client

	void HandleCharEnumOpcode(WorldPacket& recvPacket);
	void HandleCharDeleteOpcode(WorldPacket& recvPacket);
	void HandleCharCreateOpcode(WorldPacket& recvPacket);
	void HandlePlayerLoginOpcode(WorldPacket& recvPacket);
	void HandleCharEnum(QueryResult* result);
	void HandlePlayerLogin(LoginQueryHolder* holder);

public:
	// Account mute time
	time_t m_muteTime;

private:
	// private trade methods
	void moveItems(Item* myItems[], Item* hisItems[]);
	/*bool VerifyMovementInfo(MovementInfo const& movementInfo, ObjectGuid const& guid) const;
	bool VerifyMovementInfo(MovementInfo const& movementInfo) const;
	void HandleMoverRelocation(MovementInfo& movementInfo);*/

	void ExecuteOpcode(OpcodeHandler_l const& opHandle, WorldPacket* packet);

	// logging helper
	void LogUnexpectedOpcode(WorldPacket* packet, const char* reason);
	void LogUnprocessedTail(WorldPacket* packet);

	std::mutex m_logoutMutex;                           // this mutex is necessary to avoid two simultaneous logouts due to a valid logout request and socket error
	Player * _player;
	DBServerSocket * const m_Socket;                       // socket pointer is owned by the network thread which created 

	//uint32 _accountId;

	time_t _logoutTime;
	bool m_inQueue;                                     // session wait in auth.queue
	bool m_playerLoading;                               // code processed in LoginPlayer

	// True when the player is in the process of logging out (DBServerSession::LogoutPlayer is currently executing)
	bool m_playerLogout;
	bool m_playerRecentlyLogout;
	bool m_playerSave;                                  // code processed in LogoutPlayer with save request
	LocaleConstant m_sessionDbcLocale;
	int m_sessionDbLocaleIndex;
	uint32 m_latency;

	uint32 m_Tutorials[8];
	//TutorialDataState m_tutorialState;

	std::mutex m_recvQueueLock;
	std::deque<WorldPacket *> m_recvQueue;

	//int32 m_playerLimit;

	//self info
	ServerNodeType m_node_type;
	uint32 m_node_index;
};

#endif
/// @}
