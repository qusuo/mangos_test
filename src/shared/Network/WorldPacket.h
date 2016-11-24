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

#ifndef MANGOSSERVER_WORLDPACKET_H
#define MANGOSSERVER_WORLDPACKET_H

#include "Common.h"
#include "ByteBuffer.h"
#include "Protocol\ss_opcodes.h"
#include "Config\DefaultDef.h"

 

// Note: m_opcode and size stored in platfom dependent format
// ignore endianess until send, and converted at receive
class WorldPacket : public ByteBuffer
{
public:
	// just container for later use
	WorldPacket() 
		: ByteBuffer(0)
		, m_opcode(MSG_NULL_ACTION)
		, m_opcode_type(SS_MSG_TYPE_SYS)
		, m_node_index(0)
		, m_node_type(0)
	{
	}
	
	explicit WorldPacket(uint16 opcode, size_t res = DEFAULT_PACK_BUF, uint32 opcode_type = SS_MSG_TYPE_SYS)
		: ByteBuffer(res)
		, m_opcode(opcode)
		, m_opcode_type(opcode_type)
		, m_node_index(0)
		, m_node_type(0)
	{ }

	// copy constructor
	WorldPacket(const WorldPacket& packet) 
		: ByteBuffer(packet)
		, m_opcode(packet.m_opcode)
		, m_opcode_type(packet.m_opcode_type)
		, m_node_index(packet.m_node_index)
		, m_node_type(packet.m_node_type)
	{
	}

	void Initialize(uint16 opcode, size_t newres = DEFAULT_PACK_BUF)
	{
		clear();
		_storage.reserve(newres);
		m_opcode = opcode;
		m_opcode_type = SS_MSG_TYPE_SYS;
		m_node_index = 0;	//dest 
		m_node_type = 0;
	}

	uint16	GetOpcode() const { return m_opcode; }
	void	SetOpcode(uint16 opcode) { m_opcode = opcode; }

	uint32	GetOpcodeType() const { return m_opcode_type; }
	void	SetOpcodeType(uint32 opcode_type) { m_opcode_type = opcode_type; }

	uint32	GetNodeIndex() const { return m_node_index; }
	void	SetNodeIndex(uint32 node_index) { m_node_index = node_index; }

	uint32	GetNodeType() const { return m_node_type; }
	void	SetNodeType(uint32 node_type) { m_node_type = node_type; }

	//inline const char* GetOpcodeName() const { return LookupOpcodeName(m_opcode); }
protected:
	uint16 m_opcode;
	uint32 m_opcode_type;
	uint32 m_node_index;
	uint32 m_node_type;
};

#endif
