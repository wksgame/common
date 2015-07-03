#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

//class Player;
#include"Player.h"
#include"../io/SocketInputStream.h"
#include"../io/DataInputStream.h"
#include"../io/SocketOutputStream.h"
#include"../io/DataOutputStream.h"
#include"../io/SocketIODevice.h"

#define RECV_BUFF_SIZE		10240

union MessageBuffer
{
	unsigned int msgLength;
	char recvBuff[RECV_BUFF_SIZE];
};

struct ClientSession
{
	Player* player;
	unsigned int sock;
	
	bool quit;
	long long userID;

	kiss::IODevice *io;

	ClientSession();
	~ClientSession();

	void Init(const int sock);

	bool Update();
	bool UpdateMessage();

	template<typename T>
	bool Send(int msgId, T& value)
	{
		// �ϳ�������Ϣ����
		char arr[RECV_BUFF_SIZE]={0};
		MemoryOutputStream ready_mos(arr+4, RECV_BUFF_SIZE-4);	// Ԥ��4�ֽ�д����Ϣ����
		DataOutputStream ready_dos(&ready_mos);

		ready_dos<<msgId;		// �����ϢID
		if(!(ready_dos<<value))	// ʵ������
			return false;		

		int msgLength = ready_dos.cur_pos();	// ��ȡ����
		ready_dos<<msgLength;					// д�볤��
		memcpy(arr, arr+4+msgLength, 4);		// ������ͷ������ֱ�ӿ���msgLength��Ϊ�˱����С������

		return io->write(arr, msgLength+4);
	}
};//struct ClientSession

#endif//CLIENT_SESSION_H