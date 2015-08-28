#include"SocketThread.h"
#include"other/RingBuffer.h"
#include<other/DateTime.h>
#include<platform/platform.h>
#include<message/ClientSession.h>
#include<logger/logger.h>
#include<string.h>

using namespace std;

namespace kiss
{
	SocketThread::SocketThread():Thread("SocketThread")
	{
		timeout.tv_sec=1;
		timeout.tv_usec = 0;
		FD_ZERO(&all_sock);
		FD_ZERO(&recv_sock);
		FD_ZERO(&send_sock);
		FD_ZERO(&err_sock);
		max_sock = 0;
		cur_time = 0;
		last_time = 0;
		sleep_time = 0;
	}

	SocketThread::~SocketThread()
	{
	}

	void SocketThread::Run()
	{
		while (true)
		{
			Update();
		}
	}

	void SocketThread::Join(ClientSession*cs)
	{
		joinLock.lock();
			joinClients.push_back(cs);
		joinLock.unlock();
	}

	void SocketThread::Update()
	{
		cur_time = NowTime();

		//add new_player
		if (joinLock.try_lock())
		{
			if(joinClients.size()>0)
			{
				for (auto i : joinClients)
				{
					if(max_sock < i->sock->Socket())
						max_sock=i->sock->Socket()+1;

					FD_SET(i->sock->Socket(), &all_sock);
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

		memcpy(&recv_sock, &all_sock, sizeof(all_sock));
		memcpy(&send_sock, &all_sock, sizeof(all_sock));
		memcpy(&err_sock, &all_sock, sizeof(all_sock));

		auto selret = select(max_sock, &recv_sock, &send_sock, &err_sock, nullptr);//&timeout);

		auto clientsIter = clients.begin();
		while (clientsIter != clients.end())
		{
			auto cs = *clientsIter;
			if (FD_ISSET(cs->sock->Socket(), &err_sock))
			{
				//quitClients.push_back(cs);
				
				FD_CLR(cs->sock->Socket(),&all_sock);
				clientsIter = clients.erase(clientsIter);
				
				continue;
			}

			if(FD_ISSET(cs->sock->Socket(), &recv_sock))
			{
				if (!cs->sock->Recv())
				{
					//quitClients.push_back(cs);
				
					FD_CLR(cs->sock->Socket(),&all_sock);
					clientsIter = clients.erase(clientsIter);
				
					continue;
				}
			}
			
// 			if (FD_ISSET(cs->sock->Socket(), &send_sock))
// 			{
// 				if (!cs->sock->Send())
// 				{
// 					//quitClients.push_back(cs);
// 				
// 					FD_CLR(cs->sock->Socket(),&all_sock);
// 					clientsIter = clients.erase(clientsIter);
// 					
// 					continue;
// 				}
// 			}

			++clientsIter;
		}

//		quitClients.clear();
	}
}//namespace kiss