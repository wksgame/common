#ifndef KISS_TCP_SOCKET_H
#define KISS_TCP_SOCKET_H

#include"Socket.h"

namespace kiss
{
	class TCPSocket
	{
	protected:

		sockaddr_in address;
		sock_t sockfd;
		timeval timeout;
		bool block;

	protected:

		bool CreateSocket(const char* ip, const unsigned short port);

	public:
		TCPSocket();
		~TCPSocket();

		inline sock_t GetSocketFD(){return sockfd;}

		bool SetNoBlock();
		bool SetBlockTimeOut(const int sec=5, const int usec=0);

	};//class Socket
}//namespace kiss
#endif//KISS_TCP_SOCKET_H