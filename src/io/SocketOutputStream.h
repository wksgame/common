#ifndef KISS_SOCKET_OUTPUT_STREAM_H
#define KISS_SOCKET_OUTPUT_STREAM_H

#include"OutputStream.h"

namespace kiss
{
	class SocketOutputStream : public OutputStream
	{
		int sock;

	public:
		SocketOutputStream(const int sock);
		~SocketOutputStream();

		bool write(const void* data, const buff_t size)override;
		buff_t cur_pos()const override;

		void close()override;
	};
}//namespace kiss

#endif//KISS_SOCKET_OUTPUT_STREAM_H