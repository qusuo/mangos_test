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

#ifndef __BASESESSION_H
#define __BASESESSION_H

//#include "Common.h"
//#include "SharedDefines.h"
//#include "ObjectGuid.h"
//#include "AuctionHouseMgr.h"
//#include "Item.h"
//#include "WorldSocket.h"
//#include "Network\NetDef.h"
//#include "SessionFilter.h"
//
//#include <deque>
//#include <mutex>


class Player;



/// Player session in the World
class MANGOS_DLL_SPEC BaseSession
{
    public:
		BaseSession(){};
		~BaseSession(){};

		virtual Player* GetPlayer() const = 0;
};
#endif
/// @}
