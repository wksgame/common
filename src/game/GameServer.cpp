#include"GameServer.h"
#include<network/Socket.h>
#include<thread/SocketThread.h>
#include<logger/logger.h>
#include<message/ClientSession.h>

using namespace std;
using namespace kiss;

#define  PORT 4000
#define  IP_ADDRESS "0.0.0.0"

bool GameServer::Init()
{
	strncpy(thread_name,"GameServer",64);
	thread_id =5;

	if (!InitNetwork())
		return false;

	srvSock = new TCPServerSocket();
	srvSock->CreateSocket(IP_ADDRESS, PORT);
	srvSock->Bind();
	srvSock->Listen(100);

	logger("server start");

	arr = new SocketThread*[4];
	for (int i = 0; i < 4; ++i)
	{
		arr[i] = new SocketThread();

		arr[i]->Start();
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

		string message = "client connect ";
		message += inet_ntoa(clientAddress.sin_addr);
		message += ":";
		message += to_string(clientAddress.sin_port);

		logger(message.c_str());

		ClientSession* cs = new ClientSession(clientSocket);
		arr[clientSocket % 4]->Join(cs);
	}

	CloseNetwork();

	getchar();
}