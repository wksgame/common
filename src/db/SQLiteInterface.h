#ifndef KISS_SQLITE_INTERFACE_H
#define KISS_SQLITE_INTERFACE_H

#include<sqlite/sqlite3.h>
#include"DBInterface.h"

typedef void (*fun)(sqlite3_stmt* stmt);
typedef void (*db_callback)(char** result, int nRow, int nCol, void* out);

namespace kiss
{
	namespace db
	{
		class SQLiteInterface:public DBInterface
		{
			sqlite3* db;
			sqlite3_stmt* stmt;

		public:

			SQLiteInterface();
			~SQLiteInterface();

			bool Open(const char* dbname)override;
			void Close()override;

			bool Prepare(const char* sqlstr);
			bool Prepare(const char* sqlstr, const int size);
			bool Step();		// select 不能用此函数

			bool CreateTable(const char* sqlstr)override;
			bool DropTable(const char* tablename)override;

			bool Insert(const char* sqlstr)override;
			long long LastInsertRowID();
			
			bool Update(const char* sqlstr)override;

			/**
			 * @param sqlstr sql string end of '\0'
			 * @param callback callback function
			 * @param arg argument to callback function
			 */
			bool Select(const char* sqlstr, void* callback, void* arg)override;

			/**
			 * @param sqlstr sql string end of '\0'
			 * @param callback callback function
			 * @param arg argument to callback function
			 */
			bool Select(const char* sqlstr, sqlite3_callback callback, void* arg);

			bool Select(const char* sqlstr, db_callback callback, void* arg);

			bool SelectAllFromTable(const char* tablename, fun f);

			bool Insert();//测试用
		};
	}
}
#endif//KISS_SQLITE_INTERFACE_H