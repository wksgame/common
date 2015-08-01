#include"DataInputStream.h"
#include<string>

namespace kiss
{
	DataInputStream::DataInputStream()
	{
		is = nullptr;
	}

	DataInputStream::DataInputStream(InputStream* is):is(is)
	{
	}

	void DataInputStream::close()
	{
		if(is)
			is->close();
	}

	void DataInputStream::Set(InputStream* is)
	{
		this->is = is;
	}

	bool DataInputStream::read(void* data, const buff_t size)
	{
		if(is)
			return is->read(data, size);
		else
			return false;
	}

	bool DataInputStream::peek(void* data, const buff_t size)
	{
		if(is)
			return is->peek(data, size);
		else
			return false;
	}

	bool DataInputStream::skip(const buff_t size)
	{
		if(is)
			return is->skip(size);
		else
			return false;
	}

	bool DataInputStream::ReadBool(bool& b)
	{
		return read((char*)&b, sizeof(b));
	}

	bool DataInputStream::ReadInt16(int16& i)
	{
		return read((char*)&i, sizeof(i));
	}

	bool DataInputStream::ReadUint16(uint16& i)
	{
		return read((char*)&i, sizeof(i));
	}

	bool DataInputStream::ReadInt32(int32& i)
	{
		return read((char*)&i, sizeof(i));
	}

	bool DataInputStream::ReadUInt32(uint32& i)
	{
		return read((char*)&i, sizeof(i));
	}

	bool DataInputStream::ReadInt64(int64& i)
	{
		return read((char*)&i, sizeof(i));
	}

	bool DataInputStream::ReadUInt64(uint64& i)
	{
		return read((char*)&i, sizeof(i));
	}

	bool DataInputStream::ReadUnsignedLongInt(unsigned long int& i)
	{
		return read((char*)&i, sizeof(i));
	}

	bool DataInputStream::ReadString(std::string& s)
	{
		//return is->read();

		buff_t size=0;
		if(Read(size))
		{
			char* p = new char[size+1];
			p[size]='\0';
			if(read(p, size))
			{
				s=p;
				delete[] p;
				return true;
			}
			delete[] p;
		}

		return false;
	}

	bool DataInputStream::Read(bool& data)
	{
		return ReadBool(data);
	}

	bool DataInputStream::Read(int16& data)
	{
		return ReadInt16(data);
	}

	bool DataInputStream::Read(uint16& data)
	{
		return ReadUint16(data);
	}

	bool DataInputStream::Read(int32& data)
	{
		return ReadInt32(data);
	}

	bool DataInputStream::Read(uint32& data)
	{
		return ReadUInt32(data);
	}

	bool DataInputStream::Read(int64& data)
	{
		return ReadInt64(data);
	}

	bool DataInputStream::Read(uint64& data)
	{
		return ReadUInt64(data);
	}

	bool DataInputStream::Read(unsigned long int& data)
	{
		return ReadUnsignedLongInt(data);
	}

	bool DataInputStream::Read(std::string& data)
	{
		return ReadString(data);
	}
}