#include"IOSocket.h"
#include<other/RingBuffer.h>
#include<cstring>

namespace kiss 
{
	TCPIOSocket::TCPIOSocket(const int buffSize)
	{
		readBuff = new RingBuffer(buffSize);
		writeBuff = new RingBuffer(buffSize);
	}

	TCPIOSocket::TCPIOSocket(const sock_t sock,const sockaddr_in& address,const int buffSize)
	{
		this->sock = sock;
		memcpy(&(this->address),&address,sizeof(address));

		readBuff = new RingBuffer(buffSize);
		writeBuff = new RingBuffer(buffSize);
	}
	
	bool TCPIOSocket::Recv()
	{
		const int buffSize = 32 * 1024;
		char tempBuff[buffSize] = {};

		auto result = ::recv(sock, tempBuff, buffSize, 0);
		
		if(result<=0)
			return(false);
		
		return readBuff->write(tempBuff,result);
	}

	bool TCPIOSocket::Send()
	{
		const int buffSize = 1024;
		char tempBuff[buffSize] = {};
		
		int sendSize = writeBuff->readSize();
		
		while(sendSize>0)
		{
			sendSize = sendSize < buffSize ? sendSize : buffSize;

			writeBuff->peek(tempBuff,sendSize);
			
			auto result = ::send(sock, tempBuff, sendSize, 0);
			
			if(result<0)
				return false;
			
			if(result==0)
				break;
			
			writeBuff->skip(result);
			sendSize = writeBuff->readSize();
		}
		
		return true;
	}

	bool TCPIOSocket::Read(char* b, const uint64 size)
	{
		return readBuff->read(b,size);
	}

	bool TCPIOSocket::Write(const char* b, const uint64 size)
	{
		return writeBuff->write(b,size);
	}
	
	bool TCPIOSocket::Peek(char* b, const uint64 size)
	{
		return readBuff->peek(b,size);
	}

	bool TCPIOSocket::Skip(const uint64 size)
	{
		return readBuff->skip(size);
	}

}//namespace kiss