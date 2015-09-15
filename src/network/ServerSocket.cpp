#include"ServerSocket.h"

namespace kiss
{
	bool ServerSocket::Bind()
	{
		if(::bind(sockfd, (struct sockaddr*)&address, sizeof(address)) != 0 )
		{
			return false;
		}

		return true;
	}

	bool ServerSocket::Listen(const int count)
	{
		if(::listen(sockfd, count) != 0 )
		{
			return false;
		}

		return true;
	}

	bool ServerSocket::Accept(int& s)
	{
		sockaddr_in sockaddr;
		return Accept(sockaddr, s);
	}

	bool ServerSocket::Accept(sockaddr_in& sockaddr, int& s)
	{
		unsigned int addressLength = sizeof(sockaddr);
		s = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addressLength);

		if(s == SOCKET_ERROR )
		{
			return false;
		}

		return true;
	}

	bool ServerSocket::Init(const char* ip, const int port, const int count)
	{
		if(!CreateSocket(ip, port))
			return false;

		if(!Bind())
			return false;

		if(!Listen(count))
			return false;

		return true;
	}

}//namespace kiss