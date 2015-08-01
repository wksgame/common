#ifndef KISS_DATA_INPUT_STREAM_H
#define KISS_DATA_INPUT_STREAM_H

#include"InputStream.h"
#include<string>
#include<list>

namespace kiss
{
	class DataInputStream : public InputStream
	{
		InputStream* is;

	public:
		DataInputStream();
		DataInputStream(InputStream* is);
		~DataInputStream(){};

		void close()override;
		void Set(InputStream* is);
		void Delete();				// �ͷ�OutputStream���ڴ�

		bool read(void* data, const buff_t size)override;
		bool peek(void* data, const buff_t size)override;
		bool skip(const buff_t size)override;

		bool ReadBool(bool& b);
		bool ReadInt16(int16& i);
		bool ReadUint16(uint16& i);
		bool ReadInt32(int32& i);
		bool ReadUInt32(uint32& i);
		bool ReadInt64(int64& i);
		bool ReadUInt64(uint64& i);
		bool ReadUnsignedLongInt(unsigned long int& i);
		bool ReadString(std::string& s);

		template<class T>
		bool Read(T& data)
		{
			return data.Read(*this);	//�Զ���������Ҫʵ�� bool Read(DataInputStream& dos);
		}

		bool Read(bool& data);
		bool Read(int16& i);
		bool Read(uint16& i);
		bool Read(int32& i);
		bool Read(uint32& i);
		bool Read(int64& i);
		bool Read(uint64& i);
		bool Read(unsigned long int& i);
		bool Read(std::string& s);

		template<typename T>
		bool Read(std::list<T>& data)
		{
			auto count = data.size();

			if(!Read(count))
				return false;

			for(decltype(count) i=0; i<count; ++i)
			{
				T d;
				if(!d.Read(*this))
					return false;

				data.push_back(d);
			}

			return true;
		}

		template<class T>
		bool operator>>(T& data)
		{
			return Read(data);
		}
	};
}//namespace kiss

#endif//KISS_DATA_INPUT_STREAM_H