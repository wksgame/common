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
		// 合成完整消息数据
		char arr[RECV_BUFF_SIZE]={0};
		MemoryOutputStream ready_mos(arr+4, RECV_BUFF_SIZE-4);	// 预留4字节写入消息长度
		DataOutputStream ready_dos(&ready_mos);

		ready_dos<<msgId;		// 填充消息ID
		if(!(ready_dos<<value))	// 实际数据
			return false;		

		int msgLength = ready_dos.cur_pos();	// 获取长度
		ready_dos<<msgLength;					// 写入长度
		memcpy(arr, arr+4+msgLength, 4);		// 拷贝到头部，不直接拷贝msgLength是为了避免大小端问题

		return io->write(arr, msgLength+4);
	}
};//struct ClientSession

#endif//CLIENT_SESSION_H