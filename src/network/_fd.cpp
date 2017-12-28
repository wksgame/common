#include"_fd.h"

#ifdef WIN32
#include"_fd_windows.h"
#else
#include"_fd_unix.h"
#endif

bool SetSocketNoBlock(const int sock)
	{
#ifdef WIN32
		unsigned long ul = 1;
		if (ioctlsocket(sock, FIONBIO, &ul) == SOCKET_ERROR)
			return false;
#else
		int opts = fcntl(sock,F_GETFL);
		opts|=O_NONBLOCK;
		if(fcntl(sock,F_SETFL,O_NONBLOCK)<0)
			return false;
#endif
		return true;
	}

netFD::netFD()
{
}

bool netFD::Init()
{
	sysfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return sysfd != -1;
}

bool netFD::ok()
{
	return sysfd == -1;
}

bool netFD::SetNoBlock()
{
	return SetSocketNoBlock(sysfd);
}

bool netFD::SetAddress(const sockAddr& addr)
{
	this->addr = addr;
}

bool netFD::Connect()
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(addr.ip);
	address.sin_port = htons(addr.port);
	//memset(address.sin_zero, 0x00, 8);

	int on=1;
	if(setsockopt(sysfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		return false;

	if(::connect(sysfd,(struct sockaddr*)&address, sizeof(address)) == -1)
		return false;

	return true;
}
