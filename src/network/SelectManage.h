#ifndef KISS_SELECT_MANAGE_H
#define KISS_SELECT_MANAGE_H

#include<list>
#include<mutex>
#include<platform/platform.h>
#include"SocketManage.h"

namespace kiss
{
	class Session;

	class SelectManage : public SocketManage
	{
		SelectManage(const SelectManage&)=delete;
		SelectManage& operator=(const SelectManage&)=delete;

	public:
		SelectManage();
		~SelectManage();

		bool Add(AcceptSocket* s);
		void Remove(AcceptSocket* s);

		void ProcessAdd();
		void ProcessRemove();

		bool Update()override;

	private:

		std::list<AcceptSocket*>  clients;
		std::list<AcceptSocket*>  joinClients;
		std::list<AcceptSocket*>  quitClients;

		timeval timeout;
		fd_set all_sock;
		fd_set recv_sock;
		fd_set send_sock;
		fd_set err_sock;
		sock_t max_sock;
	};//class SelectManage

}//namespace kiss

#endif//KISS_SELECT_MANAGE_H