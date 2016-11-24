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

#ifndef _SSOPCODES_H
#define _SSOPCODES_H

#include "Common.h"
#include "protocol_def.h"

// Note: this include need for be sure have full definition of class WorldSession
//       if this class definition not complite then VS for x64 release use different size for
//       struct OpcodeHandler in this header and Opcode.cpp and get totally wrong data from
//       table opcodeTable in source when Opcode.h included but WorldSession.h not included
//#include "WorldSession.h"

//enum ServerMessageType{

//
//
//};

/// List of Opcodes
enum OpcodesList_s
{
	MSG_NULL_ACTION = 0,

	//һ������������һ��������ע��
	SMSG_REGISTER = SERVER_SERVER_MSG_BEGIN,
	SMSG_REGISTER_RET,

	SMSG_REQ_DISCONNECT_USER_SG,					//�����������ط���Ͽ������������
	SMSG_TELL_DISCONNECT_USER_GS,					//����֪ͨ��������������ѶϿ�
	SMSG_REQ_SET_TIME_GAP_SM,						//��������GM������ı������ʱ������
	SMSG_TELL_SET_TIME_GAP_MS,						//GM֪ͨ�������ı������ʱ��
	SMSG_REQ_SET_ANTI_ADDICTION_SM,					//��������GM������ı��������������
	SMSG_TELL_SET_ANTI_ADDICTION_SM,				//GM��֪ͨ�������ı����������

	CMSG_PING,
	SMSG_PONG,
	SMSG_SERVER_MESSAGE,

	SNUM_SMSG_TYPES,
};

// Don't forget to change this value and add opcode name to Opcodes.cpp when you add new opcode!
//#define NUM_MSG_TYPES 0x424

/// Player state
enum SessionStatus_s
{
	STATUS_AUTHED = 0,                                      ///< Player authenticated (_player==nullptr, m_playerRecentlyLogout = false or will be reset before handler call)
	STATUS_LOGGEDIN,                                        ///< Player in game (_player!=nullptr, inWorld())
	STATUS_TRANSFER,                                        ///< Player transferring to another map (_player!=nullptr, !inWorld())
	STATUS_LOGGEDIN_OR_RECENTLY_LOGGEDOUT,                  ///< _player!= nullptr or _player==nullptr && m_playerRecentlyLogout)
	STATUS_NEVER,                                           ///< Opcode not accepted from client (deprecated or server side only)
	STATUS_UNHANDLED                                        ///< We don' handle this opcode yet
};

enum PacketProcessing_s
{
	PROCESS_INPLACE = 0,                                    // process packet whenever we receive it - mostly for non-handled or non-implemented packets
	PROCESS_THREADUNSAFE,                                   // packet is not thread-safe - process it in World::UpdateSessions()
	PROCESS_THREADSAFE                                      // packet is thread-safe - process it in Map::Update()
};



#endif
/// @}
