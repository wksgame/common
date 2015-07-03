#include"IODevice.h"

namespace kiss
{
	IODevice::IODevice()
	{
		is = nullptr;
		os = nullptr;
	}

	IODevice::~IODevice()
	{
	}

	bool IODevice::read(void* data, const buff_t size)
	{
		if(is)
			return is->read(data, size);
		else
			return false;
	}

	bool IODevice::peek(void* data, const buff_t size)
	{
		if(is)
			return is->peek(data, size);
		else
			return false;
	}

	bool IODevice::skip(const buff_t size)
	{
		if(is)
			return is->skip(size);
		else
			return false;
	}

	bool IODevice::write(const void* data, const buff_t size)
	{
		if(os)
			return os->write(data, size);
		else
			return false;
	}

	buff_t IODevice::cur_pos()const
	{
		if(os)
			return os->cur_pos();
		else
			return 0;
	}

}//namespace kiss