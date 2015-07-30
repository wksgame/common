#include"Socket.h"
#include <fcntl.h> 
#include<string.h>
#include<logger/logger.h>

#define  PORT 4000
#define  IP_ADDRESS "0.0.0.0"

namespace kiss
{

	bool InitNetwork()
	{
#ifdef WIN32
		WSADATA ws;

		//Init Windows Socket
		if ( WSAStartup(MAKEWORD(2,2), &ws) != 0 )
		{
			LOG_INFO("Init Windows Socket Failed"+GetLastError())
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
		if ( sock == INVALID_SOCKET )
		{
			LOG_INFO("Create Socket Failed")
			return INVALID_SOCKET;
		}
 
		return sock;
	}

	TCPSocket::~TCPSocket()
	{
		CloseSocket();
	}

	sock_t TCPSocket::Socket()
	{
		return sock;
	}

	sock_t TCPSocket::CreateSocket(const char* ip, const unsigned short port)
	{
		sock = kiss::CreateSocket();

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(ip);
		address.sin_port = htons(port);
		memset(address.sin_zero, 0x00, 8);

		return sock;
	}

	void TCPSocket::CloseSocket()
	{
		::closesocket(sock);
	}

	TCPServerSocket::TCPServerSocket()
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&rd);
		block = true;
	}

	bool TCPServerSocket::Bind()
	{
		if(::bind(sock, (struct sockaddr*)&address, sizeof(address)) != 0 )
		{
			LOG_INFO("Bind Socket Failed")
			return false;
		}

		return true;
	}

	bool TCPServerSocket::Listen(int count)
	{
		if(::listen(sock, count) != 0 )
		{
			LOG_INFO("Listen Socket Failed")
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

		if(s == INVALID_SOCKET )
		{
			LOG_INFO("Accept Socket Failed")
			return false;
		}

		return true;
	}

	bool TCPServerSocket::SetNoBlock()
	{
#ifdef WIN32
		unsigned long ul = 1;
		if (ioctlsocket(sock, FIONBIO, &ul) == SOCKET_ERROR)
			return false;
#else
#endif
		return true;
	}

	bool TCPServerSocket::SetBlockTimeOut(const int sec, const int usec)
	{
		struct timeval tv_out;
		tv_out.tv_sec = sec;
		tv_out.tv_usec = usec;
		block = false;
		FD_SET(sock, &rd);

		return SetNoBlock();
		//auto ret = ::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
		//return ret == 0;
	}

	bool TCPClientSocket::Connect()
	{
		if(connect(sock,(struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
		{
			LOG_INFO("Connect Socket Failed")
			return false;
		}

		return true;
	}
}//namespace kiss