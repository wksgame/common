#ifndef KISS_IO_SOCKET_H
#define KISS_IO_SOCKET_H

#include"Socket.h"

namespace kiss
{
	class RingBuffer;

	class TCPIOSocket:public TCPClientSocket
	{
		RingBuffer* readBuff;
		RingBuffer* writeBuff;
		
	public:
		TCPIOSocket(const int buffSize);
		TCPIOSocket(const sock_t sock,const sockaddr_in& address,const int buffSize);
		
		bool Recv();
		bool Send();
		
		bool Read(char* b, const uint64 size);				// read from buffer
		bool Write(const char* b, const uint64 size);		// write to buffer
		
		bool Peek(char* b, const uint64 size);
		bool Skip(const uint64 size);
	};
}//namespace kiss
#endif//KISS_IO_SOCKET_H