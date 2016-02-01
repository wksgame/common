#include"SelectManage.h"
#include"other/RingBuffer.h"
#include<other/DateTime.h>
#include<platform/platform.h>
#include<message/ClientSession.h>
#include<string.h>

using namespace std;

namespace kiss
{
	SelectManage::SelectManage()
	{
		timeout.tv_sec=1;
		timeout.tv_usec = 0;
		FD_ZERO(&all_sock);
		FD_ZERO(&recv_sock);
		FD_ZERO(&send_sock);
		FD_ZERO(&err_sock);
		max_sock = 0;
	}

	SelectManage::~SelectManage()
	{
	}

	bool SelectManage::Add(kiss::AcceptSocket* s)
	{
		joinClients.push_back(s);
	}

	void SelectManage::Remove(kiss::AcceptSocket* s)
	{
		quitClients.push_back(s);
	}

	void SelectManage::ProcessAdd()
	{
		if(joinClients.size()>0)
		{
			for (AcceptSocket* sock : joinClients)
			{
				if(max_sock < sock->GetSocketFD())
					max_sock=sock->GetSocketFD()+1;

				FD_SET(sock->GetSocketFD(), &all_sock);
			}

			clients.insert(clients.end(), joinClients.begin(), joinClients.end());
			joinClients.clear();
		}
	}

	void SelectManage::ProcessRemove()
	{
		for(AcceptSocket* i:quitClients)
		{
			FD_CLR(i->GetSocketFD(),&all_sock);
			clients.remove(i);
		}
		quitClients.clear();
	}

	bool SelectManage::Update()
	{
		memcpy(&recv_sock, &all_sock, sizeof(all_sock));
		memcpy(&send_sock, &all_sock, sizeof(all_sock));
		memcpy(&err_sock, &all_sock, sizeof(all_sock));

		auto selret = ::select(max_sock, &recv_sock, &send_sock, &err_sock, nullptr);//&timeout);

		auto clientsIter = clients.begin();
		while (clientsIter != clients.end())
		{
			auto cs = *clientsIter;
			if (FD_ISSET(cs->GetSocketFD(), &err_sock))
			{
				//quitClients.push_back(cs);

				FD_CLR(cs->GetSocketFD(),&all_sock);
				clientsIter = clients.erase(clientsIter);

				continue;
			}

			if(FD_ISSET(cs->GetSocketFD(), &recv_sock))
			{
				if (!cs->Recv())
				{
					//quitClients.push_back(cs);

					FD_CLR(cs->GetSocketFD(),&all_sock);
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
	}
}//namespace kiss