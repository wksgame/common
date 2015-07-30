#ifndef GAME_SERVER_H
#define GAME_SERVER_H

namespace kiss
{
	class TCPServerSocket;
	class SocketThread;
}

class GameServer
{
	kiss::TCPServerSocket* srvSock;
	kiss::SocketThread** arr;

public:

	bool Init();
	void Run();
};//class GameServer
#endif//GAME_SERVER_H