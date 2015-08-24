#ifndef KISS_DATA_OUTPUT_STREAM_H
#define KISS_DATA_OUTPUT_STREAM_H

#include"OutputStream.h"
#include<string>
#include<list>

namespace kiss
{
	class DataOutputStream : public OutputStream
	{
		OutputStream* os;

	public:
		DataOutputStream(OutputStream* os);
		~DataOutputStream(){};

		bool operator!()const{return os?true:false;}

		bool write(const void* data, const buff_t size)override;
		buff_t cur_pos()const override;

		bool WriteBool(const bool& data);
		bool WriteInt16(const int16& data);
		bool WriteUint16(const uint16& data);
		bool WriteInt32(const int32& data);
		bool WriteUInt32(const uint32& data);
		bool WriteInt64(const int64& data);
		bool WriteUInt64(const uint64& data);
		bool WriteULong(const long unsigned int& data);
		bool WriteString(const std::string& s);
		bool WriteString(const char* s);

		bool Write(const bool& data);
		bool Write(const char& data);
		bool Write(const unsigned char& data);
		bool Write(const short int& data);
		bool Write(const unsigned short int& data);
		bool Write(const int& data);
		bool Write(const unsigned int& data);
		bool Write(const long long int& data);
		bool Write(const unsigned long long int& data);
		bool Write(const long int& data);
		bool Write(const unsigned long int& data);
		bool Write(const std::string& s);
		bool Write(const char* data);
		bool Write(char* const data);

		template<class T>
		bool Write(const T& data)
		{
			return data.Write(*this);	//bool Write(DataOutputStream& dos)const;
		}
		
		template<class T>
		bool Write(const T* data,const int size)
		{
			for(int i=0; i<size; ++i)
				if(!Write(data[i]))
					return false;
			return true;
		}

		template<typename T>
		bool Write(const std::list<T>& data)
		{
			uint32 size = data.size();
			if(!Write(size))
				return false;

			for(auto&i:data)
				if(!i.Write(*this))
					return false;

			return true;
		}

		template<class T>
		bool operator<<(const T& data)
		{
			return Write(data);
		}
	};
}//namespace kiss

#endif//KISS_DATA_OUTPUT_STREAM_H