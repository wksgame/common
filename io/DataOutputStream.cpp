#include"DataOutputStream.h"
#include<string>

namespace kiss
{
	DataOutputStream::DataOutputStream()
	{
		os = nullptr;
	}

	DataOutputStream::DataOutputStream(OutputStream* os):os(os)
	{
	}

	void DataOutputStream::close()
	{
		if(os)
			os->close();
	}

	void DataOutputStream::Set(OutputStream* os)
	{
		this->os = os;
	}

	void DataOutputStream::Delete()
	{
		delete os;
		os = nullptr;
	}

	bool DataOutputStream::write(const void* data, const buff_t size)
	{
		if(os)
			return os->write(data, size);
		else
			return false;
	}

	buff_t DataOutputStream::cur_pos()const
	{
		if(os)
			return os->cur_pos();
		else
			return 0;
	}

	bool DataOutputStream::WriteBool(const bool& b)
	{
		return write((char*)&b, sizeof(b));
	}

	bool DataOutputStream::WriteInt16(const int16& i)
	{
		return write((char*)&i, sizeof(i));
	}

	bool DataOutputStream::WriteUint16(const uint16& i)
	{
		return write((char*)&i, sizeof(i));
	}

	bool DataOutputStream::WriteInt32(const int32& i)
	{
		return write((char*)&i, sizeof(i));
	}

	bool DataOutputStream::WriteUInt32(const uint32& i)
	{
		return write((char*)&i, sizeof(i));
	}

	bool DataOutputStream::WriteInt64(const int64& i)
	{
		return write((char*)&i, sizeof(i));
	}

	bool DataOutputStream::WriteUInt64(const uint64& i)
	{
		return write((char*)&i, sizeof(i));
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
		buff_t size = std::strlen(s);

		if(Write(size))
			if(write(s, size))
				return true;

		return false;
	}

	bool DataOutputStream::Write(const bool& data)
	{
		return WriteBool(data);
	}

	bool DataOutputStream::Write(const int16& data)
	{
		return WriteInt16(data);
	}

	bool DataOutputStream::Write(const uint16& data)
	{
		return WriteUint16(data);
	}

	bool DataOutputStream::Write(const int32& data)
	{
		return WriteInt32(data);
	}

	bool DataOutputStream::Write(const uint32& data)
	{
		return WriteUInt32(data);
	}

	bool DataOutputStream::Write(const int64& data)
	{
		return WriteInt64(data);
	}

	bool DataOutputStream::Write(const uint64& data)
	{
		return WriteUInt64(data);
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