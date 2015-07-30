#ifndef KISS_SOCKET_IO_DEVICE_H
#define KISS_SOCKET_IO_DEVICE_H

#include"IODevice.h"

namespace kiss
{
	class SocketIODevice:public IODevice
	{
		int sock;
	public:
		SocketIODevice(const int sock);
		~SocketIODevice();

		void close()override;
	};//class SocketIODevice
}//namespace kiss

#endif//KISS_SOCKET_IO_DEVICE_H