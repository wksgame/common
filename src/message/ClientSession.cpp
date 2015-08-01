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

		RegisterMessage(1, &ClientSession::OnLogin, new c2sLogin());
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

		char buff[1024] = {};		///<���Ŵӻ��λ������ж�ȡ��������

		if (readBuff->readSize() > 0)
		{
			if (msgSize == 0 && !readBuff->read((char*)&msgSize, 4))
				return true;

			if (msgSize < 4 || msgSize>1024)	///<��Ϣ���ȴ�����Ӧ�����ٰ�����Ϣ�ŵĳ���
				return false;

			if (readBuff->readSize() < msgSize)	///<��Ϣ����������������
				return true;

			readBuff->read((char*)&msgId, 4);
			readBuff->read(buff, msgSize - 4);	///<�۳�����Ϣ�ŵĳ���

			auto result = Process(msgId, buff, msgSize - 4);
			msgSize = 0;
			msgId = -1;
			return result;
		}

		return true;
	}

	bool ClientSession::OnLogin(const google::protobuf::MessageLite* msg)
	{
		c2sLogin* c2s = (c2sLogin*)msg;

		string str;
		str += "sock:";
		str += to_string(sock);
		str += " login";
		LOG_INFO(str.c_str());
		return true;
	}
}//namespace kiss