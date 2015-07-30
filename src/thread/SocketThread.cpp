#include"SocketThread.h"
#include"other/RingBuffer.h"
#include<other/DateTime.h>
#include<platform/platform.h>
#include<message/ClientSession.h>

using namespace std;

namespace kiss
{
	SocketThread::SocketThread()
	{
		timeout.tv_sec=1;
		timeout.tv_usec = 0;
		FD_ZERO(&all_sock);
		FD_ZERO(&recv_sock);
		FD_ZERO(&send_sock);
		FD_ZERO(&err_sock);
		max_sock = 0;
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
		double cur_time = NowTime();
		//���������
		if (joinLock.try_lock())
		{
			if(joinClients.size()>0)
			{
				for (auto i : joinClients)
				{
					if(max_sock < i->sock)
						max_sock=i->sock+1;

					FD_SET(i->sock, &all_sock);
				}

				clients.insert(clients.end(), joinClients.begin(), joinClients.end());
				joinClients.clear();
			}

			joinLock.unlock();
		}

		if (clients.size() <= 0)
			WaitTime(1000);

		memcpy(&recv_sock, &all_sock, sizeof(all_sock));
		memcpy(&send_sock, &all_sock, sizeof(all_sock));
		memcpy(&err_sock, &all_sock, sizeof(all_sock));

		auto selret = select(max_sock, &recv_sock, &send_sock, &err_sock, &timeout);

		//��ȡ����
		const int buffSize = 32 * 1024;
		char buff[buffSize] = {};
		auto clientsIter = clients.begin();
		if (selret > 0)
		{
			while (clientsIter != clients.end())
			{
				auto cs = *clientsIter;
				if (FD_ISSET(cs->sock, &recv_sock))
				{
					auto result = ::recv(cs->sock, buff, buffSize, 0);
					if (result <= 0 || !cs->readBuff->write(buff, result))
					{
						quitClients.push_back(cs);
						clientsIter = clients.erase(clientsIter);
					}
					else
						++clientsIter;
				}
				else
					++clientsIter;
			}
		}

		//��������
		clientsIter = clients.begin();
		while (clientsIter != clients.end())
		{
			auto cs = *clientsIter;
			if (!cs->Update(cur_time))
			{
				quitClients.push_back(cs);
				clientsIter = clients.erase(clientsIter);
			}
			else
				++clientsIter;
		}

		//��������
		if (selret > 0)
		{
			clientsIter = clients.begin();
			while (clientsIter != clients.end())
			{
				auto cs = *clientsIter;
				auto sendSize = cs->writeBuff->readSize();
				if (FD_ISSET(cs->sock, &send_sock) && sendSize>0)
				{
					sendSize = sendSize < buffSize ? sendSize : buffSize;
					cs->writeBuff->read(buff, sendSize);
					auto result = ::send(cs->sock, buff, sendSize, 0);
					if (result < sendSize)
					{
						quitClients.push_back(cs);
						clientsIter = clients.erase(clientsIter);
					}
					else
						++clientsIter;
				}
				else
					++clientsIter;
			}
		}

		//����socket�������
		if (selret > 0)
		{
			clientsIter = clients.begin();
			while (clientsIter != clients.end())
			{
				auto cs = *clientsIter;
				if (FD_ISSET(cs->sock, &err_sock))
				{
					quitClients.push_back(cs);
					clientsIter = clients.erase(clientsIter);
				}
				else
					++clientsIter;
			}
		}

		//��������˳�
		for (auto i : quitClients)
		{
			FD_CLR(i->sock, &all_sock);
			closesocket(i->sock);
			delete i;
		}

		quitClients.clear();
	}
}//namespace kiss