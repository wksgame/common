#include"Session.h"

namespace kiss
{
	Session::Session(const int sock, const sockaddr_in& address, const int buffSize)
	{
		this->sock = new TCPIOSocket(sock,address,buffSize);
		msgSize = 0;
		work_thread = nullptr;
	}

	Session::~Session()
	{
		delete sock;
		sock = nullptr;
	}

	bool Session::Update()
	{
		const int buffSize = 1024;
		char tempBuff[1024] = {};

		if(!sock->enable)
			return false;

		if(msgSize==0)
		{
			if(!sock->Read((char*)&msgSize, 4))
				return true;

			if(msgSize<4 || msgSize>1024)
				return false;
		}

		if(!sock->Read(tempBuff,msgSize))
			return true;

		auto result = ProcessMessage(tempBuff, msgSize);
		msgSize = 0;

		return result;
	}

	void Session::SetWorkThread(WorkThread* wt)
	{
		this->work_thread = wt;
	}

}//namespace kiss