#ifndef KISS_CLIENT_SESSION_H
#define KISS_CLIENT_SESSION_H

#include<message/ProtobufMessageProcess.h>

class SocketThread;
class RingBuffer;

class ClientSession :public kiss::pb::ProtobufMessageProcess<ClientSession>
{
	friend SocketThread;

	RingBuffer* readBuff;
	RingBuffer* writeBuff;
	int sock;
	int msgId;
	unsigned int msgSize;

	double cur_time;

	bool Update(const double cur_time);

public:
	ClientSession(const int sock,const int buffSize=32*1024);

	bool OnLogin(const google::protobuf::MessageLite* msg);
};

#endif//KISS_CLIENT_SESSION_H