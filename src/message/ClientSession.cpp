#include"ClientSession.h"
#include"msg.pb.h"
#include<iostream>
#include"other/RingBuffer.h"
#include<string>
#include<logger/logger.h>
#include<network/IOSocket.h>

using namespace std;

namespace kiss
{
	ClientSession::ClientSession(const int sock, const sockaddr_in& address, const int buffSize)
	{
		this->sock = new TCPIOSocket(sock,address,buffSize);

		msgSize = 0;
		cur_time = 0;

		messageProcess.RegisterMessage(c2sLogin::id, &ClientSession::OnLogin, new c2sLogin());
	}

	ClientSession::~ClientSession()
	{
		delete sock;
		sock = nullptr;
	}
	
	bool ClientSession::Send()
	{
		bool result = sock->Write(messageSend.buff, messageSend.curPos);
		messageSend.ClearData();
		
		if(result)
			LOG_INFO("send ok %d",sock->Socket());
		return result;
	}

	bool ClientSession::Update(const double cur_time)
	{
		this->cur_time = cur_time;

		const int buffSize = 1024;
		char tempBuff[1024] = {};
		
		if(!sock->enable)
			return false;

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

	bool ClientSession::OnLogin(const google::protobuf::MessageLite* msg)
	{
		c2sLogin* c2s = (c2sLogin*)msg;

		LOG_INFO("account_name %s password %s login ok",c2s->account_name().c_str(),c2s->password().c_str());
		
		s2cLogin s2c;
		s2c.set_result(true);

		messageSend.Append(&s2c);
		
		return Send();
	}
}//namespace kiss