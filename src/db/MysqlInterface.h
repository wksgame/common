#ifndef KISS_MYSQL_INTERFACE_H
#define KISS_MYSQL_INTERFACE_H

#include"DBInterface.h"
#include<mysql/mysql.h>

namespace kiss
{
	namespace db 
	{
		class MysqlInterface : public DBInterface
		{
			MYSQL mysql;
			MYSQL_RES *res;
			MYSQL_ROW row;

		public:
			MysqlInterface();
			~MysqlInterface();

			bool Open(const char* host, const char* user, const char* passwd, const char* dbname, unsigned int port, const char* unix_socket, unsigned long clientflag);
			bool Open(const char* dbname)override;
			bool Open(const char* dbname, const char* user, const char* passwd, const char* host="127.0.0.1", unsigned int port=3306);
			void Close()override;

			bool CreateTable(const char* sqlstr)override;
			bool DropTable(const char* tablename)override;

			bool Insert(const char* sqlstr)override;
			bool Update(const char* sqlstr)override;

			bool Select(const char* sqlstr, db_callback callback, void* output)override;
		};//class MysqlInterface
	}

}//namespace kiss
#endif//KISS_MYSQL_INTERFACE_H