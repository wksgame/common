#ifndef KISS_SOCKET_IO_DEVICE_H
#define KISS_SOCKET_IO_DEVICE_H

#include"IOStream.h"

namespace kiss
{
	class SocketInputStream;
	class SocketOutputStream;

	class SocketIODevice:public IOStream
	{
		int sock;
		SocketInputStream* is;
		SocketOutputStream* os;

	public:
		SocketIODevice(const int sock);
		~SocketIODevice();

		bool read(void* data, const buff_t size)override;
		bool peek(void* data, const buff_t size)override;
		bool skip(const buff_t size)override;
		
		bool write(const void* data, const buff_t size)override;
		buff_t cur_pos()const override;

		void close()override;
	};//class SocketIODevice
}//namespace kiss

#endif//KISS_SOCKET_IO_DEVICE_H