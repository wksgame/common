#ifndef SGL_ACCEPT_SOCKET_H
#define SGL_ACCEPT_SOCKET_H

#include <netinet/in.h>

typedef unsigned long long int uint64;

namespace kiss
{
	class RingBuffer;

	/**
	 * @brief 服务器端接入的客户端连接
	 */
	class AcceptSocket
	{
		RingBuffer* readBuff;
		RingBuffer* writeBuff;

		int sockfd;
		sockaddr_in address;

	public:

		AcceptSocket(const int sock,const sockaddr_in& address,const int buffSize=64*1024);
		~AcceptSocket();

		inline int GetSocketFD(){return sockfd;}
		
		bool Recv();
		bool Send(const char* b, const uint64 size);
		
		bool Read(char* b, const uint64 size);				// read from buffer
		bool Write(const char* b, const uint64 size);		// write to buffer
		
		bool Peek(char* b, const uint64 size);
		bool Skip(const uint64 size);
	};
}//namespace sgl
#endif//SGL_ACCEPT_SOCKET_H