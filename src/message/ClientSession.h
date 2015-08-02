#ifndef KISS_CLIENT_SESSION_H
#define KISS_CLIENT_SESSION_H

#include<message/ProtobufMessageProcess.h>
#include<message/ProtobufMessageSend.h>
#include<network/IOSocket.h>

namespace kiss
{
	class SocketThread;
	class TCPIOSocket;

	class ClientSession// :public kiss::pb::ProtobufMessageProcess<ClientSession>
	{
		friend SocketThread;
		
		kiss::pb::ProtobufMessageProcess<ClientSession> messageProcess;
		kiss::pb::ProtobufMessageSend<> messageSend;
		
		TCPIOSocket* sock;
		
		int msgId;
		unsigned int msgSize;

		double cur_time;

		bool Update(const double cur_time);

	public:
		ClientSession(const int sock,const sockaddr_in& address,const int buffSize=32*1024);
		~ClientSession();
		
		bool Send();

		bool OnLogin(const google::protobuf::MessageLite* msg);
	};//class ClientSession
}//namespace kiss
#endif//KISS_CLIENT_SESSION_H