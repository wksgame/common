#ifndef KISS_OUTPUT_STREAM_H
#define KISS_OUTPUT_STREAM_H

#include"../defines.h"

namespace kiss
{
	class OutputStream
	{
	public:
		virtual~OutputStream(){};

		virtual bool write(const void* data, const buff_t size)=0;
		virtual buff_t cur_pos()const=0;

		virtual void close(){};
	};
}//namespace kiss

#endif//KISS_OUTPUT_STREAM_H