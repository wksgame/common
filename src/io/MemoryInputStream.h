#ifndef KISS_MEMORY_INPUT_STREAM_H
#define KISS_MEMORY_INPUT_STREAM_H

#include"InputStream.h"

namespace kiss
{
	class MemoryInputStream : public InputStream
	{
		const char* buff;
		buff_t buffSize;
		buff_t curPos;

	public:
		MemoryInputStream(const char* b, const buff_t size);
		~MemoryInputStream(){};

		bool read(void* data, const buff_t size)override;
		bool peek(void* data, const buff_t size)override;
		bool skip(const buff_t size)override;
	};
}//namespace kiss

#endif//KISS_MEMORY_INPUT_STREAM_H