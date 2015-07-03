#include"ClientSession.h"
#include<string>
#include"winsock.h"
#include<iostream>
#include"MessageFunc.h"

using namespace std;

ClientSession::ClientSession()
{
	player = nullptr;
	sock = 0;
	quit = false;
	userID = 0;

	io = nullptr;
}

ClientSession::~ClientSession()
{
	delete io;
}

void ClientSession::Init(const int sock)
{
	io = new kiss::SocketIODevice(sock);
	this->sock = sock;
}

bool ClientSession::Update()
{
	if(!UpdateMessage())
	{
		io->close();
		cout<<"客户端退出!"<<endl;
		return false;
	}

	return true;
}

bool ClientSession::UpdateMessage()
{
	MessageBuffer msgBuff;

	memset(msgBuff.recvBuff, 0, sizeof(msgBuff));

	// 接收消息长度
	if(!io->read(msgBuff.recvBuff, 4))
		return false;

	// 消息长度超过1000字节的直接踢掉
	if(msgBuff.msgLength >= RECV_BUFF_SIZE-4)
		return false;

	// 心跳
	if(msgBuff.msgLength==0)
		return true;

	// 错误数据，消息ID都不够
	if(msgBuff.msgLength < 4)
		return false;

	// 接收消息内容
	if(!io->read(msgBuff.recvBuff+4, msgBuff.msgLength))
		return false;

	// 处理消息
	if(!MessageFunc(*this, msgBuff.recvBuff))
		return false;
 
	return true;
}