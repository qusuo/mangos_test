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

#include "LogicSocket.h"
#include "Common.h"

#include "Util.h"
//#include "WorldPacket.h"
//#include "SharedDefines.h"
#include "ByteBuffer.h"
//#include "AddonHandler.h"
#include "Opcodes_g.h"
#include "Database/DatabaseEnv.h"
#include "Auth/Sha1.h"
#include "Log.h"
#include "LogicSession.h"
#include "gateway.h"

#include <chrono>
#include <functional>

#include <boost/asio.hpp>
#include "Protocol/ss_protocol_def.h"
#include "Network/WorldPacket.h"
#include "global/SessionMgr.h"

LogicSocket::LogicSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler)
	: Socket(service, closeHandler), m_lastPingTime(std::chrono::system_clock::time_point::min()), m_overSpeedPings(0),
	m_useExistingHeader(false), m_session(nullptr), m_sessionFinalized(false), m_seed(urand()), m_pings(0)
{}

LogicSocket::~LogicSocket()
{
	delete m_session;
	m_session = NULL;
}

void LogicSocket::SendPacket(const WorldPacket& pct, bool immediate)
{
	if (IsClosed())
		return;

	// Dump outgoing packet.
	sLog.outWorldPacketDump(GetRemoteEndpoint().c_str(), pct.GetOpcode(), LookupOpcodeName(pct.GetOpcode()), &pct, false);

	prot::SSHeader header;

	header.cmd = pct.GetOpcode();
	EndianConvert(header.cmd);
	
	header.cmd_type = pct.GetOpcodeType();
	EndianConvert(header.cmd_type);

	header.tar_node_index = pct.GetNodeIndex();
	EndianConvert(header.tar_node_index);

	header.tar_node_type = pct.GetNodeType();
	EndianConvert(header.tar_node_type);

	header.size = static_cast<uint16>(pct.size() + 4);
	EndianConvertReverse(header.size);

	m_crypt.EncryptSend(reinterpret_cast<uint8 *>(&header), sizeof(header));

	Write(reinterpret_cast<const char *>(&header), sizeof(header));

	if (!!pct.size())
		Write(reinterpret_cast<const char *>(pct.contents()), pct.size());

	if (immediate)
		ForceFlushOut();
}

bool LogicSocket::Open()
{
	if (!Socket::Open())
		return false;

	// Send regist packet.
	WorldPacket packet(SMSG_REGISTER, 100);
	packet << m_seed;

	packet << sConfigMgr.getConfig(G_CFG_UINT32_NODE_TYPE);

	//BigNumber seed1;
	//seed1.SetRand(16 * 8);
	//packet.append(seed1.AsByteArray(16), 16);               // new encryption seeds

	packet << sConfigMgr.getConfig(G_CFG_UINT32_NODE_INDEX);
	
	//BigNumber seed2;
	//seed2.SetRand(16 * 8);
	//packet.append(seed2.AsByteArray(16), 16);               // new encryption seeds

	packet << sConfigMgr.GetServerName();

	SendPacket(packet);

	return true;
}

