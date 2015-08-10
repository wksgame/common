#ifndef KISS_SESSION_H
#define KISS_SESSION_H

#include<network/IOSocket.h>

namespace kiss
{
	class SocketThread;
	class EpollThread;
	class WorkThread;

	class Session
	{
		friend SocketThread;
		friend EpollThread;
		friend WorkThread;

		Session(const Session&)=delete;
		Session& operator=(const Session&)=delete;

	public:
		Session(const int sock,const sockaddr_in& address,const int buffSize);
		virtual ~Session();

		bool Update();

		virtual bool ProcessMessage(const char* data, const int size)=0;

	public:
		TCPIOSocket* sock;
		WorkThread* work_thread;

		unsigned int msgSize;		// current process message size
	};//class Session
}//namespace kiss
#endif//KISS_SESSION_H