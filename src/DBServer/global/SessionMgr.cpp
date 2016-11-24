
#include "SessionMgr.h"
#include <algorithm>
#include "Log.h"
#include "..\network\SessionFilter.h"
#include "Protocol\ss_protocol_def.h"
#include "network\DBServerSession.h"
#include "dbserver/DBServer.h"
#include "network\logicSession.h"
#include "Util.h"


SessionMgr::SessionMgr()
{
	//memset(m_sessions, 0, sizeof(m_sessions));
	m_maxActiveSessionCount = 0;
	m_DBSession = nullptr;
}


SessionMgr::~SessionMgr()
{
	///- Empty the kicked session set
	for (auto const session : m_sessions)
		delete session.second;

	for (auto const session : m_sessionAddQueue)
		delete session;
}

//bool SessionMgr::IsExistSession(uint32 node_type, uint32 node_index) const
//{
//	if (node_type >= SERVER_NODE_TYPE_COUNT || node_index >= MAX_NODE_SERVER)
//		return false;
//
//	return m_sessions[node_type][node_index] == NULL ? false : true;
//}

/// Remove a given session
bool SessionMgr::RemoveSession(uint32 id)
{
	///- Find the session, kick the user, but we can't delete session at this moment to prevent iterator invalidation
	//delete when update session
	SessionMap::const_iterator itr = m_sessions.find(id);

	if (itr != m_sessions.end() && itr->second)
	{
		if (itr->second->PlayerLoading())
			return false;
		itr->second->KickPlayer();
	}

	return true;
}

void SessionMgr::AddSession(DBServerSession* s)
{
	std::lock_guard<std::mutex> guard(m_sessionAddQueueLock);

	m_sessionAddQueue.push_back(s);
}


/// Find a session by its id
DBServerSession* SessionMgr::FindSession(uint32 id) const
{
	SessionMap::const_iterator itr = m_sessions.find(id);

	if (itr != m_sessions.end())
		return itr->second;                                 // also can return nullptr for kicked session
	else
		return nullptr;
}

void SessionMgr::AddSession_(DBServerSession* s)
{
	MANGOS_ASSERT(s);

	// NOTE - Still there is race condition in WorldSession* being used in the Sockets

	///- kick already loaded player with same account (if any) and remove session
	///- if player is in loading and want to load again, return
	if (!RemoveSession(s->GetAccountId()))
	{
		s->KickPlayer();
		delete s;                                           // session not added yet in session list, so not listed in queue
		return;
	}

	
	m_sessions[s->GetAccountId()] = s;

	uint32 Sessions = GetActiveAndQueuedSessionCount();
	uint32 pLimit = sDBServer.GetPlayerAmountLimit();

	if (pLimit > 0 && Sessions >= pLimit && s->GetSecurity() == SEC_PLAYER)
	{
		AddQueuedSession(s);
		UpdateMaxSessionCounters();
		DETAIL_LOG("PlayerQueue: Account id %u is in Queue Position (%u).", s->GetAccountId(), ++Sessions);
		return;
	}

	//check user info 后续登陆服完成
	// Checked for 1.12.2
	//WorldPacket packet(SMSG_AUTH_RESPONSE, 1 + 4 + 1 + 4);
	//packet << uint8(AUTH_OK);
	//packet << uint32(0);                                    // BillingTimeRemaining
	//packet << uint8(0);                                     // BillingPlanFlags
	//packet << uint32(0);                                    // BillingTimeRested
	//s->SendPacket(&packet);

	//UpdateMaxSessionCounters();

	//// Updates the population
	//if (pLimit > 0)
	//{
	//	float popu = float(GetActiveSessionCount());        // updated number of users on the server
	//	popu /= pLimit;
	//	popu *= 2;

	//	static SqlStatementID id;

	//	SqlStatement stmt = LoginDatabase.CreateStatement(id, "UPDATE realmlist SET population = ? WHERE id = ?");
	//	stmt.PExecute(popu, realmID);

	//	DETAIL_LOG("Server Population (%f).", popu);
	//}
}


int32 SessionMgr::GetQueuedSessionPos(DBServerSession* sess)
{
	return 0;
}

void SessionMgr::AddQueuedSession(DBServerSession* sess)
{
	// login respone
	/*WorldPacket packet(SMSG_REGISTER_RET);
	packet << sess->GetNodeType();
	packet << sess->GetNodeIndex();
	packet << uint8(RESPONSE_SUCCESS);

	sess->SendPacket(&packet);*/
}

bool SessionMgr::RemoveQueuedSession(DBServerSession* sess)
{
	bool found = false;
	return found;
}

void SessionMgr::UpdateMaxSessionCounters()
{
	m_maxActiveSessionCount = std::max(m_maxActiveSessionCount, GetActiveAndQueuedSessionCount());
}

void SessionMgr::KickAll()
{
	// session not removed at kick and will removed in next update tick
	for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
		itr->second->KickPlayer();
}

void SessionMgr::UpdateSessions()
{
	///- Add new sessions
	{
		std::lock_guard<std::mutex> guard(m_sessionAddQueueLock);

		std::for_each(m_sessionAddQueue.begin(), m_sessionAddQueue.end(), [&](DBServerSession *session) { AddSession_(session); });

		m_sessionAddQueue.clear();
	}

	///- Then send an update signal to remaining ones
	for (SessionMap::iterator itr = m_sessions.begin(), next; itr != m_sessions.end(); itr = next)
	{
		next = itr;
		++next;
		///- and remove not active sessions from the list
		DBServerSession* pSession = itr->second;
		WorldSessionFilter updater(pSession);

		// the session itself is owned by the socket which created it.  that is where the destruction of the session will happen.
		if (!pSession->Update(updater))
		{
			RemoveQueuedSession(pSession);
			m_sessions.erase(itr);
			pSession->Finalize();
		}
	}

	//connecter 
	if (m_DBSession)
	{
		WorldSessionFilter updater_logic(m_DBSession);

		if (!m_DBSession->Update(updater_logic))
		{
			m_DBSession->Finalize();
			m_DBSession = nullptr;
		}
		else
		{
			return;
			uint32 ping = irand();
			WorldPacket data(CMSG_PING, 50);              // guess size
			data << ping;

			m_DBSession->SendPacket(&data);
		}
	}
}