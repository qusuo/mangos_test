#ifndef __SESSIONFILTER_H
#define __SESSIONFILTER_H

class BaseSession;
class WorldPacket;

class PacketFilter
{
public:
	explicit PacketFilter(BaseSession* pSession) : m_pSession(pSession) {}
	virtual ~PacketFilter() {}

	virtual bool Process(WorldPacket* /*packet*/) { return true; }
	virtual bool ProcessLogout() const { return true; }

protected:
	BaseSession* const m_pSession;
};
// process only thread-safe packets in Map::Update()
class MapSessionFilter : public PacketFilter
{
public:
	explicit MapSessionFilter(BaseSession* pSession) : PacketFilter(pSession) {}
	~MapSessionFilter() {}

	virtual bool Process(WorldPacket* packet) override;
	// in Map::Update() we do not process player logout!
	virtual bool ProcessLogout() const override { return false; }
};

// class used to filer only thread-unsafe packets from queue
// in order to update only be used in World::UpdateSessions()
class WorldSessionFilter : public PacketFilter
{
public:
	explicit WorldSessionFilter(BaseSession* pSession) : PacketFilter(pSession) {}
	~WorldSessionFilter() {}

	virtual bool Process(WorldPacket* packet) override;
};

#endif //__SESSIONFILTER_H
