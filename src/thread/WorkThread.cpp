#include"WorkThread.h"
#include"other/RingBuffer.h"
#include<other/DateTime.h>
#include<platform/platform.h>
#include<message/ClientSession.h>
#include<logger/logger.h>
#include<string.h>
#include<game/GameDB.h>

namespace kiss
{
	WorkThread::WorkThread(): Thread()
	{
		cur_time = 0;
		last_time = 0;
		sleep_time = 0;
		db = new game::GameDB();
	}

	void WorkThread::Run()
	{
		while (true)
		{
			Update();
		}
	}

	void WorkThread::Add(Session* cs)
	{
		joinLock.lock();
			//cs->work_thread = this;
			cs->SetWorkThread(this);
			joinClients.push_back(cs);
		joinLock.unlock();
	}

	void WorkThread::Update()
	{
		cur_time = NowTime();

		if(last_time> cur_time-1)
		{
			WaitTime((cur_time-last_time)*1000);
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
		{
			WaitTime(1000);
			return;
		}

		//process message
		auto clientsIter = clients.begin();
		while (clientsIter != clients.end())
		{
			auto cs = *clientsIter;
			if (!cs->Update())
			{
//				quitClients.push_back(cs);
				clientsIter = clients.erase(clientsIter);
				syslogger.warn("client quit socket:%d",cs->sock->GetSocketFD());
				SAFE_DELETE(cs);
			}
			else
				++clientsIter;
		}
	}

}//namespace kiss