#ifndef KISS_CONFIG_TABLE_H
#define KISS_CONFIG_TABLE_H

#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include<stddef.h>
#include<unordered_map>

namespace kiss
{
	template<typename T>
	class ConfigTable
	{
		/**
		* 数据类型枚举
		*/
		enum ValueType:int
		{
			unknow=0,
			bool_vt,
			int32_vt,
			int64_vt,
			float_vt,
			double_vt,
			char_vt,
		};

		struct FieldType
		{
			ValueType ftype;	//字段类型
			int count;			//如果是数组，数组长度，非数组为1
			int offset;			//在数据中的偏移量
			int len;			//数据大小
			
			FieldType()
			{
				ftype = unknow;
				count = 0;
				offset = 0;
				len = 0;
			}
		};
		
		struct convert_struct
		{
			std::string field_name;
			int user_struct_offset;
			int data_offset;
			int size;
		};
		
		std::unordered_map<std::string, convert_struct> convert;

	public:
		ConfigTable()
		{

		}
		
		void Map(const char* field_name,size_t offset)
		{
			convert[field_name].user_struct_offset = offset;
		}
		
		virtual void MapField()=0;

		bool Parse(const char* filename)
		{
			if(!filename)
				return false;
			
			const char* dot = strrchr(filename,'.');
			
			if(!dot)
				return false;

			if(strcmp(dot,".table")!=0)
				return false;

			std::ifstream file(filename,std::ios::binary);

			if(!file)
				return false;

			int row_count=0;
			int data_size=0;
			int field_count=0;
			file.read((char*)&row_count,sizeof(int));
			file.read((char*)&data_size,sizeof(int));
			file.read((char*)&field_count,sizeof(int));
			
			std::vector<FieldType> field_type(field_count);
			char field_name[128]={};
			
			for(int i=0; i<field_count; ++i)
			{
				FieldType field;
				int namelen = 0;
				file.read((char*)&namelen,sizeof(int));						// 写入字段名长度
				file.read(field_name,namelen);								// 写入字段名
				file.read((char*)&(field.ftype),sizeof(int));		// 写入字段类型
				file.read((char*)&(field.count),sizeof(int));		// 写入数据个数
				file.read((char*)&(field.offset),sizeof(int));		// 写入数据偏移量
				file.read((char*)&(field.len),sizeof(int));			// 写入本字段数据长度
				
				convert[field_name].data_offset = field.offset;
				convert[field_name].size = field.len;
			}
			
			char* databuff = new char[row_count*data_size];
			char** data = &databuff;
			file.read(databuff,row_count*data_size);
			
			MapField();
			
			for(int i=0; i<row_count; ++i)
			{
				T* s = new T();
				for(auto &it:convert)
					memcpy(((char*)&s)+it.second.user_struct_offset, data[i]+it.second.data_offset, it.second.size);
				
				CheckData(s);
			}

			file.close();
		}
protected:
    virtual void CheckData(T* t)=0;
	};//class ConfigTable
}//namespace kiss
#endif//KISS_CONFIG_TABLE_H