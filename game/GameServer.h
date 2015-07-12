#ifndef GAME_SERVER_H
#define GAME_SERVER_H

namespace kiss
{
	class TCPServerSocket;
}

class SocketThread;

class GameServer
{
	kiss::TCPServerSocket* srvSock;
	SocketThread** arr;

public:

	bool Init();
	void Run();
};
#endif//GAME_SERVER_H