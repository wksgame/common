#include"GameServer.h"
#include"network/ServerSocket.h"
//#include<thread/SocketThread.h>
#include<thread/EpollThread.h>
#include<thread/WorkThread.h>
#include<logger/logger.h>
#include<message/ClientSession.h>
#include<stdio.h>
#include"GameDB.h"

using namespace std;
using namespace kiss;

#define  PORT 4000
#define  IP_ADDRESS "0.0.0.0"

const unsigned short port = 4000;

bool GameServer::Init()
{
	if(!game::InitGameDB())
		return false;

	srvSock = new ServerSocket();
//	srvSock->CreateSocket(IP_ADDRESS, port);

	if(!srvSock->Init(IP_ADDRESS,port,100))
	{
		LOG_ERROR("server listen socket error");
		return false;
	}

	LOG_HINT("server start listen port:%hd",port);

	recv_threads = new EpollThread*[thread_count];
	for (int i = 0; i < thread_count; ++i)
	{
		recv_threads[i] = new EpollThread("EpollThread");

		recv_threads[i]->Start();
	}

	work_threads = new WorkThread*[thread_count];
	for (int i = 0; i < thread_count; ++i)
	{
		work_threads[i] = new WorkThread("WorkThread");

		work_threads[i]->Start();
	}

	return true;
}

void GameServer::Run()
{
	while (true)
	{
		sock_t clientSocket;
		sockaddr_in clientAddress;

		if (!srvSock->Accept(clientAddress, clientSocket))
			break;

		LOG_INFO("client connect %s:%d",inet_ntoa(clientAddress.sin_addr),clientAddress.sin_port);

		Session* cs = new ClientSession(clientSocket,clientAddress);
		recv_threads[clientSocket % thread_count]->Join(cs);
		work_threads[clientSocket % thread_count]->Join(cs);
	}

	getchar();
}