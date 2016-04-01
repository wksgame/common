#ifdef WIN32
#include"_fd_windows.h"

static bool __networkinit = true;

struct InitNetwork
{
	InitNetwork()
	{
		WSADATA ws;
		__networkinit = (WSAStartup(MAKEWORD(2,2), &ws)==0);		// 返回0表示OK
	}

	~InitNetwork()
	{
		WSACleanup();
	}
};

static InitNetwork init_network;		// 初始化

bool SetSocketNoBlock(const int sock)
{
	unsigned long ul = 1;
	if (ioctlsocket(sock, FIONBIO, &ul) == SOCKET_ERROR)
		return false;
	else
		return true;
}
#endif//WIN32