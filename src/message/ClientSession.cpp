#include"ClientSession.h"
#include"msg.pb.h"
#include<iostream>
#include"other/RingBuffer.h"
#include<string>
#include<logger/logger.h>

using namespace std;

namespace kiss
{
	ClientSession::ClientSession(const int sock, const int buffSize)
	{
		this->sock = sock;
		readBuff = new RingBuffer(buffSize);
		writeBuff = new RingBuffer(buffSize);
		msgId = -1;
		msgSize = 0;

		cur_time = 0;

		RegisterMessage(1, &ClientSession::OnLogin, new Login());
	}

	ClientSession::~ClientSession()
	{
		delete readBuff;
		delete writeBuff;
		readBuff = nullptr;
		writeBuff = nullptr;
	}

	bool ClientSession::Update(const double cur_time)
	{
		this->cur_time = cur_time;

		char buff[1024] = {};		///<存放从环形缓冲区中读取到的数据

		if (readBuff->readSize() > 0)
		{
			if (msgSize == 0 && !readBuff->read((char*)&msgSize, 4))
				return true;

			if (msgSize < 4 || msgSize>1024)	///<消息长度错误，应该至少包含消息号的长度
				return false;

			if (readBuff->readSize() < msgSize)	///<消息还不完整，不处理
				return true;

			readBuff->read((char*)&msgId, 4);
			readBuff->read(buff, msgSize - 4);	///<扣除掉消息号的长度

			auto result = Process(msgId, buff, msgSize - 4);
			msgSize = 0;
			msgId = -1;
			return result;
		}

		return true;
	}

	bool ClientSession::OnLogin(const google::protobuf::MessageLite* msg)
	{
		Login* c2s = (Login*)msg;

		string str = to_string(cur_time);
		str += " sock:";
		str += to_string(sock);
		str += " login";
		LOG_INFO(str.c_str());
		return true;
	}
}//namespace kiss