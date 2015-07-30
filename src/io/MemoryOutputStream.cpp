#include"MemoryOutputStream.h"
#include<string>
#include<cstring>

namespace kiss
{
	MemoryOutputStream::MemoryOutputStream(char* b, const buff_t size)
	{
		buff = b;
		buffSize = size;
		curPos = 0;
	}

	bool MemoryOutputStream::write(const void* data, const buff_t size)
	{
		if(buffSize-curPos < size)
			return false;

		memcpy(buff+curPos, data, size);

		curPos += size;

		return true;
	}

	buff_t MemoryOutputStream::cur_pos()const
	{
		return curPos;
	}
}