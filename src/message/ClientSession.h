#ifndef KISS_CLIENT_SESSION_H
#define KISS_CLIENT_SESSION_H

#include<message/ProtobufMessageProcess.h>
#include<message/ProtobufMessageSend.h>
#include<network/IOSocket.h>
#include<vector>
#include"Session.h"

namespace game
{
	struct RoleInfo;
	struct UserInfo;
}
namespace kiss
{
	class TCPIOSocket;
	class WorkThread;

	class ClientSession : public Session
	{
		ClientSession& operator= (const ClientSession& rhs)=delete;
		ClientSession(const ClientSession& rhs)=delete;

		kiss::pb::ProtobufMessageProcess<ClientSession> messageProcess;
		kiss::pb::ProtobufMessageSend<> messageSend;
		
//		unsigned int msgSize;		// current process message size

		double cur_time;			// current update time
		game::UserInfo* user_info;
		std::vector<game::RoleInfo*> role_info;
		int role_count;

	public:
//		bool Update();
		virtual bool ProcessMessage(const char* data, const int size);

	public:
//		TCPIOSocket* sock;
		WorkThread* work_thread;

	public:
		ClientSession(const int sock,const sockaddr_in& address,const int buffSize=32*1024);

		~ClientSession();
		
		bool Send();

		bool OnSignup(const google::protobuf::MessageLite* msg);
		bool OnLogin(const google::protobuf::MessageLite* msg);
		bool OnCreateRole(const google::protobuf::MessageLite* msg);
		bool OnSelectRole(const google::protobuf::MessageLite* msg);
	};//class ClientSession
}//namespace kiss
#endif//KISS_CLIENT_SESSION_H