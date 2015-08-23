#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include<stdlib.h>

using namespace std;

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

/**
 * 数据类型对应的数据长度
 */
int ValueTypeSize[]=
{
	0,
	1,
	4,
	8,
	4,
	8,
	1,
};

/**
 * 数据类型对应的首字母，方便比较
 */
char ValueTypeString[]=
{
	'\0',
	'b',
	'i',
	'l',
	'f',
	'd',
	'c',
};

const int buffSize = 65536;		// 一行最大字符数量
char field_name_buff[buffSize];	// 字段名缓存
char field_type_buff[buffSize];	// 字段类型缓存
char row_data_buff[buffSize];		// 一行待解析的数据

/**
 * 字段类型
 */
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

void SplitString(char* str, vector<string>& result)
{
	char* substr;
	while(substr = strsep((char**)&str,"\t"))
	{
		result.push_back(substr);
	}
}

bool Parse(const char* filename)
{
	if(!filename)
		return false;
	
	char outname[128]={};
	strncpy(outname,filename,128);
	char* dot = strrchr(outname,'.');
	strcpy(dot+1,"table");

	std::ifstream ifile(filename);
	std::ofstream ofile(outname,std::ios::binary);
	
	if(!ifile)
	{
		printf("error:can't open file %s\n",filename);
		return false;
	}
	
	if(!ofile)
	{
		printf("error:can't open file %s\n",outname);
		return false;
	}

	std::vector<char*> headers;		// 存放指向每个字段名的指针
	std::vector<char*> types;			// 存放指向每个字段类型的指针
	std::vector<FieldType> field_type;	// 存放解析出来的字段类型
	std::vector<bool> skip;				// 存放需要跳过的字段，缺少字段名或者字段类型都会被跳过
	
	memset(field_name_buff,0,buffSize);
	if(!ifile.getline(field_name_buff,buffSize))	// 获取字段名
	{
		printf("error:file %s don't have field name\n",filename);
		return false;
	}

	memset(field_type_buff,0,buffSize);
	if(!ifile.getline(field_type_buff,buffSize))	// 获取字段类型
	{
		printf("error:file %s don't have field type\n",filename);
		return false;
	}
	
	char* p = strchr(field_name_buff,'\r');	// windows下带有\r字符需要去掉
	p?*p='\0':'\0';
	p = strchr(field_type_buff,'\r');	// windows下带有\r字符需要去掉
	p?*p='\0':'\0';
	
	char* substr;		// 拆分得到的字符串

	char* pname = field_name_buff;
	while(substr = strsep(&pname,"\t"))		// 拆分字段名
		headers.push_back(substr);
	
	char* ptype = field_type_buff;
	while(substr = strsep(&ptype,"\t"))		// 拆分字段类型
		types.push_back(substr);
	
	if(headers.size() != types.size())
	{
		printf("error:file %s field name count(%d) != field type count(%d)\n",filename,headers.size(),types.size());
		return false;
	}

	int field_count = headers.size();		// 读表读到的原始字段数量
	
	int struct_size = 0;					// 所有有效字段的字节总和
	int convert_field_count = 0;			// 实际转换的字段数量，去掉了无效字段
	for(int i=0; i<field_count; ++i)
	{
		FieldType ft;
		
		if(*headers[i] == '\0' || *types[i] == '\0')	// 字段名或者字段类型为空，不解析
		{
			skip.push_back(true);
			field_type.push_back(ft);
			
			continue;
		}
		
		switch(*types[i])
		{
			case 'b':ft.ftype = bool_vt;break;
			case 'i':ft.ftype = int32_vt;break;
			case 'l':ft.ftype = int64_vt;break;
			case 'f':ft.ftype = float_vt;break;
			case 'd':ft.ftype = double_vt;break;
			case 'c':ft.ftype = char_vt;break;
			default:ft.ftype = unknow;break;
		}
		
		if(ft.ftype == unknow)		// 字段类型未知，不解析
		{
			skip.push_back(true);
			field_type.push_back(ft);
			
			printf("warn:file %s unknow field type[%s]\n",filename,types[i]);
			
			continue;
		}
		
		char* p = strchr(types[i], '[');	// 检测是否是数组
		ft.count = p ? atoi(p+1) : 1;
		ft.offset = struct_size;
		ft.len = ft.count * ValueTypeSize[ft.ftype];
		
		skip.push_back(false);
		field_type.push_back(ft);
		
		struct_size += ft.len; // 计算数据大小
		++convert_field_count;
	}

	int row_index=2;	// 行号，包括字段名和字段类型，方便报错
	int convert_row_count = 0;	// 实际转换的行数

	ofile.write((char*)&convert_row_count,sizeof(int));		// 写入转换的数据行数，占位，最后修改
	ofile.write((char*)&struct_size,sizeof(int));			// 写入一条数据的长度
	ofile.write((char*)&convert_field_count,sizeof(int));	// 写入转换的字段数量
	
	for(int i=0; i<field_count; ++i)	// 写入 字段名、字段
	{
		if(skip[i])
			continue;
		
		int namelen = strlen(headers[i]);
		ofile.write((char*)&namelen,sizeof(int));						// 写入字段名长度
		ofile.write(headers[i],namelen);								// 写入字段名
		ofile.write((char*)&(field_type[i].ftype),sizeof(int));			// 写入字段类型
		ofile.write((char*)&(field_type[i].count),sizeof(int));			// 写入数据个数
		ofile.write((char*)&(field_type[i].offset),sizeof(int));		// 写入数据偏移量
		ofile.write((char*)&(field_type[i].len),sizeof(int));			// 写入本字段数据长度
	}
	
	while(true)
	{
		memset(row_data_buff,0,buffSize);

		if(!ifile.getline(row_data_buff,buffSize))
			break;
		
		++row_index;
		
		char* p = strchr(row_data_buff,'\r');	// windows下带有\r字符需要去掉
		p?*p='\0':'\0';
		
		if(row_data_buff[0]=='\0')	// 跳过空行
		{
			printf("warn:file %s row[%d] data null\n",filename,row_index);
			continue;
		}

		char* substr;
		int field_index=0;			// 字段索引
		char* pdata = row_data_buff;
		while(substr = strsep(&pdata,"\t"))		// 处理一行中的每个字段
		{
			if(skip[field_index])		// 跳过不处理的字段
			{
				++field_index;
				continue;
			}
			
			switch(field_type[field_index].ftype)		// 根据字段类型将字符串转换成对应类型值
			{
				case bool_vt:
				{
					bool value=false;
					if(field_type[field_index].count == 1)
					{
						value = *substr == '0';	// ‘0’为假 非'0'为真
						ofile.write((char*)&value,sizeof(value));
					}
					else
					{
						char* n;
						int nc=0;	// 解析出来的数组大小
						while(n = strsep(&substr,";"))		// 数组以分号隔开
						{
							value = *n == '0';
							ofile.write((char*)&value,sizeof(value));
							++nc;
							if(nc>=field_type[field_index].count)	// 超过字段类型中声明的字段大小，不解析剩余数据
								break;
						}
						
						value = false;
						int left = field_type[field_index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));		// 数据不足时，写入默认数据，保证结构大小一致，方便读取的时候直接进行偏移
					}
					
					break;
				}
				case int32_vt:
				{
					int value = 0;
					if(field_type[field_index].count == 1)
					{
						value = atoi(substr);
						ofile.write((char*)&value,sizeof(value));
					}
					else
					{
						char* n;
						int nc=0;
						while(n = strsep(&substr,";"))
						{
							value = atoi(n);
							ofile.write((char*)&value,sizeof(value));
							++nc;
							if(nc>=field_type[field_index].count)
								break;
						}
						
						value = 0;
						int left = field_type[field_index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));
					}
					
					break;
				}
				case int64_vt:
				{
					long long value = 0;
					if(field_type[field_index].count == 1)
					{
						value = atoll(substr);
						ofile.write((char*)&value,sizeof(value));
					}
					else
					{
						char* n;
						int nc=0;
						while(n = strsep(&substr,";"))
						{
							value = atoll(n);
							ofile.write((char*)&value,sizeof(value));
							++nc;
							if(nc>=field_type[field_index].count)
								break;
						}
						
						value = 0;
						int left = field_type[field_index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));
					}
					
					break;
				}
				case float_vt:
				{
					float value = 0;
					if(field_type[field_index].count == 1)
					{
						value = atof(substr);
						ofile.write((char*)&value,sizeof(value));
					}
					else
					{
						char* n;
						int nc=0;
						while(n = strsep(&substr,";"))
						{
							value = atof(n);
							ofile.write((char*)&value,sizeof(value));
							++nc;
							if(nc>=field_type[field_index].count)
								break;
						}
						
						value = 0;
						int left = field_type[field_index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));
					}
					
					break;
				}
				case double_vt:
				{
					double value = 0;
					if(field_type[field_index].count == 1)
					{
						value = atof(substr);
						ofile.write((char*)&value,sizeof(value));
					}
					else
					{
						char* n;
						int nc=0;
						while(n = strsep(&substr,";"))
						{
							value = atof(n);
							ofile.write((char*)&value,sizeof(value));
							++nc;
							if(nc>=field_type[field_index].count)
								break;
						}
						
						value = 0;
						int left = field_type[field_index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));
					}
					
					break;
				}
				case char_vt:
				{
					char value='\0';
					int nc = strlen(substr);
					ofile.write(substr,nc);
					
					int left = field_type[field_index].count - nc;
					while(left--)
						ofile.write((char*)&value,sizeof(value));
					
					break;
				}
			}
			
			++field_index;
		}
		
		if(field_index!=headers.size())	// 本行字段数量对不上
		{
			printf("error:file %s row[%d] data less\n",filename,row_index);
			break;
		}
		
		++convert_row_count;
	}
	
	ofile.seekp(0);		//跳到文件头重写数据行数
	ofile.write((char*)&convert_row_count, sizeof(int));

	ifile.close();
	ofile.close();
}
		
int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("%s file1 file2 ...\n",argv[0]);
		return 0;
	}
	
	int filecount = argc -1;
	
	for(int i=1; i<=filecount; ++i)
		Parse(argv[i]);
	
	return 0;
}