#ifndef KISS_SOCKET_THREAD_H
#define KISS_SOCKET_THREAD_H

#include"Thread.h"

class SocketThread:public Thread
{
public:
	SocketThread();
	~SocketThread();

protected:

	void Run()override;
private:

};//class SocketThread

#endif//KISS_SOCKET_THREAD_H