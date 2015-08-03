#include"WorkThread.h"
#include"other/RingBuffer.h"
#include<other/DateTime.h>
#include<platform/platform.h>
#include<message/ClientSession.h>
#include<logger/logger.h>
#include<string.h>

namespace kiss
{
	WorkThread::WorkThread(const char* thread_name): Thread(thread_name)
	{
		cur_time = 0;
		last_time = 0;
		sleep_time = 0;
	}

	void WorkThread::Run()
	{
		while (true)
		{
			Update();
		}
	}

	void WorkThread::Join(ClientSession* cs)
	{
		joinLock.lock();
			joinClients.push_back(cs);
		joinLock.unlock();
	}

	void WorkThread::Update()
	{
		cur_time = NowTime();

		if(last_time> cur_time-1)
		{
			WaitTime(cur_time-last_time);
		}

		last_time = cur_time;

		//add new_player
		if (joinLock.try_lock())
		{
			if(joinClients.size()>0)
			{
				clients.insert(clients.end(), joinClients.begin(), joinClients.end());
				joinClients.clear();
			}

			joinLock.unlock();
		}

		if (clients.size() <= 0)
			WaitTime(1000);

		//process message
		auto clientsIter = clients.begin();
		while (clientsIter != clients.end())
		{
			auto cs = *clientsIter;
			if (!cs->Update(cur_time))
			{
				quitClients.push_back(cs);
				clientsIter = clients.erase(clientsIter);
			}
			else
				++clientsIter;
		}

		//clost error socket
		for (auto i : quitClients)
		{
			delete i;
		}

		quitClients.clear();
	}

}//namespace kiss