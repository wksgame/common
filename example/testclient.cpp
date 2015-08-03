#include<message/ProtobufMessageProcess.h>
#include<message/ProtobufMessageSend.h>
#include<network/IOSocket.h>
#include<message/msg.pb.h>
#include<other/DateTime.h>
#include<logger/logger.h>
#include<thread/Thread.h>
#include<atomic>

using namespace kiss;
using namespace std;

std::atomic<int>	cur_count;

class ClientSession
{
	kiss::pb::ProtobufMessageProcess<ClientSession> messageProcess;
	kiss::pb::ProtobufMessageSend<> messageSend;
	
	TCPIOSocket* sock;
	
	int msgId;
	unsigned int msgSize;

	double cur_time;

public:
	ClientSession()
	{
		messageProcess.RegisterMessage(2, &ClientSession::OnLogin, new s2cLogin());
		
		sock = new TCPIOSocket(1024);
		sock->CreateSocket("127.0.0.1",4000);
		
		//sock->SetBlockTimeOut(10,0);

		if(!sock->Connect())
			return;
	}
	
	~ClientSession()
	{
		delete sock;
	}

	bool Update(const double cur_time)
	{
		if(!sock->Recv())
			return false;
		
		const int buffSize = 1024;
		char tempBuff[1024] = {};
		
		if(msgSize==0)
		{
			if(!sock->Read((char*)&msgSize, 4))
				return true;
			
			if(msgSize<4 || msgSize>1024)
				return false;
		}
		
		if(!sock->Read(tempBuff,msgSize))
			return true;
		
		msgId = *((int*)tempBuff);

		auto result = messageProcess.Process(msgId, tempBuff+4, msgSize - 4);
		msgSize = 0;
		msgId = -1;
			
		return true;
	}
	
	bool Send()
	{
		char account_name[32];
		snprintf(account_name,32,"name%d",sock->Socket());
		
		c2sLogin cts;
		cts.set_account_name(account_name);
		cts.set_password("123456");
		
		messageSend.Append(&cts);

		if(!sock->Write(messageSend.buff,messageSend.curPos))
		{
			LOG_ERROR("buff full");
			return false;
		}
		
		if(!sock->Send())
			return false;

		messageSend.ClearData();

		return true;
	}

	bool OnLogin(const google::protobuf::MessageLite* msg)
	{
		s2cLogin* s2c = (s2cLogin*)msg;

		if(s2c->result())
			LOG_HINT("%d",sock->Socket());
		else
			LOG_ERROR("%d",sock->Socket());
		
		return true;
	}
};//class ClientSession

class ClientThread:public Thread
{
	int index;
	ClientSession* cs;

public:
	
	ClientThread(const int index):Thread("ClientThread")
	{
		this->index = index;
		cs = new ClientSession();

		++cur_count;
	}

	void Run()
	{
		WaitTime(1000);
		while(true)
		{
			if(!cs->Send())
			{
				LOG_ERROR("send message error");
				break;
			}

			LOG_INFO("send success");
			
			if(!cs->Update(NowTime()))
			{
				LOG_ERROR("update error");
				break;
			}

			WaitTime(1000);
		}

		--cur_count;
	}
};


int main()
{
	InitNetwork();

	const int count =500;
	ClientThread* tharr[count];

	for(int i=0; i<count; ++i)
	{
		tharr[i]=new ClientThread(i);
		tharr[i]->Start();
	}

	while(cur_count>0)
		WaitTime(1000);

	CloseNetwork();
	return 0;
}