#include"MemoryInputStream.h"
#include<string>

namespace kiss
{
	MemoryInputStream::MemoryInputStream(const char* b, const buff_t size)
	{
		buff = b;
		buffSize = size;
		curPos = 0;
	}

	bool MemoryInputStream::read(void* data, const buff_t size)
	{
		if(!peek(data, size))
			return false;

		curPos += size;

		return true;
	}

	bool MemoryInputStream::peek(void* data, const buff_t size)
	{
		if(buffSize-curPos < size)
			return false;

		memcpy(data, buff+curPos, size);

		return true;
	}

	bool MemoryInputStream::skip(const buff_t size)
	{
		if(buffSize-curPos < size)
			return false;

		curPos += size;

		return true;
	}
}