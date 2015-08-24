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
		DataInputStream(InputStream* is);
		~DataInputStream(){};
		
		bool operator!()const{return is?true:false;}

		bool read(void* data, const buff_t size)override;
		bool peek(void* data, const buff_t size)override;
		bool skip(const buff_t size)override;

		bool ReadBool(bool& data);
		bool ReadInt16(int16& i);
		bool ReadUint16(uint16& i);
		bool ReadInt32(int32& i);
		bool ReadUInt32(uint32& i);
		bool ReadInt64(int64& i);
		bool ReadUInt64(uint64& i);
		bool ReadUnsignedLongInt(unsigned long int& i);
		bool ReadString(std::string& s);

		bool Read(bool& data);
		bool Read(char& data);
		bool Read(unsigned char& data);
		bool Read(short int& data);
		bool Read(unsigned short int& data);
		bool Read(int& data);
		bool Read(unsigned int& data);
		bool Read(long long int& data);
		bool Read(unsigned long long int& data);
		bool Read(long int& data);
		bool Read(unsigned long int& i);
		bool Read(std::string& s);
		
		template<class T>
		bool Read(T& data)
		{
			return data.Read(*this);	//bool Read(DataInputStream& dos);
		}
		
		template<class T>
		bool Read(T* data,const int size)
		{
			for(int i=0; i<size; ++i)
				if(!Read(data[i]))
					return false;
			return true;
		}

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
	};//class DataInputStream
}//namespace kiss

#endif//KISS_DATA_INPUT_STREAM_H