#include"ClientSession.h"
#include"msg.pb.h"
#include<iostream>
#include"other/RingBuffer.h"
#include<string>
#include<logger/logger.h>
#include<network/IOSocket.h>
#include<game/GameDB.h>
#include<game/Player.h>

using namespace std;

namespace kiss
{
	ClientSession::ClientSession(const int sock, const sockaddr_in& address, const int buffSize)
	{
		this->sock = new TCPIOSocket(sock,address,buffSize);

		msgSize = 0;
		cur_time = 0;
		user_info = nullptr;

		messageProcess.RegisterMessage(c2sSignup::id, &ClientSession::OnSignup, new c2sSignup());
		messageProcess.RegisterMessage(c2sLogin::id, &ClientSession::OnLogin, new c2sLogin());
		messageProcess.RegisterMessage(c2sCreateRole::id, &ClientSession::OnCreateRole, new c2sCreateRole());
		messageProcess.RegisterMessage(c2sSelectRole::id, &ClientSession::OnSelectRole, new c2sSelectRole());
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

	bool ClientSession::OnSignup(const google::protobuf::MessageLite* msg)
	{
		c2sSignup* c2s = (c2sSignup*)msg;
		
		bool result = CreateUser(c2s->account_name().c_str(),c2s->password().c_str());

		if(result)
			LOG_INFO("account_name %s password %s signup ok",c2s->account_name().c_str(),c2s->password().c_str());
		else
			LOG_ERROR("account_name %s signup failed",c2s->account_name().c_str());
		
		s2cSignup s2c;
		s2c.set_result(result);

		messageSend.Append(&s2c);
		
		return Send();
	}
	
	bool ClientSession::OnLogin(const google::protobuf::MessageLite* msg)
	{
		c2sLogin* c2s = (c2sLogin*)msg;

		user_info = GetUser(c2s->account_name().c_str(),c2s->password().c_str());
		
		if(user_info)
			LOG_INFO("account_name %s password %s login ok",c2s->account_name().c_str(),c2s->password().c_str());
		else
			LOG_ERROR("account_name %s login failed",c2s->account_name().c_str());
		
		s2cLogin s2c;
		s2c.set_result(user_info?true:false);
		
		if(user_info)
			GetRole(user_info->id,&role_info,role_count);

		messageSend.Append(&s2c);
		
		return Send();
	}
	
	bool ClientSession::OnCreateRole(const google::protobuf::MessageLite* msg)
	{
		c2sCreateRole* c2s = (c2sCreateRole*)msg;

		if(role_info.size()>=3)
			return false;

		game::RoleInfo* new_role = CreateRole(c2s->role_name().c_str(),user_info->id);
		
		if(new_role)
			LOG_INFO("account_name %s create role ok role name %s",user_info->username.c_str(),c2s->role_name().c_str());
		else
			LOG_ERROR("account_name %s create role failed role name %s",user_info->username.c_str(),c2s->role_name().c_str());
		
		role_info.push_back(new_role);
		
		s2cCreateRole s2c;
		s2c.set_result(new_role?true:false);

		messageSend.Append(&s2c);
		
		return Send();
	}
	
	bool ClientSession::OnSelectRole(const google::protobuf::MessageLite* msg)
	{
		c2sSelectRole* c2s = (c2sSelectRole*)msg;

		LOG_INFO("account_name %s role id %d login ok",user_info->username.c_str(),c2s->id);
		
		s2cSelectRole s2c;
		s2c.set_result(true);

		messageSend.Append(&s2c);
		
		return Send();
	}
}//namespace kiss