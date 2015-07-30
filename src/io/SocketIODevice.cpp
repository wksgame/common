#include"SocketIODevice.h"
#include"SocketInputStream.h"
#include"SocketOutputStream.h"
#include<platform/platform.h>

namespace kiss
{
	SocketIODevice::SocketIODevice(const int sock):sock(sock)
	{
		is = new SocketInputStream(sock);
		os = new SocketOutputStream(sock);
	}

	SocketIODevice::~SocketIODevice()
	{
		delete is;
		delete os;
	}

	void SocketIODevice::close()
	{
		::closesocket(sock);
		sock = 0;

		delete is;
		delete os;

		is = nullptr;
		os = nullptr;
	}
}