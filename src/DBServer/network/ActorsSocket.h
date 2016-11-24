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

/** \addtogroup u2w User to Gateway Communication
 * @{
 * \file GatewaySocket.h
 * \author Derex <derex101@gmail.com>
 */

#ifndef _ACTORSOCKET_H
#define _ACTORSOCKET_H

#include "Common.h"
#include "Auth/AuthCrypt.h"
#include "Auth/BigNumber.h"
#include "Network/Socket.hpp"

#include <chrono>
#include <functional>

class GatewayPacket;
class GatewaySession;

/**
 * GatewaySocket.
 *
 * This class is responsible for the communication with
 * remote clients.
 * Most methods return -1 on failure.
 * The class uses reference counting.
 *
 * For output the class uses one buffer (64K usually) and
 * a queue where it stores packet if there is no place on
 * the queue. The reason this is done, is because the server
 * does really a lot of small-size writes to it, and it doesn't
 * scale well to allocate memory for every. When something is
 * written to the output buffer the socket is not immediately
 * activated for output (again for the same reason), there
 * is 10ms celling (thats why there is Update() override method).
 * This concept is similar to TCP_CORK, but TCP_CORK
 * uses 200ms celling. As result overhead generated by
 * sending packets from "producer" threads is minimal,
 * and doing a lot of writes with small size is tolerated.
 *
 * The calls to Update () method are managed by GatewaySocketMgr
 * and ReactorRunnable.
 *
 * For input ,the class uses one 1024 bytes buffer on stack
 * to which it does recv() calls. And then received data is
 * distributed where its needed. 1024 matches pretty well the
 * traffic generated by client for now.
 *
 * The input/output do speculative reads/writes (AKA it tryes
 * to read all data available in the kernel buffer or tryes to
 * write everything available in userspace buffer),
 * which is ok for using with Level and Edge Triggered IO
 * notification.
 *
 */

class GatewaySocket : public MaNGOS::Socket
{
    private:
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif
        struct ClientPktHeader
        {
            uint16 size;
            uint32 cmd;
        };
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

        /// Time in which the last ping was received
        std::chrono::system_clock::time_point m_lastPingTime;

        /// Keep track of over-speed pings ,to prevent ping flood.
        uint32 m_overSpeedPings;

        ClientPktHeader m_existingHeader;
        bool m_useExistingHeader;

        /// Class used for managing encryption of the headers
        AuthCrypt m_crypt;

        /// Session to which received packets are routed
        GatewaySession *m_session;
        bool m_sessionFinalized;

        const uint32 m_seed;

        BigNumber m_s;

        /// process one incoming packet.
        virtual bool ProcessIncomingData() override;

        /// Called by ProcessIncoming() on CMSG_AUTH_SESSION.
        bool HandleAuthSession(GatewayPacket &recvPacket);

        /// Called by ProcessIncoming() on CMSG_PING.
        bool HandlePing(GatewayPacket &recvPacket);

    public:
        GatewaySocket(boost::asio::io_service &service, std::function<void (Socket *)> closeHandler);
        virtual ~GatewaySocket();

        // send a packet \o/
        void SendPacket(const GatewayPacket& pct, bool immediate = false);

        void FinalizeSession() { m_sessionFinalized = true; }

        virtual bool Open() override;
        virtual bool Deletable() const override { return (m_sessionFinalized || IsClosed()) && Socket::Deletable(); }

        /// Return the session key
        BigNumber &GetSessionKey() { return m_s; }

};

#endif  /* _GatewaySOCKET_H */

/// @}
