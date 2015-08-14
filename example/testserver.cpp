#include<game/GameServer.h>

int main()
{
	GameServer server;

	if(!server.Init())
		return -1;

	server.Run();
    return 0;
}