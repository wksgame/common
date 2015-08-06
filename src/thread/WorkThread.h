#ifndef KISS_WORK_THREAD_H
#define KISS_WORK_THREAD_H

#include"Thread.h"
#include<list>
#include<mutex>
#include<platform/platform.h>

namespace game
{
	class GameDB;
}

namespace kiss
{
	class ClientSession;

	class WorkThread : public Thread
	{
	public:
		WorkThread(const char* thread_name);

		void Join(ClientSession*cs);

	protected:
		void Run()override;
		void Update();

	public:
		game::GameDB* db;
	private:
		std::list<ClientSession*>  clients;
		std::list<ClientSession*>  joinClients;
		std::list<ClientSession*>  quitClients;
		std::mutex joinLock;
		std::mutex quitLock;

//		timeval timeout;

		double cur_time;
		double last_time;
		double sleep_time;
	};//class WorkThread
}//namespace kiss
#endif//KISS_WORK_THREAD_H