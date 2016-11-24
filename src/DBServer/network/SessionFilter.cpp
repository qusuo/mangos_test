#include "SessionFilter.h"
#include "global/Opcodes_d.h"
#include "Network\BaseSession.h"
#include "Network\BasePlayer.h"
#include "Network\WorldPacket.h"



// select opcodes appropriate for processing in Map::Update context for current session state
static bool MapSessionFilterHelper(BaseSession* session, OpcodeHandler_g const& opHandle)
{
	// we do not process thread-unsafe packets
	if (opHandle.packetProcessing == PROCESS_THREADUNSAFE)
		return false;

	// we do not process not loggined player packets
	BasePlayer* plr = session->GetPlayer();
	if (!plr)
		return false;

	// in Map::Update() we do not process packets where player is not in world!
	return plr->IsInWorld();
}


bool MapSessionFilter::Process(WorldPacket* packet)
{
	OpcodeHandler_g const& opHandle = opcodeTable_g[packet->GetOpcode()];
	if (opHandle.packetProcessing == PROCESS_INPLACE)
		return true;

	// let's check if our opcode can be really processed in Map::Update()
	return MapSessionFilterHelper(m_pSession, opHandle);
}


// we should process ALL packets when player is not in world/logged in
// OR packet handler is not thread-safe!
bool WorldSessionFilter::Process(WorldPacket* packet)
{
	OpcodeHandler_g const& opHandle = opcodeTable_g[packet->GetOpcode()];
	// check if packet handler is supposed to be safe
	if (opHandle.packetProcessing == PROCESS_INPLACE)
		return true;

	// let's check if our opcode can't be processed in Map::Update()
	return !MapSessionFilterHelper(m_pSession, opHandle);
}