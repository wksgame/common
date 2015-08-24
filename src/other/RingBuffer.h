#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef unsigned long long int uint64;

namespace kiss
{
	class RingBuffer
	{
		uint64 buffSize;
		uint64 readLeftSize;
		uint64 writeLeftSize;
		uint64 readPos;
		uint64 writePos;

		char* buff;	

	public:

		RingBuffer(const uint64 size);
		~RingBuffer();

		bool write(const char* b, const uint64 size);
		bool read(char* b, const uint64 size);

		bool peek(char* b, const uint64 size);		// for read
		bool skip(const uint64 size);				// for read

		uint64 readSize(){ return readLeftSize; }	// can read size
		uint64 writeSize(){return writeLeftSize;}	// can write size
	};// class Buffer
}//namespace kiss
#endif//RING_BUFFER_H