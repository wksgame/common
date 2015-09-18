#ifndef KISS_SELECT_THREAD_H
#define KISS_SELECT_THREAD_H

#include"Thread.h"
#include<list>
#include<mutex>
#include<platform/platform.h>

namespace kiss
{
	class Session;
	class SelectManage;

	class SocketThread:public Thread
	{
		SocketThread(const SocketThread&)=delete;
		SocketThread& operator=(const SocketThread&)=delete;

	public:
		SocketThread();
		~SocketThread();

		bool Add(Session* s);
		void Remove(Session* s);

	protected:
		void Update();

	private:
		SelectManage* manage;

		std::mutex joinLock;
		std::mutex quitLock;

		double cur_time;
		double last_time;
		double sleep_time;

	};//class SocketThread

}//namespace kiss

#endif//KISS_SELECT_THREAD_H