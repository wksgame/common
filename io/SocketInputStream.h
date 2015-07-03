#ifndef KISS_SOCKET_INPUT_STREAM_H
#define KISS_SOCKET_INPUT_STREAM_H

#include"InputStream.h"

namespace kiss
{
	class SocketInputStream : public InputStream
	{
		int sock;

	public:
		SocketInputStream(const int sock);
		~SocketInputStream();

		bool read(void* data, const buff_t size)override;
		bool peek(void* data, const buff_t size)override;
		bool skip(const buff_t size)override;

		void close()override;
	};
}//namespace kiss

#endif//KISS_SOCKET_INPUT_STREAM_H