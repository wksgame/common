#ifndef GAME_SERVER_H
#define GAME_SERVER_H

namespace kiss
{
	class TCPServerSocket;
	class SocketThread;
	class WorkThread;
}

class GameServer
{
	kiss::TCPServerSocket* srvSock;
	kiss::SocketThread** recv_threads;
	kiss::WorkThread** work_threads;

public:

	bool Init();
	void Run();
};//class GameServer
#endif//GAME_SERVER_H