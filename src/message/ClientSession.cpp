#include"ClientSession.h"
#include"msg.pb.h"
#include<iostream>
#include"other/RingBuffer.h"
#include<string>
#include<logger/logger.h>
#include<network/ClientSocket.h>
#include<game/GameDB.h>
#include<game/Player.h>
#include<thread/WorkThread.h>

using namespace std;

namespace kiss
{
	ClientSession::ClientSession(AcceptSocket* as):Session(as),messageProcess(this)
	{
		this;
		cur_time = 0;
		user_info = nullptr;

		messageProcess.RegisterMessage(c2sSignup::id, &ClientSession::OnSignup, new c2sSignup());
		messageProcess.RegisterMessage(c2sLogin::id, &ClientSession::OnLogin, new c2sLogin());
		messageProcess.RegisterMessage(c2sCreateRole::id, &ClientSession::OnCreateRole, new c2sCreateRole());
		messageProcess.RegisterMessage(c2sSelectRole::id, &ClientSession::OnSelectRole, new c2sSelectRole());
		messageProcess.RegisterMessage(c2sAttackMonster::id, &ClientSession::OnAttackMonster, new c2sAttackMonster());
	}

	ClientSession::~ClientSession()
	{
	}
	


	bool ClientSession::ProcessMessage(const char* data, const int size)
	{
		return messageProcess.Process(data, size);
	}

	bool ClientSession::SendMessage(const char* data, const int size)
	{
		return sock->Send(data, size);
	}

	bool ClientSession::SendMessage()
	{
		bool result = SendMessage(messageSend.buff, messageSend.curPos);
		messageSend.ClearData();

		return result;
	}

	bool ClientSession::OnSignup(const google::protobuf::MessageLite* msg)
	{
		c2sSignup* c2s = (c2sSignup*)msg;
		s2cSignup s2c;

		ACCOUNT_ID newaccountid = work_thread->db->CreateAccount(c2s->account_name().c_str(),c2s->password().c_str());

		if(newaccountid!=-1)
		{
			s2c.set_result(true);
			syslogger.info("account_name %s password %s signup ok",c2s->account_name().c_str(),c2s->password().c_str());
		}
		else
		{
			s2c.set_result(false);
			syslogger.error("account_name %s signup failed",c2s->account_name().c_str());
		}
		
		messageSend.Append(&s2c);
		
		return SendMessage();
	}
	
	bool ClientSession::OnLogin(const google::protobuf::MessageLite* msg)
	{
		c2sLogin* c2s = (c2sLogin*)msg;

		user_info = work_thread->db->CheckAccount(c2s->account_name().c_str(),c2s->password().c_str());
		
		if(user_info)
			syslogger.info("account_name %s password %s login ok",c2s->account_name().c_str(),c2s->password().c_str());
		else
			syslogger.error("account_name %s login failed",c2s->account_name().c_str());
		
		s2cLogin s2c;
		s2c.set_result(user_info?true:false);
		
		if(user_info)
		{
			work_thread->db->GetRoleList(user_info->id,&role_info);
			for(auto i:role_info)
			{
				auto role= s2c.add_rolelist();
				role->set_role_id(i->roleID);
				role->set_role_name(i->rolename);
				role->set_level(i->level);
			}
		}

		messageSend.Append(&s2c);
		
		return SendMessage();
	}
	
	bool ClientSession::OnCreateRole(const google::protobuf::MessageLite* msg)
	{
		c2sCreateRole* c2s = (c2sCreateRole*)msg;

		if(role_info.size()>=3)
			return false;

		if(!user_info)
			return false;

		game::RoleInfo* new_role = work_thread->db->CreateRole(c2s->role_name().c_str(),user_info->id);
		
		if(new_role)
			syslogger.info("account_name %s create role ok role name %s",user_info->username.c_str(),c2s->role_name().c_str());
		else
			syslogger.error("account_name %s create role failed role name %s",user_info->username.c_str(),c2s->role_name().c_str());
		
		role_info.push_back(new_role);
		
		s2cCreateRole s2c;
		s2c.set_result(new_role?true:false);

		messageSend.Append(&s2c);
		
		return SendMessage();
	}
	
	bool ClientSession::OnSelectRole(const google::protobuf::MessageLite* msg)
	{
		c2sSelectRole* c2s = (c2sSelectRole*)msg;

		syslogger.info("account_name %s role id %d login ok",user_info->username.c_str(),c2s->id);
		
		s2cSelectRole s2c;
		s2c.set_result(true);

		messageSend.Append(&s2c);
		
		return SendMessage();
	}

	bool ClientSession::OnAttackMonster(const google::protobuf::MessageLite* msg)
	{
		c2sAttackMonster* c2s = (c2sAttackMonster*)msg;

		syslogger.info("%s attack %d",user_info->username.c_str(),c2s->monster_id());

		s2cAttackMonster s2c;
		s2c.set_result(true);

		messageSend.Append(&s2c);

		return SendMessage();
	}

}//namespace kiss