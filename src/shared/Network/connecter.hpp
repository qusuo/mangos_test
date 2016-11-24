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

#ifndef __CONNECTER_HPP_
#define __CONNECTER_HPP_

#include <memory>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

#include "NetworkThread.hpp"
#include <string>

namespace MaNGOS
{
    template <typename SocketType>
    class Connecter
    {
        private:
            std::unique_ptr<NetworkThread<SocketType>> m_workerThreads;
			//boost::asio::io_service::work m_work;
			
			bool m_bConnect;
            // the time in milliseconds to sleep a worker thread at the end of each tick
            const int SleepInterval = 100;
			std::map<std::string, uint32> m_mapAddr;

			void BeginConnect(const std::string &sAddr, int nPort);
			void Reconnect(const std::string &sAdd);
			void OnConnect(NetworkThread<SocketType> *worker, SocketType *socket, const boost::system::error_code &ec);

        public:
			Connecter(std::map<std::string, uint32> &add_conn);
			~Connecter();

			bool IsConnected() { return m_bConnect; }
    };

    template <typename SocketType>
	Connecter<SocketType>::Connecter(std::map<std::string, uint32> &add_conn)
		: m_mapAddr(add_conn)
		, m_bConnect(false)
        //: m_acceptor(m_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
		m_workerThreads = std::unique_ptr<NetworkThread<SocketType>>(new NetworkThread<SocketType>([this](const std::string &sAdd) { this->Reconnect(sAdd); }));

		std::map<std::string, uint32>::const_iterator iter = m_mapAddr.begin();
		for (; iter != m_mapAddr.end(); ++iter)
		{
			BeginConnect(iter->first, iter->second);
		}
    }

    // FIXME - is this needed?
    template <typename SocketType>
    Connecter<SocketType>::~Connecter()
    {
        //m_service.stop();
        //m_acceptor.close();
		//m_workerThreads.join();
    }

    template <typename SocketType>
	void Connecter<SocketType>::BeginConnect(const std::string &sAddr,	int nPort)
	{
		NetworkThread<SocketType> *worker = m_workerThreads.get();
        SocketType *socket = worker->CreateSocket();
		socket->SetRemoteAddress(sAddr);
		socket->GetAsioSocket().async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(sAddr), nPort), [this, worker, socket](const boost::system::error_code &ec) { this->OnConnect(worker, socket, ec); });
    } 

	template <typename SocketType>
	void Connecter<SocketType>::Reconnect(const std::string &sAdd)
	{
		std::map<std::string, uint32>::iterator itr = m_mapAddr.find(sAdd);

		if (itr != m_mapAddr.end() && itr->second)
		{
			BeginConnect(itr->first, itr->second);
		}
	}

    template <typename SocketType>
	void Connecter<SocketType>::OnConnect(NetworkThread<SocketType> *worker, SocketType *socket, const boost::system::error_code &ec)
    {
		if (ec)
		{
			m_bConnect = false;
			// an error has occurred
			std::string sErr = ec.message();
			socket->Close();
			sLog.outString("Connect logic server error:%s", sErr.c_str());
			worker->RemoveSocket(socket);
		}
		else
		{
			m_bConnect = true;
			socket->Open();
			sLog.outString("Connect logic server success!");
		}
    }
}

#endif /* !__CONNECTER_HPP_ */
