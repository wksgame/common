#ifndef GAME_SERVER_H
#define GAME_SERVER_H

namespace kiss
{
	class TCPServerSocket;
	class SocketThread;
	class WorkThread;
	class EpollThread;
}

class GameServer
{
	kiss::TCPServerSocket* srvSock;
	//kiss::SocketThread** recv_threads;
	kiss::WorkThread** work_threads;
	kiss::EpollThread** recv_threads;
	const int thread_count = 1;

public:

	bool Init();
	void Run();
};//class GameServer
#endif//GAME_SERVER_H