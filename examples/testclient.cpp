#include<message/ProtobufMessageProcess.h>
#include<message/ProtobufMessageSend.h>
#include<network/ClientSocket.h>
#include<message/msg.pb.h>
#include<other/DateTime.h>
#include<logger/logger.h>
#include<thread/Thread.h>
#include<atomic>

using namespace kiss;
using namespace std;

std::atomic<int>	cur_count;
Logger logger("testclient.log");

class ClientSession
{
	kiss::pb::ProtobufMessageProcess<ClientSession> messageProcess;
	kiss::pb::ProtobufMessageSend<> messageSend;
	
	ClientSocket* sock;
	
	int msgId;
	unsigned int msgSize;
	long long role_id=0;

	double cur_time;

public:
	ClientSession():messageProcess(this)
	{
		messageProcess.RegisterMessage(s2cSignup::id, &ClientSession::OnSignup, new s2cSignup());
		messageProcess.RegisterMessage(s2cLogin::id, &ClientSession::OnLogin, new s2cLogin());
		messageProcess.RegisterMessage(s2cCreateRole::id, &ClientSession::OnCreateRole, new s2cCreateRole());
		messageProcess.RegisterMessage(s2cSelectRole::id, &ClientSession::OnSelectRole, new s2cSelectRole());
		messageProcess.RegisterMessage(s2cAttackMonster::id, &ClientSession::OnAttackMonster, new s2cAttackMonster());
		
		sock = new ClientSocket(1024);
		sock->Init("127.0.0.1",4000);
		
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
		
		auto result = messageProcess.Process(tempBuff, msgSize);
		msgSize = 0;
			
		return true;
	}
	
	bool SendCreateAccount()
	{
		char account_name[32];
		snprintf(account_name,32,"name%d",sock->GetSocketFD());
		
		c2sSignup cts;
		cts.set_account_name(account_name);
		cts.set_password("123456");
		
		messageSend.Append(&cts);

		if(!sock->Send(messageSend.buff,messageSend.curPos))
		{
			logger.error("buff full");
			return false;
		}

		messageSend.ClearData();

		return true;
	}

	bool SendLogin()
	{
		char account_name[32];
		snprintf(account_name,32,"name%d",sock->GetSocketFD());

		c2sLogin cts;
		cts.set_account_name(account_name);
		cts.set_password("123456");

		messageSend.Append(&cts);

		if(!sock->Send(messageSend.buff,messageSend.curPos))
		{
			logger.error("buff full");
			return false;
		}

		messageSend.ClearData();

		return true;
	}

	bool SendCreateRole()
	{
		char account_name[32];
		snprintf(account_name,32,"name%d",sock->GetSocketFD());

		c2sCreateRole cts;
		cts.set_role_name(account_name);

		messageSend.Append(&cts);

		if(!sock->Send(messageSend.buff,messageSend.curPos))
		{
			logger.error("buff full");
			return false;
		}

		messageSend.ClearData();

		return true;
	}

	bool SendSelectRole()
	{
		char account_name[32];
		snprintf(account_name,32,"name%d",sock->GetSocketFD());

		c2sSelectRole cts;
		cts.set_role_id(role_id);

		messageSend.Append(&cts);

		if(!sock->Send(messageSend.buff,messageSend.curPos))
		{
			logger.error("buff full");
			return false;
		}

		messageSend.ClearData();

		return true;
	}

	bool SendAttackMonster()
	{
		c2sAttackMonster cts;
		cts.set_monster_id(1);

		messageSend.Append(&cts);

		if(!sock->Send(messageSend.buff,messageSend.curPos))
		{
			logger.error("buff full");
			return false;
		}

		messageSend.ClearData();

		return true;
	}

	bool OnSignup(const google::protobuf::MessageLite* msg)
	{
		s2cLogin* s2c = (s2cLogin*)msg;

		if(s2c->result())
			logger.info("%d",sock->GetSocketFD());
		else
			logger.error("%d",sock->GetSocketFD());
		
		return true;
	}

	bool OnLogin(const google::protobuf::MessageLite* msg)
	{
		s2cLogin* s2c = (s2cLogin*)msg;

		if(s2c->result())
			logger.info("%d",sock->GetSocketFD());
		else
			logger.error("%d",sock->GetSocketFD());

		return true;
	}

	bool OnCreateRole(const google::protobuf::MessageLite* msg)
	{
		s2cLogin* s2c = (s2cLogin*)msg;

		if(s2c->result())
			logger.info("%d",sock->GetSocketFD());
		else
			logger.error("%d",sock->GetSocketFD());

		return true;
	}

	bool OnSelectRole(const google::protobuf::MessageLite* msg)
	{
		s2cLogin* s2c = (s2cLogin*)msg;

		if(s2c->result())
			logger.info("%d",sock->GetSocketFD());
		else
			logger.error("%d",sock->GetSocketFD());

		return true;
	}

	bool OnAttackMonster(const google::protobuf::MessageLite* msg)
	{
		s2cAttackMonster* s2c = (s2cAttackMonster*)msg;

		if(s2c->result())
			logger.info("%d",sock->GetSocketFD());
		else
			logger.error("%d",sock->GetSocketFD());

		return true;
	}
};//class ClientSession

class ClientThread:public Thread
{
	int index;
	ClientSession* cs;

public:
	
	ClientThread(const int index):Thread()
	{
		this->index = index;
		cs = new ClientSession();

		++cur_count;
	}

	void Run()override
	{
		while(true)
		{
			Update();
		}
	}

	void Update()
	{
		WaitTime(1000);

		if(!cs->SendCreateAccount())
		{
			logger.error("send SendCreateAccount error");
			return;
		}

		logger.info("send SendCreateAccount success");

		if(!cs->Update(NowTime()))
		{
			logger.error("update error");
			return;
		}

		WaitTime(1000);

		if(!cs->SendLogin())
		{
			logger.error("send SendLogin error");
			return;
		}

		logger.info("send SendLogin success");

		if(!cs->Update(NowTime()))
		{
			logger.error("update error");
			return;
		}

		WaitTime(1000);

		if(!cs->SendCreateRole())
		{
			logger.error("send SendCreateRole error");
			return;
		}

		logger.info("send SendCreateRole success");

		if(!cs->Update(NowTime()))
		{
			logger.error("update error");
			return;
		}

		WaitTime(1000);

		if(!cs->SendSelectRole())
		{
			logger.error("send SendSelectRole error");
			return;
		}

		logger.info("send SendSelectRole success");

		if(!cs->Update(NowTime()))
		{
			logger.error("update error");
			return;
		}

		WaitTime(1000);

		while(true)
		{
			if(!cs->SendAttackMonster())
			{
				logger.error("send SendSelectRole error");
				break;
			}

			logger.info("send SendSelectRole success");

			if(!cs->Update(NowTime()))
			{
				logger.error("update error");
				break;
			}

			WaitTime(1000);
		}

		--cur_count;
	}
};


int main()
{
	const int count =900;
	ClientThread* tharr[count];

	for(int i=0; i<count; ++i)
	{
		tharr[i]=new ClientThread(i);
		tharr[i]->Start();
	}

	while(cur_count>0)
		WaitTime(1000);

	return 0;
}