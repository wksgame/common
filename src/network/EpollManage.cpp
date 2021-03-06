#include<sys/epoll.h>
#include<unistd.h>
#include"EpollManage.h"
#include"message/Session.h"

namespace kiss
{
	EpollManage::EpollManage(const int count)
	{
		events_size = count;
		events = new epoll_event[events_size];
		epfd = epoll_create(events_size);
		timeout = -1;
	}
	
	EpollManage::~EpollManage()
	{
		::close(epfd);
		delete [] events;
	}
	
	bool EpollManage::Add(AcceptSocket* s)
	{
		if(cur_size>=events_size)
			return false;

		epoll_event ee;

		ee.events = EPOLLIN
//					|EPOLLOUT
					|EPOLLERR
					|EPOLLRDHUP
					|EPOLLHUP
					|EPOLLET;

		ee.data.ptr = (void*)s;

		int result = epoll_ctl(epfd,EPOLL_CTL_ADD,s->GetSocketFD(),&ee);
		if(result==-1)
			return false;

		++cur_size;
		return true;
	}

	void EpollManage::Remove(AcceptSocket* s)
	{
		epoll_ctl(epfd,EPOLL_CTL_DEL,s->GetSocketFD(),nullptr);
	}

	bool EpollManage::Update()
	{
		auto selret = epoll_wait(epfd, events, events_size, timeout);

		for(int i=0; i<selret; ++i)
		{
			AcceptSocket* s = (AcceptSocket*)events[i].data.ptr;

			if(events[i].events&(EPOLLERR|EPOLLRDHUP|EPOLLHUP))
			{
				epoll_ctl(epfd,EPOLL_CTL_DEL,s->GetSocketFD(),nullptr);
				continue;
			}

			if(events[i].events&EPOLLIN)
			{
				if(!s->Recv())
				{
					Remove(s);
					continue;
				}

// 				if(!s->Update())
// 				{
// 					Remove(s);
// 					continue;
// 				}

// 				if(!Process(s))
// 				{
// 					Remove(s);
// 					continue;
// 				}
			}
/*			
			if(events[i].events&EPOLLOUT)
			{
				if(!sock->Send())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),nullptr);
					sock->enable = false;
					continue;
				}
			}*/
		}

		return true;
	}
}//namespace kiss