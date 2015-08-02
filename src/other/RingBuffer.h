#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef unsigned long long int uint64;

namespace kiss
{
	class RingBuffer
	{
		uint64 buffSize;			// ��������С
		uint64 readLeftSize;		// ʣ���ɶ�ȡ��С
		uint64 writeLeftSize;		// ʣ����д����С
		uint64 readPos;				// ��ǰ��ȡλ��
		uint64 writePos;			// ��ǰд��λ��

		char* buff;					// ������
		bool isNew;					// �Ƿ����ڲ�new�Ļ�����

	public:

		RingBuffer(uint64 size);
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