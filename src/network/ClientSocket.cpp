#include"ClientSocket.h"

namespace kiss
{
	ClientSocket::ClientSocket(const uint32 buffSize): TCPIOSocket(buffSize)
	{

	}

	bool ClientSocket::Init(const char* ip, const int port)
	{
		return CreateSocket(ip, port);
	}

	bool ClientSocket::Connect()
	{
		if(::connect(sockfd,(struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
			return false;

		return true;
	}
}