#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include<stdlib.h>

using namespace std;

/**
 * 数据类型枚举
 */
enum ValueType
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

/**
 * 字段类型
 */
struct FieldType
{
	ValueType ftype;	//字段类型
	int count;			//如果是数组，数组长度
	
	FieldType()
	{
		ftype = unknow;
		count = 0;
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
		return false;
	
	if(!ofile)
		return false;

	const int buffSize = 4096;			// 一行的最大长度

	char header[buffSize]={};			// 未分割的字段名
	char type[buffSize]={};				// 未分割的字段类型

	std::vector<char*> headers;		// 存放指向每个字段名的指针
	std::vector<char*> types;			// 存放指向每个字段类型的指针
	std::vector<FieldType> field_type;	// 存放解析出来的字段类型
	std::vector<bool> skip;				// 存放需要跳过的字段，缺少字段名或者字段类型都会被跳过

	if(!ifile.getline(header,buffSize))	// 获取字段名
		return false;

	if(!ifile.getline(type,buffSize))	// 获取字段类型
		return false;
	
	char* substr;		// 拆分得到的字符串

	char* pheader = header;
	while(substr = strsep(&pheader,"\t"))	// 拆分字段名
		headers.push_back(substr);
	
	char* ptype = type;
	while(substr = strsep(&ptype,"\t"))		// 拆分字段类型
		types.push_back(substr);
	
	int struct_size = 0;		// 所有有效字段的字节总和
	for(int i=0; i<headers.size(); ++i)
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
			
			continue;
		}
		
		char* p = strchr(types[i], '[');	// 检测是否是数组
		ft.count = p ? atoi(p+1) : 1;
		
		skip.push_back(false);
		field_type.push_back(ft);
		
		struct_size += ft.count * ValueTypeSize[ft.ftype]; // 计算数据大小
	}

	char code[buffSize]={0};
	while(ifile.getline(code,buffSize))
	{
		char* p = strchr(code,'\r');	// windows下带有\r字符需要去掉
		p?*p='\0':'\0';

		char* substr;
		int index=0;			// 字段索引
		char* pcode = code;
		while(substr = strsep(&pcode,"\t"))		// 处理一行中的每个字段
		{
			if(skip[index])		// 跳过不处理的字段
			{
				++index;
				continue;
			}
			
			switch(field_type[index].ftype)		// 根据字段类型将字符串转换成对应字段
			{
				case bool_vt:
				{
					bool value=false;
					if(field_type[index].count == 1)
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
							if(nc>=field_type[index].count)	// 超过字段类型中声明的字段大小，不解析剩余数据
								break;
						}
						
						value = false;
						int left = field_type[index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));		// 数据不足时，写入默认数据，保证结构大小一致，方便读取的时候直接进行偏移
					}
					
					break;
				}
				case int32_vt:
				{
					int value = 0;
					if(field_type[index].count == 1)
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
							if(nc>=field_type[index].count)
								break;
						}
						
						value = 0;
						int left = field_type[index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));
					}
					
					break;
				}
				case int64_vt:
				{
					long long value = 0;
					if(field_type[index].count == 1)
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
							if(nc>=field_type[index].count)
								break;
						}
						
						value = 0;
						int left = field_type[index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));
					}
					
					break;
				}
				case float_vt:
				{
					float value = 0;
					if(field_type[index].count == 1)
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
							if(nc>=field_type[index].count)
								break;
						}
						
						value = 0;
						int left = field_type[index].count - nc;
						while(left--)
							ofile.write((char*)&value,sizeof(value));
					}
					
					break;
				}
				case double_vt:
				{
					double value = 0;
					if(field_type[index].count == 1)
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
							if(nc>=field_type[index].count)
								break;
						}
						
						value = 0;
						int left = field_type[index].count - nc;
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
					
					int left = field_type[index].count - nc;
					while(left--)
						ofile.write((char*)&value,sizeof(value));
					
					break;
				}
			}
			
			++index;
		}
	}

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