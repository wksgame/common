#ifndef SLG_EPOLL_MANAGE_H
#define SLG_EPOLL_MANAGE_H

struct epoll_event;

namespace kiss
{
	class Session;

	class EpollManage
	{
	public:
		EpollManage(const int count);
		~EpollManage();
		
		bool Add(Session* s);
		void Remove(Session* s);

		bool Update();
		bool Process(Session* sock){return true;}

	private:
		epoll_event* events;
		int epfd;
		int events_size;
		int cur_size;
		int timeout;
	};//class EpollManage
}//namespace kiss

#endif//SLG_EPOLL_MANAGE_H