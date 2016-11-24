#include "os_net_function.h"
#include <assert.h>
#include "../log/logapi.h"

#ifdef	_WIN32
#include <errno.h>
#include <WS2tcpip.h>
#else
#include <stdio.h>        
#include <sys/types.h>  
#include <ifaddrs.h>  
#include <netinet/in.h>   
#include <string.h>   
#include <arpa/inet.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <string.h>  
#include <stdlib.h>  
#include <stdlib.h>  
#include <time.h>  
#include <sys/vfs.h>  
#include <stdio.h>  
#include <signal.h>  
#include <sys/stat.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>
#endif

namespace jxsnet
{
namespace netapi
{
	static int started_net_num = 0;
	int InitNet()
	{
		if (0 == started_net_num) 
		{
#ifdef _WIN32
			WORD version_wanted = MAKEWORD(2,2);
			WSADATA wsaData;
			if ( ::WSAStartup(version_wanted, &wsaData) != 0 ) 
			{
				return(-1);
			}
#else
			void (*handler)(int);
			handler = signal(SIGPIPE, SIG_IGN);
			if ( handler != SIG_DFL ) 
			{
				signal(SIGPIPE, handler);
			}
#endif
		}
		++started_net_num;
		return(0);
	}

	void UnInitNet()
	{
		if (0 == started_net_num) 
		{
			return;
		}

		if ( --started_net_num == 0 ) 
		{
#ifdef _WIN32
			if ( ::WSACleanup() == SOCKET_ERROR ) 
			{
				if ( ::WSAGetLastError() == WSAEINPROGRESS ) 
				{
					::WSACleanup();
				}
			}
#else
			//恢复SIGPIPE信号
			void (*handler)(int);
			handler = signal(SIGPIPE, SIG_DFL);
			if ( handler != SIG_IGN ) 
			{
				signal(SIGPIPE, handler);
			}
#endif
		}
	}

	int GetNetLastError()
	{
#ifdef _WIN32
		return WSAGetLastError();
#else
		return errno;
#endif
	}

	void SetNetLastError(int err)
	{
#ifdef _WIN32
		WSASetLastError(err);
#else
		errno = err;
#endif
	}

	bool IsNetErrorOccured()
	{
		bool is_true = true;

		int error_code = GetNetLastError();
		switch(error_code)
		{
		case EINTR:
		case EAGAIN:
#ifdef _WIN32
		case ERROR_IO_PENDING:
		case WSA_WAIT_TIMEOUT:
#endif
			{
				is_true = false;
				break;
			}
		default:
			break;
		}

		if (is_true)
		{
			printf("IsNetErrorOccured::last_net_error=%d \n", error_code);
		}
		return is_true;
	}

	bool SetSocketUnBlock( SocketHandle socket_handle )
	{
#ifdef _WIN32
		u_long mode = 1;						//non-blocking mode is enabled.
		ioctlsocket(socket_handle, FIONBIO, &mode); //设置为非阻塞模式
#else
		int opts = fcntl(socket_handle, F_GETFL);
		if(opts < 0)
		{
			return false;
		}
		opts = opts | O_NONBLOCK;
		if(fcntl(socket_handle, F_SETFL, opts) < 0)
		{
			return false;
		}
#endif
		return true;
	}

	bool SetSocketReuse(SocketHandle socket_handle)
	{
		int reuse_opt = 1; 
		if (setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse_opt, sizeof(reuse_opt)) < 0)
		{
			return false;
		}
		return true;
	}

	bool SetSocketKeepAlive(SocketHandle socket_handle)
	{
		int reuse_opt = 1;
		if (setsockopt(socket_handle, SOL_SOCKET, SO_KEEPALIVE, (char *)&reuse_opt, sizeof(reuse_opt)) < 0)
		{
			return false;
		}
		return true;
	}

	bool SetSocketCloseOnExeC(SocketHandle socket_handle)
	{
#if !defined(_WIN32)
		int flags;
		if ((flags = fcntl(socket_handle, F_GETFD, NULL)) < 0) 
		{
			printf("fcntl(%d, F_GETFD)", socket_handle);
			return false;
		}
		if (fcntl(socket_handle, F_SETFD, flags | FD_CLOEXEC) == -1) 
		{
			printf("fcntl(%d, F_SETFD)", socket_handle);
			return false;
		}
#endif
		return true;
	}

	void CloseSocket( SocketHandle socket_handle )
	{
#ifdef _WIN32
		closesocket(socket_handle);
#else
		close(socket_handle);
#endif
	}

