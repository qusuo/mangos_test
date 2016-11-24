//客户端与服务器之间的逻辑协议

#ifndef _CSOPCODES_H
#define _CSOPCODES_H

#include "Common.h"
#include "protocol_def.h"

/// List of Opcodes
enum OpcodesList_c
{
	CMSG_NULL_ACTION = 0,

	SMSG_ADDON_INFO = CLIENT_GAMELOGIC_MSG_BEGIN,

	CNUM_SMSG_TYPES,
};


#endif
/// @}
