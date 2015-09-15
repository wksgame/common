#ifndef SGL_ACCEPT_SOCKET_H
#define SGL_ACCEPT_SOCKET_H

#include"TCPIOSocket.h"

typedef unsigned long long int uint64;

namespace kiss
{
	class RingBuffer;

	/**
	 * @brief 服务器端接入的客户端连接
	 */
	class AcceptSocket:public TCPIOSocket
	{
	public:

		AcceptSocket(const int sock,const sockaddr_in& address,const int buffSize=64*1024);
	};
}//namespace sgl
#endif//SGL_ACCEPT_SOCKET_H