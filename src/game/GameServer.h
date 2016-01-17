#ifndef GAME_SERVER_H
#define GAME_SERVER_H

namespace kiss
{
	class ServerSocket;
	class SocketThread;
	class WorkThread;
}

class GameServer
{
	kiss::ServerSocket* srvSock;
	kiss::WorkThread** work_threads;
	kiss::SocketThread** recv_threads;
	const int thread_count = 1;

public:

	bool Init();
	void Run();
};//class GameServer
#endif//GAME_SERVER_H