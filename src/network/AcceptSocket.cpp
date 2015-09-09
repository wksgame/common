#include"AcceptSocket.h"
#include"other/RingBuffer.h"
#include<cstring>
#include<errno.h>

namespace kiss
{
	AcceptSocket::AcceptSocket(const int sock,const sockaddr_in& address,const int buffSize)
	{
		this->sockfd = sock;
		memcpy(&(this->address),&address,sizeof(address));

		readBuff = new RingBuffer(buffSize);
		writeBuff = new RingBuffer(buffSize);
	}
	
	AcceptSocket::~AcceptSocket()
	{
		delete readBuff;
		readBuff=nullptr;
		delete writeBuff;
		writeBuff=nullptr;
	}

	bool AcceptSocket::Recv()
	{
		const int buffSize = 1024*64;
		char tempBuff[buffSize] = {};

		auto recv_size = ::recv(sockfd, tempBuff, buffSize, 0);

		if(recv_size<=0)		// 调用时表示有数据可以接收，没收到就算出错了
			return(false);

			bool result = readBuff->write(tempBuff,recv_size);

		return result;
	}

	bool AcceptSocket::Send(const char* b, const uint64 size)
	{
		const int buffSize = 1024*16;
		char tempBuff[buffSize] = {};

			int sendSize = writeBuff->readSize();
			
			if(sendSize<=0)		// 缓冲区没有数据，直接走发送
			{
				int ss = ::send(sockfd, b, size, 0);
				
				if(ss == size)
				{
					return true;
				}
				
				if(ss == -1 && errno !=EAGAIN)
				{
					return false;
				}
			
				if(ss<0)
					ss=0;
				
				bool result = writeBuff->write(b+ss,size-ss);		// 未发送完的数据写入缓冲区
				
				return result;
			}
			else
			{
				if(!writeBuff->write(b,size))
				{
					return false;
				}
			}

			sendSize = writeBuff->readSize();

			while(sendSize>0)
			{
				sendSize = sendSize < buffSize ? sendSize : buffSize;

				writeBuff->peek(tempBuff,sendSize);

				auto _size = ::send(sockfd, tempBuff, sendSize, 0);

				if(_size<0)
				{
					
					if(errno != EAGAIN)
						return false;
					else
						return true;
				}

				if(_size==0)
					break;

				writeBuff->skip(_size);
				sendSize = writeBuff->readSize();
			}
		
		return true;
	}

	bool AcceptSocket::Read(char* b, const uint64 size)
	{
		return readBuff->read(b,size);
	}

	bool AcceptSocket::Write(const char* b, const uint64 size)
	{
		return writeBuff->write(b,size);
	}
	
	bool AcceptSocket::Peek(char* b, const uint64 size)
	{
		return readBuff->peek(b,size);
	}

	bool AcceptSocket::Skip(const uint64 size)
	{
		return readBuff->skip(size);
	}

}//namespace sgl