bool LogicSocket::ProcessIncomingData()
{
	prot::SSHeader header;

	if (m_useExistingHeader)
	{
		m_useExistingHeader = false;
		header = m_existingHeader;

		ReadSkip(sizeof(prot::SSHeader));
	}
	else
	{
		if (!Read((char *)&header, sizeof(prot::SSHeader)))
		{
			errno = EBADMSG;
			return false;
		}

		//m_crypt.DecryptRecv((uint8 *)&header, sizeof(prot::SSHeader));

		EndianConvertReverse(header.size);
		EndianConvert(header.cmd);
	}

	// there must always be at least four bytes for the opcode,
	// and 0x2800 is the largest supported buffer in the client
	if ((header.size < 4) || (header.size > MAX_SOCKET_BUF) || (header.cmd >= SNUM_SMSG_TYPES))
	{
		sLog.outError("LogicSocket::ProcessIncomingData: client sent malformed packet size = %u , cmd = %u", header.size, header.cmd);

		errno = EINVAL;
		return false;
	}

	// the minus four is because we've already read the four byte opcode value
	const uint16 validBytesRemaining = header.size - 4;

	// check if the client has told us that there is more data than there is
	if (validBytesRemaining > ReadLengthRemaining())
	{
		// we must preserve the decrypted header so as not to corrupt the crypto state, and to prevent duplicating work
		m_useExistingHeader = true;
		m_existingHeader = header;

		// we move the read pointer backward because it will be skipped again later.  this is a slight kludge, but to solve
		// it more elegantly would require introducing protocol awareness into the socket library, which we want to avoid
		ReadSkip(-static_cast<int>(sizeof(prot::SSHeader)));

		errno = EBADMSG;
		return false;
	}

	Opcodes_g x;
	const OpcodesList_s opcode = static_cast<OpcodesList_s>(header.cmd);

	if (IsClosed())
		return false;

	WorldPacket *pct = new WorldPacket(opcode, validBytesRemaining);

	if (validBytesRemaining)
	{
		pct->append(InPeak(), validBytesRemaining);
		ReadSkip(validBytesRemaining);
	}

	sLog.outWorldPacketDump(GetRemoteEndpoint().c_str(), pct->GetOpcode(), LookupOpcodeName(pct->GetOpcode()), pct, true);

	try
	{
		switch (opcode)
		{
		case SMSG_REGISTER_RET:
			if (m_session)
			{
				sLog.outError("LogicSocket::ProcessIncomingData: Player send CMSG_AUTH_SESSION again");
				return false;
			}

			return Regist2ServerRet(*pct);

		case SMSG_PONG:
			return HandlePing(*pct);

			/*case CMSG_KEEP_ALIVE:
				DEBUG_LOG("CMSG_KEEP_ALIVE ,size: " SIZEFMTD " ", pct->size());

				return true;*/

		default:
		{
			if (!m_session)
			{
				sLog.outError("LogicSocket::ProcessIncomingData: Client not authed opcode = %u", uint32(opcode));
				return false;
			}

			//m_session->QueuePacket(pct);

			return true;
		}
		}
	}
	catch (ByteBufferException&)
	{
		sLog.outError("LogicSocket::ProcessIncomingData ByteBufferException occured while parsing an instant handled packet (opcode: %u) from client %s, accountid=%i.",
			opcode, GetRemoteAddress().c_str(), m_session ? 0 : -1);

		if (sLog.HasLogLevelOrHigher(LOG_LVL_DEBUG))
		{
			DEBUG_LOG("Dumping error-causing packet:");
			pct->hexlike();
		}

		if (sConfigMgr.getConfig(G_CFG_BOOL_KICK_PLAYER_ON_BAD_PACKET))
		{
			DETAIL_LOG("Disconnecting session [account id %i / address %s] for badly formatted packet.",
				m_session ? 0 : -1, GetRemoteAddress().c_str());

			return false;
		}
		return false;
	}

	return true;
}

bool LogicSocket::HandlePing(WorldPacket &recvPacket)
{
	uint32 ping;

	// Get the ping packet content
	recvPacket >> ping;

	if (m_lastPingTime == std::chrono::system_clock::time_point::min())
		m_lastPingTime = std::chrono::system_clock::now();              // for 1st ping
	else
	{
		auto now = std::chrono::system_clock::now();
		std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastPingTime);
		m_lastPingTime = now;

		if (seconds.count() > 27)
		{
			++m_overSpeedPings;

			const uint32 max_count = sConfigMgr.getConfig(G_CFG_UINT32_MAX_OVERSPEED_PINGS);

			if (max_count && m_overSpeedPings > max_count)
			{
				if (m_session/* && m_session->GetSecurity() == SEC_PLAYER*/)
				{
					sLog.outError("LogicSocket::HandlePing: logicserver kicked for "
						"overspeeded pings address = %s",
						GetRemoteAddress().c_str());

					return false;
				}
			}
		}
		else
			m_overSpeedPings = 0;
	}

	// critical section
	{
		if (m_session)
		{
			/*m_session->SetLatency(latency);
			m_session->ResetClientTimeDelay();*/
		}
		else
		{
			sLog.outError("LogicSocket::HandlePing: peer sent CMSG_PING, "
				"but is not authenticated or got recently kicked,"
				" address = %s",
				GetRemoteAddress().c_str());
			return false;
		}
	}

	/*WorldPacket packet(SMSG_PONG, 4);
	packet << ping;
	SendPacket(packet, true);*/

	sLog.outString("pong from server [%u]", ping);

	return true;
}

bool LogicSocket::Regist2ServerRet(WorldPacket &recvPacket)
{
	// NOTE: ATM the socket is singlethread, have this in mind ...
	uint32 node_type, node_index;
	uint8 ret;

	// Read the content of the packet
	recvPacket >> node_type;
	recvPacket >> node_index;
	recvPacket >> ret;

	DEBUG_LOG("LogicSocket::Regist2ServerRet: server node type %u, node index %u, result %u",
		node_type,
		node_index,
		ret);

	if (node_type != SERVER_NODE_TYPE_GAMELOGIC || ret != RESPONSE_SUCCESS)
	{
		return false;
	}
	if (!(m_session = new LogicSession(this, ServerNodeType(node_type), node_index)))
		return false;

	sSessionMgr.SetLogicSession(m_session);

	return true;
}
