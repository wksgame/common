#ifndef KISS_PLATFORM_H
#define KISS_PLATFORM_H

#ifdef WIN32
#	include<winsock.h>
#else
#	include<unistd.h>
#	include<sys/socket.h>
#	include<sys/select.h>
#	include<netinet/in.h>		//sockaddr_in
#	include<arpa/inet.h>		//ip地址转换
#	include<errno.h>
#endif

#ifdef WIN32
#	define sock_t	SOCKET
#	define socklen_t int				//linux
#else
#	define INVALID_SOCKET 0				//WIN32
#	define SOCKET_ERROR -1				//WIN32
#	define sock_t	int
#	define closesocket(s) close(s)		//WIN32
#endif//WIN32

#endif//KISS_PLATFORM_H