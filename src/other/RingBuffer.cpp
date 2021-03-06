#include"RingBuffer.h"
#include<string.h>

namespace kiss
{
	RingBuffer::RingBuffer(const uint64 size)
	{
		buffSize = size;
		readLeftSize = 0;
		writeLeftSize = buffSize;
		readPos = 0;
		writePos = 0;
		buff = new char[buffSize];
	}

	RingBuffer::~RingBuffer()
	{
		delete[] buff;
		buff = nullptr;
	}

	bool RingBuffer::write(const char* b, const uint64 size)
	{
		if (writeLeftSize < size)
			return false;

		if (writePos >= readPos)
		{
			uint64 left = buffSize - writePos;
			if (left >= size)
			{
				memcpy(buff + writePos, b, size);
				writePos += size;
			}
			else
			{
				memcpy(buff + writePos, b, left);
				memcpy(buff, b + left, size - left);
				writePos = size - left;
			}
		}
		else
		{
			memcpy(buff + writePos, b, size);
			writePos += size;
		}

		writeLeftSize -= size;
		readLeftSize += size;

		return true;
	}

	bool RingBuffer::read(char* b, const uint64 size)
	{
		if (readLeftSize < size)
			return false;

		if (readPos >= writePos)
		{
			uint64 left = buffSize - readPos;
			if (left >= size)
			{
				memcpy(b, buff + readPos, size);
				readPos += size;
			}
			else
			{
				memcpy(b, buff + readPos, left);
				memcpy(b + left, buff, size - left);
				readPos = size - left;
			}
		}
		else
		{
			memcpy(b, buff + readPos, size);
			readPos += size;
		}

		readLeftSize -= size;
		writeLeftSize += size;

		return true;
	}
	
	bool RingBuffer::peek(char* b, const uint64 size)
	{
		if (readLeftSize < size)
			return false;

		auto _readPos = readPos;

		if (_readPos >= writePos)
		{
			uint64 left = buffSize - _readPos;
			if (left >= size)
			{
				memcpy(b, buff + _readPos, size);
				_readPos += size;
			}
			else
			{
				memcpy(b, buff + _readPos, left);
				memcpy(b + left, buff, size - left);
				_readPos = size - left;
			}
		}
		else
		{
			memcpy(b, buff + _readPos, size);
			_readPos += size;
		}
		
		return true;
	}

	bool RingBuffer::skip(const uint64 size)
	{
		if(readLeftSize < size)
			return false;
		
		readLeftSize -= size;
		writeLeftSize += size;
		
		return true;
	}

}//namespace kiss