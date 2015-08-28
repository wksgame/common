#include"IOSocket.h"
#include<other/RingBuffer.h>
#include<cstring>

namespace kiss 
{
	TCPIOSocket::TCPIOSocket(const int buffSize)
	{
		readBuff = new RingBuffer(buffSize);
		writeBuff = new RingBuffer(buffSize);
		enable = true;
	}

	TCPIOSocket::TCPIOSocket(const sock_t sock,const sockaddr_in& address,const int buffSize)
	{
		this->sock = sock;
		memcpy(&(this->address),&address,sizeof(address));

		readBuff = new RingBuffer(buffSize);
		writeBuff = new RingBuffer(buffSize);
		enable = true;
	}
	
	TCPIOSocket::~TCPIOSocket()
	{
		SAFE_DELETE(readBuff);
		SAFE_DELETE(writeBuff);
	}

	bool TCPIOSocket::Recv()
	{
		const int buffSize = 32 * 1024;
		char tempBuff[buffSize] = {};

		auto recv_size = ::recv(sock, tempBuff, buffSize, 0);

		if(recv_size<=0)
			return(false);

		read_mutex.lock();
			bool result = readBuff->write(tempBuff,recv_size);
		read_mutex.unlock();

		return result;
	}

	bool TCPIOSocket::Send()
	{
		const int buffSize = 1024;
		char tempBuff[buffSize] = {};

		write_mutex.lock();
			int sendSize = writeBuff->readSize();

			while(sendSize>0)
			{
				sendSize = sendSize < buffSize ? sendSize : buffSize;

				writeBuff->peek(tempBuff,sendSize);

				auto _size = ::send(sock, tempBuff, sendSize, 0);

				if(_size<0)
				{
					write_mutex.unlock();
					
					return false;
				}

				if(_size==0)
					break;

				writeBuff->skip(_size);
				sendSize = writeBuff->readSize();
			}
		write_mutex.unlock();
		
		return true;
	}

	bool TCPIOSocket::Read(char* b, const uint64 size)
	{
		bool result = false;

		if(read_mutex.try_lock())
		{
			result = readBuff->read(b,size);
			read_mutex.unlock();
		}

		return result;
	}

	bool TCPIOSocket::Write(const char* b, const uint64 size)
	{
		write_mutex.lock();
			bool result = writeBuff->write(b,size);
		write_mutex.unlock();

		return result;
	}
	
	bool TCPIOSocket::Peek(char* b, const uint64 size)
	{
		read_mutex.lock();
			bool result = readBuff->peek(b,size);
		read_mutex.unlock();

		return result;
	}

	bool TCPIOSocket::Skip(const uint64 size)
	{
		read_mutex.lock();
			bool result = readBuff->skip(size);
		read_mutex.unlock();

		return result;
	}

}//namespace kiss