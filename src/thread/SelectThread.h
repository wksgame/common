#ifndef KISS_SELECT_THREAD_H
#define KISS_SELECT_THREAD_H

#include"SocketThread.h"
#include<list>
#include<mutex>
#include<platform/platform.h>

namespace kiss
{
	class AcceptSocket;
	class SelectManage;

	class SelectThread:public SocketThread
	{
		SelectThread(const SelectThread&)=delete;
		SelectThread& operator=(const SelectThread&)=delete;

	public:
		SelectThread();
		~SelectThread();

		bool Add(kiss::AcceptSocket* s)override;
		void Remove(kiss::AcceptSocket* s)override;

	protected:
		void Update()override;

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