#include"GameServer.h"
#include<network/Socket.h>
#include<thread/SocketThread.h>
#include<iostream>

using namespace std;
using namespace kiss;

#define  PORT 4000
#define  IP_ADDRESS "0.0.0.0"

bool GameServer::Init()
{
	if (!InitNetwork())
		return false;

	srvSock = new TCPServerSocket();
	srvSock->CreateSocket(IP_ADDRESS, PORT);
	srvSock->Bind();
	srvSock->Listen(100);

	cout << "服务端已经启动" << endl;

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

		string message = "客户端连接::";
		message += inet_ntoa(clientAddress.sin_addr);
		message += ":";
		message += to_string(clientAddress.sin_port);
		message += '\n';

		PrintMessage(message.c_str());
		//cout << "客户端连接::" << inet_ntoa(clientAddress.sin_addr) << ":" << clientAddress.sin_port << endl;

		ClientSession* cs = new ClientSession(clientSocket);
		arr[clientSocket % 4]->Join(cs);
	}

	CloseNetwork();

	getchar();
}