#ifndef KISS_MEMORY_OUTPUT_STREAM_H
#define KISS_MEMORY_OUTPUT_STREAM_H

#include"OutputStream.h"

namespace kiss
{
	class MemoryOutputStream : public OutputStream
	{
		char* buff;
		buff_t buffSize;
		buff_t curPos;

	public:
		MemoryOutputStream(char* b, const buff_t size);
		~MemoryOutputStream(){};

		bool write(const void* data, const buff_t size)override;
		buff_t cur_pos()const override;
	};
}//namespace kiss

#endif//KISS_MEMORY_OUTPUT_STREAM_H