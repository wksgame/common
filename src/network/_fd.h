#ifndef KISS_FD_H
#define KISS_FD_H

struct sockAddr
{
	char* ip;
	unsigned short port;
};

struct netFD
{
	int sysfd;
	sockAddr addr;

	netFD();
	bool Init();
	bool ok();
	bool SetNoBlock();
	bool SetAddress(const sockAddr& addr);

	bool Connect();
};

#endif//KISS_FD_H