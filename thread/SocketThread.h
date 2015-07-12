#ifndef KISS_SOCKET_THREAD_H
#define KISS_SOCKET_THREAD_H

#include"Thread.h"
#include"message/ClientSession.h"
#include<list>
#include<mutex>
#include<winsock.h>

class ClientSession;

void PrintMessage(const char* str);

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
};//class SocketThread

#endif//KISS_SOCKET_THREAD_H