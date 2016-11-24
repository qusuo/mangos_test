#ifndef JXS_HEADERDEF_H_
#define JXS_HEADERDEF_H_

#include<cstdio>
#include<cassert>
#include<cstdlib>
#include<iostream>
#include<sstream>
#include<queue>
#include<list>
#include<map>
#include<set>
#include<vector>
#include<fcntl.h>
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <algorithm>  
#include <functional>

#ifdef _WIN32
#include<WinSock2.h>
#include<Mswsock.h>
#include<Windows.h>
#include<process.h>
#include<io.h>
#include<direct.h>
#include<time.h>
#include<Ws2tcpip.h>

#else
#include<netinet/tcp.h>
#include<signal.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>     
#include<sys/epoll.h>     
#include<netinet/in.h>     
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<errno.h>
#include<sys/time.h>

#endif

#include "compile_def.h"
#include "platform_macros.h"



#endif