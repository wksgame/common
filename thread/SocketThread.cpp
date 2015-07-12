#include"SocketThread.h"
#include<iostream>
#include<winsock.h>
#include"other/RingBuffer.h"

using namespace std;


SocketThread::SocketThread()
{
	timeout.tv_sec=1;
	timeout.tv_usec = 0;
	FD_ZERO(&all_sock);
	FD_ZERO(&recv_sock);
	FD_ZERO(&send_sock);
	FD_ZERO(&err_sock);
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
	double cur_time = (double)clock()/CLOCKS_PER_SEC;
	//加入新玩家
	if (joinLock.try_lock())
	{
		for (auto i : joinClients)
		{
			FD_SET(i->sock, &all_sock);
		}

		clients.insert(clients.end(), joinClients.begin(), joinClients.end());
		joinClients.clear();

		joinLock.unlock();
	}

	if (clients.size() <= 0)
		Sleep(1000);

	memcpy(&recv_sock, &all_sock, sizeof(all_sock));
	memcpy(&send_sock, &all_sock, sizeof(all_sock));
	memcpy(&err_sock, &all_sock, sizeof(all_sock));

	auto selret = select(0, &recv_sock, &send_sock, &err_sock, &timeout);
	
	//读取数据
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

	//处理数据
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

	//发送数据
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
	
	//处理socket出错玩家
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

	//处理玩家退出
	for (auto i : quitClients)
	{
		FD_CLR(i->sock, &all_sock);
		closesocket(i->sock);
		delete i;
	}
		
	quitClients.clear();
}

std::mutex msgLock;
void PrintMessage(const char* str)
{
	msgLock.lock();
	printf(str);
	msgLock.unlock();
}