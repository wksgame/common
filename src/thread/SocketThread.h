#ifndef KISS_SOCKET_THREAD_H
#define KISS_SOCKET_THREAD_H

#include"Thread.h"

namespace kiss
{
	class AcceptSocket;
	class ISocketManage;

	class SocketThread : public Thread
	{
	public:
		//SocketThread(){}
		//virtual ~SocketThread(){}
		
		virtual bool Add(kiss::AcceptSocket* sock)=0;

	//protected:
//		void Run()=0;
		//virtual void Update()=0;

		//SocketManage* manage;
	};//class SocketThread

}//namespace kiss

#endif//KISS_SOCKET_THREAD_H