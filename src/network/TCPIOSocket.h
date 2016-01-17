#ifndef KISS_TCP_IO_SOCKET_H
#define KISS_TCP_IO_SOCKET_H

#include"TCPSocket.h"
#include"thread/Mutex.h"
#include<atomic>

namespace kiss
{
	class RingBuffer;

	/**
	 * @brief 有读写需求的TCPSocket
	 */
	class TCPIOSocket:public TCPSocket
	{
		RingBuffer* readBuff;
		RingBuffer* writeBuff;

		Mutex<true> read_mutex;
		Mutex<true> write_mutex;
		
	public:
		std::atomic<bool> enable;
		
	public:
		TCPIOSocket(const uint32 buffSize);
		~TCPIOSocket();
		
		bool Recv();
		bool Send(const char* b, const uint64 size);
		
		bool Read(char* b, const uint64 size);				// read from buffer
		bool Write(const char* b, const uint64 size);		// write to buffer
		
		bool Peek(char* b, const uint64 size);
		bool Skip(const uint64 size);

	};//class TCPIOSocket

}//namespace kiss

#endif//KISS_TCP_IO_SOCKET_H