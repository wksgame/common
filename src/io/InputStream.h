#ifndef KISS_INPUT_STREAM_H
#define KISS_INPUT_STREAM_H

#include"defines.h"

namespace kiss
{
	class InputStream
	{
	public:
		virtual~InputStream(){};

		virtual bool read(void* data, const buff_t size)=0;
		virtual bool peek(void* data, const buff_t size)=0;
		virtual bool skip(const buff_t size)=0;

		virtual void close(){};
	};
}//namespace kiss

#endif//KISS_INPUT_STREAM_H