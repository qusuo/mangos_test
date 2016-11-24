// player 2 gateway server session manage

#ifndef __SESSIONMGR_H
#define __SESSIONMGR_H

#include "Platform/Define.h"
#include "Protocol/protocol_def.h"
#include <mutex>
#include <deque>
#include <list>
#include <unordered_map>

class DBServerSession;
class DBServerSession;

class SessionMgr
{
public:
	SessionMgr();
	~SessionMgr();

	void UpdateSessions();
	//WorldSession* GetSession() const { return m_session; }
	DBServerSession* FindSession(uint32 id) const;
	void AddSession(DBServerSession* s);
	bool RemoveSession(uint32 id);
	/// Get the number of current active sessions
	void UpdateMaxSessionCounters();

	void KickAll();

	// player Queue
	typedef std::list<DBServerSession*> Queue;
	void AddQueuedSession(DBServerSession*);
	bool RemoveQueuedSession(DBServerSession* session);
	int32 GetQueuedSessionPos(DBServerSession*);

	//get
	uint32 GetActiveAndQueuedSessionCount() const { return m_sessions.size(); }
	uint32 GetActiveSessionCount() const { return m_sessions.size(); }
	uint32 GetMaxActiveSessionCount() const { return m_maxActiveSessionCount; }

	DBServerSession * GetDBServerSession() const { return m_DBSession; }
	//Queue &GetQueuedSessions(){ return m_QueuedSessions; }
	//std::mutex &GetSessionAddQueueLock(){ return m_sessionAddQueueLock; }
	//std::deque<WorldSession *> & GetSessionAddQueue(){ return m_sessionAddQueue; }

	//set
	void SetDBServerSession(DBServerSession * sess) { if (sess) m_DBSession = sess; }

private:
	// sessions that are added async
	void AddSession_(DBServerSession* s);

private:
	//先更新到添加队列，再次心跳时再添加取用户列表
	std::mutex m_sessionAddQueueLock;
	std::deque<DBServerSession *> m_sessionAddQueue;

	typedef std::unordered_map<uint32, DBServerSession*> SessionMap;
	SessionMap m_sessions; //player session
	uint32 m_maxActiveSessionCount;

	// all server session
	DBServerSession * m_DBSession;
	//DBServerSession * m_sessions[SERVER_NODE_TYPE_COUNT][MAX_NODE_SERVER];
};

#define sSessionMgr MaNGOS::Singleton<SessionMgr>::Instance()

#endif //__SESSIONMGR_H