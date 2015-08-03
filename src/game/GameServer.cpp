#include"GameServer.h"
#include<network/Socket.h>
#include<thread/SocketThread.h>
#include<thread/WorkThread.h>
#include<logger/logger.h>
#include<message/ClientSession.h>
#include<stdio.h>

using namespace std;
using namespace kiss;

#define  PORT 4000
#define  IP_ADDRESS "0.0.0.0"

const char* green_color="\e[0;40;32m";
const char* normal_color="\e[0m";

bool GameServer::Init()
{
	if (!InitNetwork())
		return false;

	srvSock = new TCPServerSocket();
	srvSock->CreateSocket(IP_ADDRESS, PORT);

	if(!srvSock->Bind())
	{
		LOG_ERROR("server bind socket error");
		return false;
	}

	if(!srvSock->Listen(100))
	{
		LOG_ERROR("server listen socket error");
		return false;
	}

	LOG_HINT("server start listen port:%d",PORT);

	recv_threads = new SocketThread*[4];
	for (int i = 0; i < 4; ++i)
	{
		recv_threads[i] = new SocketThread();

		recv_threads[i]->Start();
	}

	work_threads = new WorkThread*[4];
	for (int i = 0; i < 4; ++i)
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

		ClientSession* cs = new ClientSession(clientSocket,clientAddress);
		recv_threads[clientSocket % 4]->Join(cs);
		work_threads[clientSocket % 4]->Join(cs);
	}

	CloseNetwork();

	getchar();
}