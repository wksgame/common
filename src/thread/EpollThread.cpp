#include"EpollThread.h"
#include<other/DateTime.h>
#include<platform/platform.h>
#include<network/IOSocket.h>
#include<logger/logger.h>
#include<sys/epoll.h>

namespace kiss 
{
	EpollThread::EpollThread(const char* thread_name): Thread(thread_name)
	{
		events_size = 2000;
		events = new epoll_event[events_size];
		epfd = epoll_create(events_size);
		timeout = -1;
	}
	
	EpollThread::~EpollThread()
	{
		::close(epfd);
		SAFE_DELETE_ARRAY(events);
	}
	
	void EpollThread::Join(TCPIOSocket* sock)
	{
		joinLock.lock();
			joinClients.push_back(sock);
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
		//add new_player
		if (joinLock.try_lock())
		{
			if(joinClients.size()>0)
			{
				for (auto sock : joinClients)
				{
					epoll_event ee;
					ee.events = EPOLLIN|EPOLLOUT|EPOLLERR;
					ee.data.ptr = (void*)sock;
					epoll_ctl(epfd,EPOLL_CTL_ADD,sock->Socket(),&ee);
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
			TCPIOSocket* sock = (TCPIOSocket*)events[i].data.ptr;

			if(events[i].events&EPOLLERR)
			{
				epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),0);
				sock->enable = false;
				continue;
			}

			if(events[i].events&EPOLLIN)
			{
				if(!sock->Recv())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),0);
					sock->enable = false;
					continue;
				}
			}
			
			if(events[i].events&EPOLLOUT)
			{
				if(!sock->Send())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),0);
					sock->enable = false;
					continue;
				}
			}
		}
	}
}//namespace kiss