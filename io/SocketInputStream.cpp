#include"SocketInputStream.h"
#include<string>
#include<winsock.h>

namespace kiss
{
	SocketInputStream::SocketInputStream(const int sock):sock(sock)
	{
	}

	SocketInputStream::~SocketInputStream()
	{
	}

	bool SocketInputStream::read(void* data, const buff_t size)
	{
		buff_t totalRecv=0;
		while(totalRecv<size)
		{
			auto Ret = ::recv(sock, (char*)data+totalRecv, size-totalRecv, 0);

			if ( Ret == 0 || Ret == SOCKET_ERROR ) 
				return false;

			totalRecv += Ret;
		}

		return true;
	}

	bool SocketInputStream::peek(void* data, const buff_t size)
	{
		return false;
	}

	bool SocketInputStream::skip(const buff_t size)
	{
		return false;
	}

	void SocketInputStream::close()
	{
		if(sock)
			::closesocket(sock);
		sock=0;
	}
}