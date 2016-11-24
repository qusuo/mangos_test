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

#ifndef _OPCODES_D_H
#define _OPCODES_D_H

#include "Common.h"

// Note: this include need for be sure have full definition of class WorldSession
//       if this class definition not complite then VS for x64 release use different size for
//       struct OpcodeHandler_s in this header and Opcode.cpp and get totally wrong data from
//       table opcodeTable in source when Opcode.h included but DBServerSession.h not included
#include "network/DBServerSession.h"
#include "Policies/Singleton.h"
#include "Protocol/ss_opcodes.h"


class WorldPacket;


struct OpcodeHandler_d
{
	char const* name;
	SessionStatus_s status;
	PacketProcessing_s packetProcessing;
	void (DBServerSession::*handler)(WorldPacket& recvPacket);
};

typedef std::map< uint16, OpcodeHandler_d> OpcodeMap_g;

class Opcodes_d
{
    public:
		Opcodes_d();
		~Opcodes_d();
    public:
        void BuildOpcodeList();
		void StoreOpcode(uint16 Opcode, char const* name, SessionStatus_s status, PacketProcessing_s process, void (DBServerSession::*handler)(WorldPacket& recvPacket))
        {
            OpcodeHandler_d& ref = mOpcodeMap[Opcode];
            ref.name = name;
            ref.status = status;
            ref.packetProcessing = process;
            ref.handler = handler;
        }

        /// Lookup opcode
        inline OpcodeHandler_d const* LookupOpcode(uint16 id) const
        {
            OpcodeMap_g::const_iterator itr = mOpcodeMap.find(id);
            if (itr != mOpcodeMap.end())
                return &itr->second;
            return nullptr;
        }

        /// compatible with other mangos branches access

        inline OpcodeHandler_d const& operator[](uint16 id) const
        {
            OpcodeMap_g::const_iterator itr = mOpcodeMap.find(id);
            if (itr != mOpcodeMap.end())
                return itr->second;
            return emptyHandler;
        }

        static OpcodeHandler_d const emptyHandler;

        OpcodeMap_g mOpcodeMap;
};

#define opcodeTable_d MaNGOS::Singleton<Opcodes_g>::Instance()

/// Lookup opcode name for human understandable logging
inline char const* LookupOpcodeName(uint16 id)
{
	if (OpcodeHandler_d const* op = opcodeTable_d.LookupOpcode(id))
        return op->name;
    return "Received unknown opcode, it's more than max!";
}


#endif
/// @}
