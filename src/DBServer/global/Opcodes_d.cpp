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

/** \file
    \ingroup u2w
*/

#include "Opcodes_d.h"
#include "Policies/Singleton.h"

INSTANTIATE_SINGLETON_1(Opcodes_g);

OpcodeHandler_g const Opcodes_g::emptyHandler =
{
    "<none>",
    STATUS_UNHANDLED,
    PROCESS_INPLACE,
	&DBServerSession::Handle_NULL
};


Opcodes_g::Opcodes_g()
{
    /// Build Opcodes map
    BuildOpcodeList();
}

Opcodes_g::~Opcodes_g()
{
    /// Clear Opcodes
    mOpcodeMap.clear();
}


void Opcodes_g::BuildOpcodeList()
{
    /// Correspondence between opcodes and their names
	/*0x000*/  StoreOpcode(MSG_NULL_ACTION, "MSG_NULL_ACTION", STATUS_NEVER, PROCESS_INPLACE, &DBServerSession::Handle_NULL);
	/*1000*/  StoreOpcode(SMSG_REGISTER, "SMSG_REGISTER", STATUS_NEVER, PROCESS_INPLACE, &DBServerSession::Handle_NULL);
}
