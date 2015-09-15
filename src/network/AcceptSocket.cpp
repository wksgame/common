#include"AcceptSocket.h"
#include<cstring>

namespace kiss
{
	AcceptSocket::AcceptSocket(const int sock,const sockaddr_in& address,const int buffSize): TCPIOSocket(buffSize)
	{
		this->sockfd = sock;
		memcpy(&(this->address),&address,sizeof(address));
	}

}//namespace sgl