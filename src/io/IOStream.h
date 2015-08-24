#ifndef KISS_IO_DEVICE_H
#define KISS_IO_DEVICE_H

#include"InputStream.h"
#include"OutputStream.h"

namespace kiss
{
	class IOStream:public InputStream, public OutputStream
	{
	public:
		virtual ~IOStream();
		virtual void close()=0;
	};//class IOStream

}//namespace kiss
#endif//KISS_IO_DEVICE_H