#ifndef KISS_SOCKET_H
#define KISS_SOCKET_H

#include<platform/platform.h>
#include<defines.h>

namespace kiss
{
	sock_t CreateSocket();
	bool SetSocketNoBlock(const int sock);
}
#endif//KISS_SOCKET_H