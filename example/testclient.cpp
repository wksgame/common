#include<message/ClientSession.h>
#include<network/Socket.h>
#include<message/msg.pb.h>
#include<other/DateTime.h>
#include<logger/logger.h>
#include<thread/Thread.h>

using namespace kiss;

class ClientThread:public Thread
{
	void Run()
	{
		TCPClientSocket sock;
		sock.CreateSocket("192.168.1.34",4000);
		if(!sock.Connect())
			return;

		Login cts;
		char buff[128]={0};
		int* msgLen = (int*)buff;
		int* msgId = (int*)(buff+4);

		cts.SerializeToArray(buff+8,128);
		*msgLen = cts.ByteSize()+4;
		*msgId = cts.id;

		while(true)
		{
			int result = send(sock.Socket(),buff,*msgLen+4,0);
			if(result<=0)
				break;

			LOG_INFO("send success");

			WaitTime(100);
		}
	}
};


int main()
{
	InitNetwork();

	const int count =500;
	ClientThread* tharr[count];

	for(int i=0; i<count; ++i)
	{
		tharr[i]=new ClientThread();
		tharr[i]->Start();
	}

	getchar();

	CloseNetwork();
	return 0;
}