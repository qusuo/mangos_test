
#include "SessionMgr.h"
#include "..\network\DBServerSession.h"
#include <algorithm>
#include "Log.h"
//#include "..\dbser.h"
#include "..\network\SessionFilter.h"
#include "Protocol\ss_protocol_def.h"


SessionMgr::SessionMgr()
{
	memset(m_sessions, 0, sizeof(m_sessions));
	m_maxActiveSessionCount = 0;
}


SessionMgr::~SessionMgr()
{
	for (int i = SERVER_NODE_TYPE_INVALID + 1; i < SERVER_NODE_TYPE_COUNT; ++i)
	{
		for (int j = 0; j < MAX_NODE_SERVER; ++j)
		{
			if (m_sessions[i][j] != nullptr)
			{
				delete m_sessions[i][j];
			}
		}
	}

	for (auto const session : m_sessionAddQueue)
		delete session;
}

bool SessionMgr::IsExistSession(uint32 node_type, uint32 node_index) const
{
	if (node_type >= SERVER_NODE_TYPE_COUNT || node_index >= MAX_NODE_SERVER)
		return false;

	return m_sessions[node_type][node_index] == NULL ? false : true;
}

/// Remove a given session
bool SessionMgr::RemoveSession(uint32 index, uint32 type)
{
	///- Find the session, kick the user, but we can't delete session at this moment to prevent iterator invalidation
	if (!IsExistSession(type, index)) return true;

	if (m_sessions[type][index]->PlayerLoading())
		return false;
	m_sessions[type][index]->KickPlayer();

	return true;
}

void SessionMgr::AddSession(WorldSession* s)
{
	if (IsExistSession(s->GetNodeType(), s->GetNodeIndex())) return;

	std::lock_guard<std::mutex> guard(m_sessionAddQueueLock);

	m_sessionAddQueue.push_back(s);
}

uint32 SessionMgr::GetActiveAndQueuedSessionCount() const
{
	uint32 nCount = 0;
	for (int i = SERVER_NODE_TYPE_INVALID + 1; i < SERVER_NODE_TYPE_COUNT; ++i)
	{
		for (int j = 0; j < MAX_NODE_SERVER; ++j)
		{
			if (m_sessions[i][j] != nullptr)
			{
				++nCount;
			}
		}
	}

	return nCount;
}

uint32 SessionMgr::GetActiveSessionCount() const
{
	return GetActiveAndQueuedSessionCount();
}

/// Find a session by its id
WorldSession* SessionMgr::FindSession(uint32 id, uint32 node_type) const
{
	if (!IsExistSession(node_type, id)) return nullptr;

	return m_sessions[node_type][id];
}

//uint32 SessionMgr::GetActiveSessionCount() const 
//{ 
//	return GetActiveAndQueuedSessionCount() - m_QueuedSessions.size();
//	//return m_sessions.size() - m_QueuedSessions.size(); 
//}


//uint32 SessionMgr::GetQueuedSessionCount() const 
//{ 
//	return m_QueuedSessions.size(); 
//}


void SessionMgr::AddSession_(WorldSession* s)
{
	MANGOS_ASSERT(s);

	// if session already exist, prepare to it deleting at next LogicServer update
	// NOTE - KickPlayer() should be called on "old" in RemoveSession()
	WorldSession* old = FindSession(s->GetNodeType(), s->GetNodeIndex());
	if (old){
		return;
	}

	m_sessions[s->GetNodeType()][s->GetNodeIndex()] = s;

	uint32 Sessions = GetActiveAndQueuedSessionCount();
	//uint32 pLimit = GetPlayerAmountLimit();
	uint32 QueueSize = 0;// GetQueuedSessionCount();             // number of players in the queue

	AddQueuedSession(s);
	UpdateMaxSessionCounters();
	DETAIL_LOG("LogicServerQueue: server id %u type %u is in Queue Position (%u).", s->GetNodeIndex(), s->GetNodeType(), ++QueueSize);
	return;
}


int32 SessionMgr::GetQueuedSessionPos(WorldSession* sess)
{
	return 0;
}

void SessionMgr::AddQueuedSession(WorldSession* sess)
{
	// register respone
	WorldPacket packet(SMSG_REGISTER_RET);
	packet << sConfigMgr.getConfig(CONFIG_UINT32_NODE_TYPE);
	packet << sConfigMgr.getConfig(CONFIG_UINT32_NODE_INDEX);
	packet << uint8(RESPONSE_SUCCESS);

	sess->SendPacket(&packet);
}

bool SessionMgr::RemoveQueuedSession(WorldSession* sess)
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
	for (int i = SERVER_NODE_TYPE_INVALID + 1; i < SERVER_NODE_TYPE_COUNT; ++i)
	{
		for (int j = 0; j < MAX_NODE_SERVER; ++j)
		{
			if (m_sessions[i][j] != nullptr)
			{
				m_sessions[i][j]->KickPlayer();
			}
		}
	}
}

void SessionMgr::UpdateSessions()
{
	///- Add new sessions
	{
		std::lock_guard<std::mutex> guard(m_sessionAddQueueLock);

		std::for_each(m_sessionAddQueue.begin(), m_sessionAddQueue.end(), [&](WorldSession *session) { AddSession_(session); });

		m_sessionAddQueue.clear();
	}

	///- Then send an update signal to remaining ones
	for (int i = SERVER_NODE_TYPE_INVALID + 1; i < SERVER_NODE_TYPE_COUNT; ++i)
	{
		for (int j = 0; j < MAX_NODE_SERVER; ++j)
		{
			if (m_sessions[i][j] != nullptr)
			{
				WorldSession* pSession = m_sessions[i][j];
				WorldSessionFilter updater(pSession);

				// the session itself is owned by the socket which created it.  that is where the destruction of the session will happen.
				if (!pSession->Update(updater))
				{
					RemoveQueuedSession(pSession);
					pSession->Finalize();
					m_sessions[i][j] = nullptr;
				}
			}
		}
	}
}