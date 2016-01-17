#include"Socket.h"
#include <fcntl.h> 

#include<logger/logger.h>

namespace kiss
{
	static bool __networkinit = true;

#ifdef WIN32

	struct InitNetwork
	{
		InitNetwork()
		{
			WSADATA ws;
			__networkinit = (WSAStartup(MAKEWORD(2,2), &ws)==0);		// 返回0表示OK
		}

		~InitNetwork()
		{
			WSACleanup();
		}
	};

	static InitNetwork init_network;		// 初始化

#endif//WIN32

	sock_t CreateSocket()
	{
		auto sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if ( sock == SOCKET_ERROR )
		{
			syslogger.error("Create Socket Failed");
			return SOCKET_ERROR;
		}
 
		return sock;
	}

	bool SetSocketNoBlock(const int sock)
	{
#ifdef WIN32
		unsigned long ul = 1;
		if (ioctlsocket(sock, FIONBIO, &ul) == SOCKET_ERROR)
			return false;
#else
		int opts = fcntl(sock,F_GETFL);
		opts|=O_NONBLOCK;
		if(fcntl(sock,F_SETFL,O_NONBLOCK)<0)
			return false;
#endif
		return true;
	}

}//namespace kiss