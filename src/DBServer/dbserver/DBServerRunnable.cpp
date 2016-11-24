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
    \ingroup mangosd
*/

#include "Common.h"
#include "DBServer.h"
#include "DBServerRunnable.h"
#include "Timer.h"
//#include "ObjectAccessor.h"
//#include "MapManager.h"

#include "Database/DatabaseEnv.h"
#include "include/value_def.h"

#ifdef WIN32
#include "ServiceWin32.h"
extern int m_ServiceStatus;
#endif

/// Heartbeat for the DBServer
void DBServerRunnable::run()
{
    ///- Init new SQL thread for the DBServer database
#ifdef ONLY_NETWORK
    WorldDatabase.ThreadStart();                            // let thread do safe mySQL requests (one connection call enough)
#endif

    sDBServer.InitResultQueue();

    uint32 realCurrTime = 0;
    uint32 realPrevTime = WorldTimer::tick();

    uint32 prevSleepTime = 0;                               // used for balanced full tick time length near WORLD_SLEEP_CONST

    ///- While we have not DBServer::m_stopEvent, update the DBServer
    while (!DBServer::IsStopped())
    {
        ++DBServer::m_gatewayLoopCounter;
        realCurrTime = WorldTimer::getMSTime();

        uint32 diff = WorldTimer::tick();

        sDBServer.Update(diff);
        realPrevTime = realCurrTime;

        // diff (D0) include time of previous sleep (d0) + tick time (t0)
        // we want that next d1 + t1 == WORLD_SLEEP_CONST
        // we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
        // d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
        if (diff <= WORLD_SLEEP_CONST + prevSleepTime)
        {
            prevSleepTime = WORLD_SLEEP_CONST + prevSleepTime - diff;
            MaNGOS::Thread::Sleep(prevSleepTime);
        }
        else
            prevSleepTime = 0;

#ifdef WIN32
        if (m_ServiceStatus == 0) DBServer::StopNow(SHUTDOWN_EXIT_CODE);
        while (m_ServiceStatus == 2) Sleep(1000);
#endif
    }

    sDBServer.CleanupsBeforeStop();

    //sMapMgr.UnloadAll();                                    // unload all grids (including locked in memory)

    ///- End the database thread
    WorldDatabase.ThreadEnd();                              // free mySQL thread resources
}
