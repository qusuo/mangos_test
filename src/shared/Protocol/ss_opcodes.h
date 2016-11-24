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

	//一个服务器向另一个服务器注册
	SMSG_REGISTER = SERVER_SERVER_MSG_BEGIN,
	SMSG_REGISTER_RET,

	SMSG_REQ_DISCONNECT_USER_SG,					//其他服向网关发起断开玩家连接请求
	SMSG_TELL_DISCONNECT_USER_GS,					//网关通知其他服玩家连接已断开
	SMSG_REQ_SET_TIME_GAP_SM,						//其他服向GM服发起改变服务器时间请求
	SMSG_TELL_SET_TIME_GAP_MS,						//GM通知其他服改变服务器时间
	SMSG_REQ_SET_ANTI_ADDICTION_SM,					//其他服向GM服发起改变防沉迷设置请求
	SMSG_TELL_SET_ANTI_ADDICTION_SM,				//GM服通知其他服改变防沉迷设置

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
