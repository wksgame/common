#ifndef KISS_SOCKET_THREAD_H
#define KISS_SOCKET_THREAD_H

#include"Thread.h"
#include<list>
#include<mutex>
#include<platform/platform.h>

namespace kiss
{
	class ClientSession;

	class SocketThread:public Thread
	{
	public:
		SocketThread();
		~SocketThread();

		void Join(ClientSession*cs);

	protected:
		void Run()override;
		void Update();

	private:
		std::list<ClientSession*>  clients;
		std::list<ClientSession*>  joinClients;
		std::list<ClientSession*>  quitClients;
		std::mutex joinLock;
		std::mutex quitLock;

		timeval timeout;
		fd_set all_sock;
		fd_set recv_sock;
		fd_set send_sock;
		fd_set err_sock;
		sock_t max_sock;

		double cur_time;
		double last_time;
		double sleep_time;
	};//class SocketThread
}//namespace kiss
#endif//KISS_SOCKET_THREAD_H