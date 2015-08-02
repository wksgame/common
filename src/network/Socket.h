#ifndef KISS_SOCKET_H
#define KISS_SOCKET_H

#include<platform/platform.h>
#include<defines.h>

namespace kiss
{
	bool InitNetwork();
	void CloseNetwork();

	class TCPSocket
	{
	protected:

		sockaddr_in address;
		sock_t sock;
		timeval timeout;
		bool block;

	public:
		TCPSocket();
		~TCPSocket();

		sock_t Socket();

		sock_t CreateSocket(const char* ip, const unsigned short port);
		bool SetNoBlock();
		bool SetBlockTimeOut(const int sec=5, const int usec=0);

		void CloseSocket();
	};//class Socket

	class TCPServerSocket:public TCPSocket
	{
	public:

		bool Bind();
		bool Listen(int count=10);
		bool Accept(sock_t& s);
		bool Accept(sockaddr_in& sockaddr, sock_t& s);
	};//class TCPServerSocket

	class TCPClientSocket:public TCPSocket
	{
	public:

		bool Connect();
	};//class TCPClientSocket
}
#endif//KISS_SOCKET_H