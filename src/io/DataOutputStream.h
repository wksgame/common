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
		DataOutputStream();
		DataOutputStream(OutputStream* os);
		~DataOutputStream(){};

		void close()override;
		void Set(OutputStream* os);
		void Delete();				// �ͷ�OutputStream���ڴ�

		bool write(const void* data, const buff_t size)override;
		buff_t cur_pos()const override;

		bool WriteBool(const bool& b);
		bool WriteInt16(const int16& i);
		bool WriteUint16(const uint16& i);
		bool WriteInt32(const int32& i);
		bool WriteUInt32(const uint32& i);
		bool WriteInt64(const int64& i);
		bool WriteUInt64(const uint64& i);
		bool WriteUnsignedLongInt(const unsigned long int& i);
		bool WriteString(const std::string& s);
		bool WriteString(const char* s);

		template<class T>
		bool Write(const T& data)
		{
			return data.Write(*this);	//�Զ���������Ҫʵ�� bool Write(DataOutputStream& dos)const;
		}

		bool Write(const bool& data);
		bool Write(const int16& i);
		bool Write(const uint16& i);
		bool Write(const int32& i);
		bool Write(const uint32& i);
		bool Write(const int64& i);
		bool Write(const uint64& i);
		bool Write(const unsigned long int& i);
		bool Write(const std::string& s);
		bool Write(const char* data);
		bool Write(char* const data);

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