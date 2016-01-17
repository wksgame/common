#ifndef KISS_CLIENT_SESSION_H
#define KISS_CLIENT_SESSION_H

#include<message/ProtobufMessageProcess.h>
#include<message/ProtobufMessageSend.h>
#include<vector>
#include"Session.h"

namespace game
{
	struct RoleInfo;
	struct UserInfo;
}
namespace kiss
{
	class WorkThread;

	class ClientSession : public Session
	{
		ClientSession& operator= (const ClientSession& rhs)=delete;
		ClientSession(const ClientSession& rhs)=delete;

		kiss::pb::ProtobufMessageProcess<ClientSession> messageProcess;
		kiss::pb::ProtobufMessageSend<> messageSend;

		double cur_time;			// current update time
		game::UserInfo* user_info;
		std::vector<game::RoleInfo*> role_info;
		int role_count;

	public:
		ClientSession(const int sock,const sockaddr_in& address,const int buffSize=32*1024);
		~ClientSession();

		bool ProcessMessage(const char* data, const int size)override;
		bool SendMessage(const char* data, const int size)override;
		bool SendMessage();

		bool OnSignup(const google::protobuf::MessageLite* msg);
		bool OnLogin(const google::protobuf::MessageLite* msg);
		bool OnCreateRole(const google::protobuf::MessageLite* msg);
		bool OnSelectRole(const google::protobuf::MessageLite* msg);
		bool OnAttackMonster(const google::protobuf::MessageLite* msg);
	};//class ClientSession
}//namespace kiss
#endif//KISS_CLIENT_SESSION_H