#include"TCPSocket.h"
#include<string.h>

namespace kiss
{
	TCPSocket::TCPSocket()
	{
		memset(&address,0,sizeof(address));
		sockfd=0;
		timeout.tv_sec=0;
		timeout.tv_usec=0;
		block=true;
	}

	TCPSocket::~TCPSocket()
	{
		::closesocket(sockfd);
	}

	bool TCPSocket::CreateSocket(const char* ip, const unsigned short port)
	{
		sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (sockfd == SOCKET_ERROR)
			return false;

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(ip);
		address.sin_port = htons(port);
		//memset(address.sin_zero, 0x00, 8);

		int on=1;
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))==-1)
			return false;

		return true;
	}

	bool TCPSocket::SetNoBlock()
	{
		return SetSocketNoBlock(sockfd);
	}

	bool TCPSocket::SetBlockTimeOut(const int sec, const int usec)
	{
		timeout.tv_sec = sec;
		timeout.tv_usec = usec;

		if(::setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout))==-1)
			return false;

		if(::setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout))==-1)
			return false;

		return true;
	}
}//namespace kiss