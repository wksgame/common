#include"DataInputStream.h"
#include<string>

namespace kiss
{
	DataInputStream::DataInputStream(InputStream* is):is(is)
	{
	}

	bool DataInputStream::read(void* data, const buff_t size)
	{
		return is->read(data, size);
	}

	bool DataInputStream::peek(void* data, const buff_t size)
	{
		return is->peek(data, size);
	}

	bool DataInputStream::skip(const buff_t size)
	{
		return is->skip(size);
	}

	bool DataInputStream::ReadBool(bool& data)
	{
		return Read(data);
	}

	bool DataInputStream::ReadInt16(int16& data)
	{
		return Read(data);
	}

	bool DataInputStream::ReadUint16(uint16& data)
	{
		return Read(data);
	}

	bool DataInputStream::ReadInt32(int32& data)
	{
		return Read(data);
	}

	bool DataInputStream::ReadUInt32(uint32& data)
	{
		return Read(data);
	}

	bool DataInputStream::ReadInt64(int64& data)
	{
		return Read(data);
	}

	bool DataInputStream::ReadUInt64(uint64& data)
	{
		return Read(data);
	}

	bool DataInputStream::ReadUnsignedLongInt(unsigned long int& data)
	{
		return Read(data);
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
		return read((char*)&data, sizeof(data));
	}
	
	bool DataInputStream::Read(char& data)
	{
		return read((char*)&data, sizeof(data));
	}
	
	bool DataInputStream::Read(unsigned char& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(short int& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(unsigned short int& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(int& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(unsigned int& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(long long int& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(unsigned long long int& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(long int& data)
	{
		return read((char*)&data, sizeof(data));
	}
	
	bool DataInputStream::Read(unsigned long int& data)
	{
		return read((char*)&data, sizeof(data));
	}

	bool DataInputStream::Read(std::string& data)
	{
		return ReadString(data);
	}
}