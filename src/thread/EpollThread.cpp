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
		epoll_event ee;

		ee.events = EPOLLIN
					|EPOLLOUT
					|EPOLLERR
					|EPOLLRDHUP
					|EPOLLHUP;
//					|EPOLLET;

		ee.data.ptr = (void*)sock;

		int result = epoll_ctl(epfd,EPOLL_CTL_ADD,sock->Socket(),&ee);
		if(result==-1)
			sock->enable = false;
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
		auto selret = epoll_wait(epfd, events, events_size, timeout);

		for(int i=0; i<selret; ++i)
		{
			TCPIOSocket* sock = (TCPIOSocket*)events[i].data.ptr;

			if(events[i].events&(EPOLLERR|EPOLLRDHUP|EPOLLHUP))
			{
				epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),nullptr);
				sock->enable = false;
				continue;
			}

			if(events[i].events&EPOLLIN)
			{
				if(!sock->Recv())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),nullptr);
					sock->enable = false;
					continue;
				}
			}
			
			if(events[i].events&EPOLLOUT)
			{
				if(!sock->Send())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),nullptr);
					sock->enable = false;
					continue;
				}
			}
		}
	}
}//namespace kiss