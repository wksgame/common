#include"DataOutputStream.h"
#include<string>
#include<cstring>

namespace kiss
{
	DataOutputStream::DataOutputStream(OutputStream* os):os(os)
	{
	}

	bool DataOutputStream::write(const void* data, const buff_t size)
	{
		return os->write(data, size);
	}

	buff_t DataOutputStream::cur_pos()const
	{
		return os->cur_pos();
	}

	bool DataOutputStream::WriteBool(const bool& data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteInt16(const int16& data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteUint16(const uint16& data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteInt32(const int32& data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteUInt32(const uint32& data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteInt64(const int64& data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteUInt64(const uint64& data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteULong(const unsigned long int &data)
	{
		return Write(data);
	}

	bool DataOutputStream::WriteString(const std::string& s)
	{
		buff_t size=s.size();

		if(Write(size))
			if(write(s.c_str(), size))
				return true;

		return false;
	}

	bool DataOutputStream::WriteString(const char* s)
	{
		buff_t size = strlen(s);

		if(Write(size))
			if(write(s, size))
				return true;

		return false;
	}

	bool DataOutputStream::Write(const bool& data)
	{
		return write((char*)&data, sizeof(data));
	}
	
	bool DataOutputStream::Write(const char& data)
	{
		return write((char*)&data, sizeof(data));
	}
	
	bool DataOutputStream::Write(const unsigned char& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const short int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const short unsigned int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const unsigned int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const long long int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const long long unsigned int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const long int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const unsigned long int& data)
	{
		return write((char*)&data, sizeof(data));
	}

	bool DataOutputStream::Write(const std::string& data)
	{
		return WriteString(data);
	}

	bool DataOutputStream::Write(const char* data)
	{
		return WriteString(data);
	}

	bool DataOutputStream::Write(char* const data)
	{
		return WriteString(data);
	}
}