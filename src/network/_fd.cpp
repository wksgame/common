#include"_fd.h"

#ifdef WIN32
#include"_fd_windows.h"
#else
#include"_fd_unix.h"
#endif

int newFD()
{
	auto sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return sock;
}

netFD::netFD()
{
	sysfd = newFD();
}

bool netFD::ok()
{
	return sysfd == -1;
}
