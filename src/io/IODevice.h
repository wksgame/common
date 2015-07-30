#ifndef KISS_IO_DEVICE_H
#define KISS_IO_DEVICE_H

#include"InputStream.h"
#include"OutputStream.h"

namespace kiss
{
	class IODevice
	{
	protected:
		InputStream* is;
		OutputStream *os;

	public:
		IODevice();
		virtual ~IODevice();

		// read
		bool read(void* data, const buff_t size);
		bool peek(void* data, const buff_t size);
		bool skip(const buff_t size);

		// write
		bool write(const void* data, const buff_t size);
		buff_t cur_pos()const;

		// both
		virtual void close()=0;
	};//class IODevice

}//namespace kiss
#endif//KISS_IO_DEVICE_H