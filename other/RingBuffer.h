#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef unsigned long long int uint64;

class RingBuffer
{
	uint64 buffSize;			// 缓冲区大小
	uint64 readLeftSize;		// 剩余可读取大小
	uint64 writeLeftSize;		// 剩余可写入大小
	uint64 readPos;				// 当前读取位置
	uint64 writePos;			// 当前写入位置

	char* buff;					// 缓冲区
	bool isNew;					// 是否是内部new的缓冲区

public:

	RingBuffer(uint64 size);
	~RingBuffer();

	bool write(const char* b, const uint64 size);
	bool read(char* b, const uint64 size);

	uint64 readSize(){ return readLeftSize; }
};// class Buffer

#endif//RING_BUFFER_H