#include"EpollThread.h"

#include"other/RingBuffer.h"
#include<other/DateTime.h>
#include<platform/platform.h>
#include<message/ClientSession.h>
#include<logger/logger.h>
#include<string.h>

namespace kiss 
{
	EpollThread::EpollThread(const char* thread_name): Thread(thread_name)
	{
		events_size = 10000;
		events = new epoll_event[events_size];
		epfd = epoll_create(events_size);
		timeout = -1;
	}
	
	EpollThread::~EpollThread()
	{
		::close(epfd);
	}
	
	void EpollThread::Join(ClientSession* cs)
	{
		joinLock.lock();
			joinClients.push_back(cs);
		joinLock.unlock();
	}

	void EpollThread::Run()
	{
		while (true)
		{
			Update();
		}
	}

	void EpollThread::Update()
	{
		cur_time = NowTime();

		//add new_player
		if (joinLock.try_lock())
		{
			if(joinClients.size()>0)
			{
				for (auto i : joinClients)
				{
					epoll_event ee;
					ee.events = EPOLLIN|EPOLLOUT|EPOLLERR;
					ee.data.ptr = (void*)i;
					epoll_ctl(epfd,EPOLL_CTL_ADD,i->sock->Socket(),&ee);
				}

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

		auto selret = epoll_wait(epfd, events, events_size, timeout);

		for(int i=0; i<selret; ++i)
		{
			ClientSession* cs = (ClientSession*)events[i].data.ptr;

			if(events[i].events&EPOLLERR)
			{
				epoll_ctl(epfd,EPOLL_CTL_DEL,cs->sock->Socket(),0);
				continue;
			}

			if(events[i].events&EPOLLIN)
			{
				if(!cs->sock->Recv())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,cs->sock->Socket(),0);
					continue;
				}
			}
			
			if(events[i].events&EPOLLOUT)
			{
				if(!cs->sock->Send())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,cs->sock->Socket(),0);
					continue;
				}
			}
		}
	}

}//namespace kiss