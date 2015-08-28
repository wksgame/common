#include"Socket.h"
#include <fcntl.h> 
#include<string.h>
#include<logger/logger.h>

namespace kiss
{

	bool InitNetwork()
	{
#ifdef WIN32
		WSADATA ws;

		//Init Windows Socket
		if ( WSAStartup(MAKEWORD(2,2), &ws) != 0 )
		{
			syslogger.error("Init Windows Socket Failed %s",GetLastError())
			return false;
		}
#else

#endif//WIN32

		return true;
	}

	void CloseNetwork()
	{
#ifdef WIN32
		WSACleanup();
#endif//WIN32
	}

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
	
	TCPSocket::TCPSocket()
	{
		memset(&address,0,sizeof(address));
		sock=0;
		timeout.tv_sec=0;
		timeout.tv_usec=0;
		block=true;
	}

	TCPSocket::~TCPSocket()
	{
		CloseSocket();
	}

	sock_t TCPSocket::Socket()
	{
		return sock;
	}

	bool TCPSocket::CreateSocket(const char* ip, const unsigned short port)
	{
		sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if ( sock == SOCKET_ERROR )
		{
			syslogger.error("Create Socket Failed");
			return false;
		}

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(ip);
		address.sin_port = htons(port);
		//memset(address.sin_zero, 0x00, 8);
		
		int on=1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); 

		return true;
	}
	
	bool TCPSocket::SetNoBlock()
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
	
	bool TCPSocket::SetBlockTimeOut(const int sec, const int usec)
	{
		timeout.tv_sec = sec;
		timeout.tv_usec = usec;

		auto ret = ::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
		ret = ::setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

		return ret == 0;
	}

	void TCPSocket::CloseSocket()
	{
		::closesocket(sock);
	}

	bool TCPServerSocket::Bind()
	{
		if(::bind(sock, (struct sockaddr*)&address, sizeof(address)) != 0 )
		{
			syslogger.error("Bind Socket Failed");
			return false;
		}

		return true;
	}

	bool TCPServerSocket::Listen(int count)
	{
		if(::listen(sock, count) != 0 )
		{
			syslogger.error("Listen Socket Failed");
			return false;
		}

		return true;
	}

	bool TCPServerSocket::Accept(sock_t& s)
	{
		sockaddr_in sockaddr;
		return Accept(sockaddr, s);
	}

	bool TCPServerSocket::Accept(sockaddr_in& sockaddr, sock_t& s)
	{
		unsigned int addressLength = sizeof(sockaddr);
		s = accept(sock, (struct sockaddr*)&sockaddr, (socklen_t*)&addressLength);

		if(s == SOCKET_ERROR )
		{
			syslogger.error("Accept Socket Failed");
			return false;
		}

		return true;
	}

	bool TCPClientSocket::Connect()
	{
		if(connect(sock,(struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
		{
			syslogger.error("Connect Socket Failed");
			return false;
		}

		return true;
	}
}//namespace kiss