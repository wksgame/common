#include"Socket.h"
#include <fcntl.h> 

#ifdef KISS_DEBUG
#include<iostream>
using namespace std;
#endif//DEBUG

#ifdef KISS_WINDOW
#pragma comment(lib,"ws2_32.lib")
#endif//KISS_WINDOW

#define  PORT 4000
#define  IP_ADDRESS "0.0.0.0"

namespace kiss
{

	bool InitNetwork()
	{
#ifdef KISS_WINDOW
		WSADATA ws;

		//Init Windows Socket
		if ( WSAStartup(MAKEWORD(2,2), &ws) != 0 )
		{
#ifdef KISS_DEBUG
			cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
#endif//KISS_DEBUG
			return false;
		}
#else

#endif//KISS_WINDOW

		return true;
	}

	void CloseNetwork()
	{
#ifdef KISS_WINDOW
		WSACleanup();
#endif//KISS_WINDOW
	}

	sock_t CreateSocket()
	{
		auto sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if ( sock == INVALID_SOCKET )
		{
#ifdef KISS_DEBUG
			cout<<"Create Socket Failed::"<<GetLastError()<<endl;
#endif//KISS_DEBUG
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
#ifdef KISS_WINDOW
		::closesocket(sock);
#else
		::close(sock);
#endif//KISS_WINDOW
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
#ifdef KISS_DEBUG
			cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
#endif//KISS_DEBUG
			return false;
		}

		return true;
	}

	bool TCPServerSocket::Listen(int count)
	{
		if(::listen(sock, count) != 0 )
		{
#ifdef KISS_DEBUG
			cout<<"Listen Socket Failed::"<<GetLastError()<<endl;
#endif//KISS_DEBUG
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
		int addressLength = sizeof(sockaddr);
		s = accept(sock, (struct sockaddr*)&sockaddr, &addressLength);

		if(s == INVALID_SOCKET )
		{
#ifdef KISS_DEBUG
			cout<<"Accept Socket Failed::"<<GetLastError()<<endl;
#endif//KISS_DEBUG
			return false;
		}

		return true;
	}

	bool TCPServerSocket::SetNoBlock()
	{
		unsigned long ul = 1;
		if (ioctlsocket(sock, FIONBIO, &ul) == SOCKET_ERROR)
			return false;
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
#ifdef KISS_DEBUG
			cout<<"Connect Socket Failed::"<<GetLastError()<<endl;
#endif//KISS_DEBUG
			return false;
		}

		return true;
	}
}//namespace kiss