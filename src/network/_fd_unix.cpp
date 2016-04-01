//#ifdef UNIX
#include"_fd_unix.h"
#include <fcntl.h> 

bool SetSocketNoBlock(const int sock)
{
	int opts = fcntl(sock,F_GETFL);
	opts|=O_NONBLOCK;
	if(fcntl(sock,F_SETFL,O_NONBLOCK)<0)
		return false;

	return true;
}
//#endif//UNIX