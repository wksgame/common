#include"ClientSession.h"
#include"msg.pb.h"
#include<iostream>
#include"other/RingBuffer.h"
#include<string>

using namespace std;

void PrintMessage(const char* str);

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

bool ClientSession::Update(const double cur_time)
{
	this->cur_time = cur_time;

	char buff[1024] = {};		///<��Ŵӻ��λ������ж�ȡ��������

	if (readBuff->readSize() > 0)
	{
		if (msgSize == 0 && !readBuff->read((char*)&msgSize, 4))
			return true;

		if (msgSize < 4 || msgSize>1024)	///<��Ϣ���ȴ���Ӧ�����ٰ�����Ϣ�ŵĳ���
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
	Login* c2s = (Login*)msg;

	string str = to_string(cur_time);
	str += " sock:";
	str += to_string(sock);
	str += " login\n";
	PrintMessage(str.c_str());
	return true;
}