	bool ConnectBySocket(SocketHandle socket_handle, const char* ip, Int32 port, Int64 time_out)
	{
		struct sockaddr_in sockaddr;
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr;
		inet_pton(AF_INET, ip, (void *)&(sockaddr.sin_addr));
		sockaddr.sin_port = htons(port);

		int error = 0;
		SocketLen len;
		struct timeval tval;

		//成功则返回0，失败返回-1，错误原因存于errno中。
		int n  = connect(socket_handle, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
		if (0 == n)
		{
			return true;//瞬间成功
		}
		else
		{
			Int32 err = GetNetLastError();

#ifdef _WIN32
			if (err != EINPROGRESS && err != WSAEWOULDBLOCK)
			{
				return false;//出错
			}
#else 
			if (err != EINPROGRESS)
			{
				return false;//出错
			}
#endif
		}

		//以下处理非阻塞连接等待问题(errno = EINPROGRESS)
		fd_set rset,wset;
		FD_ZERO(&rset);
		FD_SET(socket_handle, &rset);

		wset = rset;
		tval.tv_sec = 0;
		tval.tv_usec = time_out;

		if ((n = select(socket_handle+1, &rset, &wset, NULL, &tval)) == 0)
		{
			SYS_LOG("建立连接超时")
			Int32 err = GetNetLastError();
			CloseSocket(socket_handle);		
			return false;
		}

		if (FD_ISSET(socket_handle, &rset) || FD_ISSET(socket_handle, &wset))
		{
			len = sizeof(error);
			if (getsockopt(socket_handle, SOL_SOCKET, SO_ERROR, (char*)&error, &len) < 0)
			{
				return false;
			}			
		}
		else
		{
			return false;
		}
		return true;
	}

	bool SendBySocket( SocketHandle socket_handle, const char* data, Int32 data_len, Int32& sended_len )
	{
		int sended_data_len = 0;
		int left_data_len = data_len;

		SetNetLastError(0);//clear the error
		do 
		{
			data_len = send(socket_handle, data, left_data_len, 0);
			if ( data_len >= 0 ) 
			{
				sended_data_len += data_len;
				left_data_len -= data_len;
				data += data_len;
			}
			else
			{
				printf("SendBySocket error :handle:%d, last_net_error:%d \n", socket_handle, GetNetLastError());
				return false;
			}

		} while ( (left_data_len > 0) && ((data_len > 0) || GetNetLastError() == EINTR ) );

		sended_len = sended_data_len;
		return true;
	}

	bool RecvBySocket( SocketHandle sochet_handle, void* data_buff, Int32 buff_size, Int32& recv_len )
	{
		int len = 0;
		recv_len = 0;
		int last_error = 0;

		SetNetLastError(0);
		while (true)
		{
			len = recv(sochet_handle, (char *)data_buff, buff_size, 0);

			if (len > 0)
			{
				break;
			}
			else if (len == 0)
			{
				break;
			}
			else
			{
				last_error = GetNetLastError();
				if (last_error == EINTR || last_error == EAGAIN)
				{
					DEBUG_LOG_D("len:%d last_error:%d", len, last_error);
					continue;
				}
				else
				{
					break;
				}
			}
		}

		recv_len = len;
		if (recv_len > 0)
		{
			return true;
		}
		else
		{
			printf("RecvBySocket fd:%d error: last_net_error:%d  \n", sochet_handle, last_error);
			return false;
		}
	}

	bool GetLocalIPBySocket(SocketHandle socket_handle, char* out_ip, Int32& out_port)
	{
		struct sockaddr_in local_sock;
		SocketLen size = sizeof(local_sock);
		memset(&local_sock, 0, sizeof(local_sock)); 

		if (getsockname(socket_handle, (struct sockaddr *)&local_sock, &size) == -1)
		{
			int error = GetNetLastError();
			return false;
		}

		char* ip = inet_ntoa(local_sock.sin_addr);
		strcpy(out_ip, ip);
		out_port = ntohs(local_sock.sin_port);

		return true;
	}

	bool GetRemoteIPBySocket(SocketHandle socket_handle, char* out_ip, Int32& out_port)
	{
		struct sockaddr_in local_sock;
		SocketLen size = sizeof(local_sock);
		memset(&local_sock, 0, sizeof(local_sock));

		if (getpeername(socket_handle, (struct sockaddr *)&local_sock, &size) == -1)
		{
			int error = GetNetLastError();
			return false;
		}

		char* ip = inet_ntoa(local_sock.sin_addr);
		strcpy(out_ip, ip);
		out_port = ntohs(local_sock.sin_port);

		return true;
	}

	bool GetLocalIp(char* out_ip)
	{ 
		
#ifndef _WIN32
		struct ifaddrs * ifAddrStruct = NULL;
		void * tmpAddrPtr = NULL;
		getifaddrs(&ifAddrStruct);
		while (ifAddrStruct != NULL)
		{
			if (ifAddrStruct->ifa_addr->sa_family == AF_INET)
			{   // check it is IP4  
				// is a valid IP4 Address  
				tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
				printf("%s IPV4 Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
				if (0 == strcmp("eth0", ifAddrStruct->ifa_name))
				{
					strcpy(out_ip, addressBuffer);
					break;
				}
			}
			else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6)
			{   // check it is IP6  
				// is a valid IP6 Address  
				tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
				char addressBuffer[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
				printf("%s IPV6 Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
			}
			ifAddrStruct = ifAddrStruct->ifa_next;
		}
#else
		InitNet();
		struct hostent *he;
		char hostname[128] ={ 0 };
		if (0 != gethostname(hostname, sizeof(hostname)))
		{
			printf("gethostname fail last_error:%d\n", netapi::GetNetLastError());
			return false;
		}
		he = gethostbyname(hostname);
		if (NULL == he)
		{
			printf("gethostbyname :%s \n", hostname);
			return false;
		}

		char* tmp_ip = inet_ntoa(*(struct in_addr*)(he->h_addr));
		strcpy(out_ip, tmp_ip);
		printf("get local ip:%s \n", tmp_ip);
		UnInitNet();
#endif	
		return true;
	}

}//end namespace netapi
}//end namespace jxsnet
