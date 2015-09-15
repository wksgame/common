#ifndef KISS_CLIENT_SOCKET_H
#define KISS_CLIENT_SOCKET_H

#include"TCPIOSocket.h"

namespace kiss
{
	class ClientSocket:public TCPIOSocket
	{
	public:
		ClientSocket(const uint32 buffSize);

		bool Init(const char* ip, const int port);
		bool Connect();
	};//class ClientSocket

}//namespace kiss

#endif//KISS_CLIENT_SOCKET_H