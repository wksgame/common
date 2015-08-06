#include"MysqlInterface.h"
#include<cstring>
#include<stdio.h>

namespace kiss
{
	namespace db 
	{
		MysqlInterface::MysqlInterface()
		{
			mysql_init(&mysql);
		}
		
		MysqlInterface::~MysqlInterface()
		{
			mysql_close(&mysql);
		}
		
		bool MysqlInterface::Open(const char* host, const char* user, const char* passwd, const char* dbname, unsigned int port, const char* unix_socket, long unsigned int clientflag)
		{
			char nvalue = 1;
			mysql_options(&mysql, MYSQL_OPT_RECONNECT,(char*)&nvalue);		// reconnect
			//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
			
			if(!mysql_real_connect(&mysql,host,user,passwd,dbname,port,unix_socket,clientflag))
				return false;
			return true;
		}

		bool MysqlInterface::Open(const char* dbname)
		{
			return Open("127.0.0.1","root","",dbname,3306,0,0);
		}

		void MysqlInterface::Close()
		{

		}

		bool MysqlInterface::CreateTable(const char* sqlstr)
		{
			if(mysql_query(&mysql, sqlstr)!=0)
				return false;
			auto count = mysql_affected_rows(&mysql);
			return true;
		}

		bool MysqlInterface::DropTable(const char* tablename)
		{
			char sqlstr[1024]={0};
			int size = snprintf(sqlstr,1024,"drop table %s",tablename);
			
			if(mysql_query(&mysql, sqlstr)!=0)
				return false;
			auto count = mysql_affected_rows(&mysql);
			return true;
		}

		bool MysqlInterface::Insert(const char* sqlstr)
		{

		}

		bool MysqlInterface::Update(const char* sqlstr)
		{

		}


		bool MysqlInterface::Select(const char* sqlstr, void* callback, void* arg)
		{
			if(mysql_real_query(&mysql, sqlstr, strlen(sqlstr)))
				return false;
			
			res = mysql_store_result(&mysql);
			
			if(res==nullptr)
				return false;
			
			while(row = mysql_fetch_row(res))
			{
			}
			
			mysql_free_result(res);

			return true;
		}


	}//namespace db

}//namespace kiss