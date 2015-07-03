#ifndef KISS_SOCKET_H
#define KISS_SOCKET_H

#ifndef KISS_WINDOW
#define KISS_WINDOW
#endif //KISS_WINDOW

#ifdef KISS_WINDOW
//#include<WinSock2.h>
#include<Windows.h>
#else
#endif//KISS_WINDOW

#ifdef KISS_WINDOW
#	define sock_t	SOCKET
#else
#	define sock_t	int
#endif//KISS_WINDOW

namespace kiss
{
	bool InitNetwork();
	void CloseNetwork();

	class TCPSocket
	{
	protected:

		sockaddr_in address;
		sock_t sock;

	public:
		~TCPSocket();

		sock_t Socket();

		sock_t CreateSocket(const char* ip, const unsigned short port);
		void CloseSocket();
	};//class Socket

	class TCPServerSocket:public TCPSocket
	{
		timeval timeout;
		fd_set rd;
		bool block;
	public:
		TCPServerSocket();

		bool Bind();
		bool Listen(int count=10);
		bool Accept(sock_t& s);
		bool Accept(sockaddr_in& sockaddr, sock_t& s);
		bool SetNoBlock();
		bool SetBlockTimeOut(const int sec=5, const int usec=0);
	};//class TCPServerSocket

	class TCPClientSocket:public TCPSocket
	{
	public:

		bool Connect();
	};//class TCPClientSocket
}
#endif//KISS_SOCKET_H