#ifndef KISS_SESSION_H
#define KISS_SESSION_H

#include<network/AcceptSocket.h>

namespace kiss
{
	class SocketThread;
	class EpollThread;
	class WorkThread;

	class Session
	{
//		friend SocketThread;
//		friend EpollThread;
//		friend WorkThread;

		Session(const Session&)=delete;
		Session& operator=(const Session&)=delete;

	public:
		Session(AcceptSocket* as);
		virtual ~Session();

		//bool Recv();
		bool Update();

		virtual bool ProcessMessage(const char* data, const int size)=0;
		virtual bool SendMessage(const char* data, const int size)=0;

	public:
		AcceptSocket* sock;

		void SetWorkThread(WorkThread* wt);
	protected:
		WorkThread* work_thread;

		unsigned int msgSize;		// current process message size
	};//class Session
}//namespace kiss
#endif//KISS_SESSION_H