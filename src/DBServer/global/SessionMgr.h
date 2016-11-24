
#ifndef __SESSIONMGR_H
#define __SESSIONMGR_H

#include "Platform/Define.h"
#include "Protocol/protocol_def.h"
#include <mutex>
#include <deque>
#include <list>



class DBServerSession;


class SessionMgr
{
public:
	SessionMgr();
	~SessionMgr();

	void UpdateSessions();
	bool IsExistSession(uint32 node_type, uint32 node_index) const;
	//DBServerSession* GetSession() const { return m_session; }
	DBServerSession* FindSession(uint32 id, uint32 node_type) const;
	void AddSession(DBServerSession* s);
	bool RemoveSession(uint32 index, uint32 type);
	/// Get the number of current active sessions
	void UpdateMaxSessionCounters();

	void KickAll();

	// player Queue
	typedef std::list<DBServerSession*> Queue;
	void AddQueuedSession(DBServerSession*);
	bool RemoveQueuedSession(DBServerSession* session);
	int32 GetQueuedSessionPos(DBServerSession*);

	//get
	
	uint32 GetActiveAndQueuedSessionCount() const;
	uint32 GetActiveSessionCount() const;
	uint32 GetMaxActiveSessionCount() const { return m_maxActiveSessionCount; }

	//Queue &GetQueuedSessions(){ return m_QueuedSessions; }
	//std::mutex &GetSessionAddQueueLock(){ return m_sessionAddQueueLock; }
	//std::deque<DBServerSession *> & GetSessionAddQueue(){ return m_sessionAddQueue; }

private:
	// sessions that are added async
	void AddSession_(DBServerSession* s);

private:
	//先更新到添加队列，再次心跳时再添加取服务器列表
	std::mutex m_sessionAddQueueLock;
	std::deque<DBServerSession *> m_sessionAddQueue;

	//typedef std::unordered_map<uint32, DBServerSession*> SessionMap;
	//SessionMap m_sessions;
	uint32 m_maxActiveSessionCount;

	// all server session
	DBServerSession * m_sessions[SERVER_NODE_TYPE_COUNT][MAX_NODE_SERVER];
};

#define sSessionMgr MaNGOS::Singleton<SessionMgr>::Instance()

#endif //__SESSIONMGR_H