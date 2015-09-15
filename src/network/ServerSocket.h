#ifndef KISS_SERVER_SOCKET_H
#define KISS_SERVER_SOCKET_H

#include"TCPSocket.h"

namespace kiss
{
	class ServerSocket:public TCPSocket
	{
		bool Bind();
		bool Listen(const int count);

	public:
		/**
		 * @param ip 监听的IP
		 * @param port 监听的端口
		 * @param count 监听时的最大等待数量
		 */
		bool Init(const char* ip, const int port, const int count=10);

		bool Accept(sock_t& s);
		bool Accept(sockaddr_in& sockaddr, sock_t& s);

	};//class ServerSocket

}//namespace kiss
#endif//KISS_SERVER_SOCKET_H