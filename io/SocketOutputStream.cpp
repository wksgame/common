#include"SocketOutputStream.h"
#include<string>
#include<winsock.h>

namespace kiss
{
	SocketOutputStream::SocketOutputStream(const int sock):sock(sock)
	{
	}

	SocketOutputStream::~SocketOutputStream()
	{
	}

	bool SocketOutputStream::write(const void* data, const buff_t size)
	{
		buff_t totalSend=0;
		while(totalSend<size)
		{
			auto Ret = ::send(sock, (char*)data+totalSend, size-totalSend, 0);

			if ( Ret == 0 || Ret == SOCKET_ERROR ) 
				return false;

			totalSend += Ret;
		}

		return true;
	}

	buff_t SocketOutputStream::cur_pos()const
	{
		return 0;
	}

	void SocketOutputStream::close()
	{
		if(sock)
			::closesocket(sock);
		sock=0;
	}
}