#ifndef JXS_OSNETFUNCTION_H_
#define JXS_OSNETFUNCTION_H_
//author:hsj
#include "network_def.h"
#include "../util/id_manager.h"

namespace jxsnet
{
namespace netapi
{
int InitNet();
void UnInitNet();
int GetNetLastError();
void SetNetLastError(int err);
bool IsNetErrorOccured();

bool SetSocketUnBlock(SocketHandle socket_handle);
bool SetSocketReuse(SocketHandle socket_handle);
bool SetSocketKeepAlive(SocketHandle socket_handle);
//非win32中设置程序退出自动关闭socket
bool SetSocketCloseOnExeC(SocketHandle socket_handle); 

void CloseSocket(SocketHandle socket_handle);
bool ConnectBySocket(SocketHandle socket_handle, const char* ip, Int32 port, Int64 time_out);
bool SendBySocket(SocketHandle socket_handle, const char* data, Int32 data_len, Int32& sended_len);
bool RecvBySocket(SocketHandle sochet_handle, void* data_buff, Int32 buff_size, Int32& recv_len);
bool GetLocalIPBySocket(SocketHandle socket_handle, char* out_ip, Int32& out_port);
bool GetRemoteIPBySocket(SocketHandle socket_handle, char* out_ip, Int32& out_port);
bool GetLocalIp(char* out_ip);

}//end namespace netapi
}//end namespace jxsnet

#endif