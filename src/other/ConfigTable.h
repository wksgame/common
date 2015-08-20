#ifndef KISS_CONFIG_TABLE_H
#define KISS_CONFIG_TABLE_H

#include<fstream>
#include<vector>
#include<string>
#include<cstring>

namespace kiss
{
	template<typename T>
	class ConfigTable
	{
	public:
		ConfigTable()
		{

		}

		bool Parse(const char* filename)
		{
			if(!filename)
				return false;

			std::ifstream file(filename);

			if(!file)
				return false;

			const int buffSize = 4096;

			char header[buffSize]={};
			char type[buffSize]={};

			if(!file.getline(header,buffSize))
				return false;

			if(!file.getline(type,buffSize))
				return false;

			std::vector<std::string> rows;

			char code[1024]={0};
			while(file.getline(code,1024))
			{
				char* p = strchr(code,'\r');

				p?*p='\0':;

				rows.push_back(code);
			}

			if(rows.size()<2)
				return false;

			file.close();
		}
	};//class ConfigTable
}//namespace kiss
#endif//KISS_CONFIG_TABLE_H