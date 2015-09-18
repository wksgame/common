#ifndef KISS_SELECT_MANAGE_H
#define KISS_SELECT_MANAGE_H

#include<list>
#include<mutex>
#include<platform/platform.h>

namespace kiss
{
	class Session;

	class SelectManage
	{
		SelectManage(const SelectManage&)=delete;
		SelectManage& operator=(const SelectManage&)=delete;

	public:
		SelectManage();
		~SelectManage();

		bool Add(Session* s);
		void Remove(Session* s);

		void ProcessAdd();
		void ProcessRemove();

		void Update();

	private:

		std::list<Session*>  clients;
		std::list<Session*>  joinClients;
		std::list<Session*>  quitClients;

		timeval timeout;
		fd_set all_sock;
		fd_set recv_sock;
		fd_set send_sock;
		fd_set err_sock;
		sock_t max_sock;
	};//class SelectManage

}//namespace kiss

#endif//KISS_SELECT_MANAGE_H