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

	bool SocketIODevice::read(void* data, const buff_t size)
	{
		return is->read(data,size);
	}

	bool SocketIODevice::peek(void* data, const buff_t size)
	{
		return is->peek(data,size);
	}

	bool SocketIODevice::skip(const buff_t size)
	{
		return is->skip(size);
	}

	bool SocketIODevice::write(const void* data, const buff_t size)
	{
		return os->write(data,size);
	}

	buff_t SocketIODevice::cur_pos() const
	{
		return os->cur_pos();
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