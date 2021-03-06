#include"SQLiteInterface.h"
#include<iostream>
#include<logger/logger.h>
#include<cstring>

using namespace std;

namespace kiss
{
	namespace db
	{
		SQLiteInterface::SQLiteInterface()
		{
			db=nullptr;
			stmt=nullptr;
		}

		SQLiteInterface::~SQLiteInterface()
		{
			Close();
		}

		bool SQLiteInterface::Open(const char* dbname)
		{
			if(sqlite3_open(dbname, &db) != SQLITE_OK)
			{
				sqlite3_close(db);
				db = nullptr;

				LOG_ERROR("can't open database:%s",sqlite3_errmsg(db));
				
				return false;
			}

			return true;
		}

		void SQLiteInterface::Close()
		{
			if(stmt)
				sqlite3_finalize(stmt);
			if(db)
				sqlite3_close(db);
		}

		bool SQLiteInterface::Prepare(const char* sqlstr)
		{
			auto rc = sqlite3_prepare(db, sqlstr, strlen(sqlstr), &stmt, nullptr);

			if(rc != SQLITE_OK)
			{
				sqlite3_finalize(stmt);
				stmt = nullptr;

				LOG_ERROR("can't sqlite3_prepare:%s",sqlite3_errmsg(db));
				return false;
			}

			return true;
		}
		
		bool SQLiteInterface::Prepare(const char* sqlstr, const int size)
		{
			auto rc = sqlite3_prepare(db, sqlstr, size, &stmt, nullptr);

			if(rc != SQLITE_OK)
			{
				sqlite3_finalize(stmt);
				stmt = nullptr;

				LOG_ERROR("can't sqlite3_prepare:%s",sqlite3_errmsg(db));
				return false;
			}

			return true;
		}

		bool SQLiteInterface::Step()
		{
			if(sqlite3_step(stmt) != SQLITE_DONE)
			{
				sqlite3_finalize(stmt);
				stmt = nullptr;

				LOG_ERROR("can't sqlite3_prepare:%s",sqlite3_errmsg(db));
				return false;
			}

			return true;
		}

		bool SQLiteInterface::CreateTable(const char* sqlstr)
		{
			if(!Prepare(sqlstr))
				return false;

			if(!Step())
				return false;

			sqlite3_finalize(stmt);
			stmt = nullptr;

			return true;
		}

		bool SQLiteInterface::DropTable(const char* tablename)
		{
			char sqlstr[1024]={0};
			int size = snprintf(sqlstr,1024,"drop table %s",tablename);

			if(!Prepare(sqlstr,size))
				return false;

			if(!Step())
				return false;

			sqlite3_finalize(stmt);
			stmt = nullptr;

			return true;
		}

		bool SQLiteInterface::Insert(const char* sqlstr)
		{
			char* msg = nullptr;
			auto rc = sqlite3_exec(db, sqlstr, nullptr, nullptr, &msg);
			return rc == SQLITE_OK;
		}

		long long SQLiteInterface::LastInsertRowID()
		{
			return sqlite3_last_insert_rowid(db);
		}

		bool SQLiteInterface::Update(const char* sqlstr)
		{
			return sqlite3_exec(db, sqlstr, nullptr, nullptr, nullptr) == SQLITE_OK;
		}

		bool SQLiteInterface::Select(const char* sqlstr, void* callback, void* arg)
		{
			return Select(sqlstr, (sqlite3_callback)callback, arg);
		}

		bool SQLiteInterface::Select(const char* sqlstr, sqlite3_callback callback, void* arg)
		{
			return sqlite3_exec(db, sqlstr, callback, arg, nullptr) == SQLITE_OK;
		}

		bool SQLiteInterface::Select(const char* sqlstr, db_callback callback, void* arg)
		{
			//char* errmsg=nullptr;
			char** result = nullptr;
			int nRow, nCol;
			int index=0;

			if(sqlite3_get_table(db, sqlstr, &result, &nRow, &nCol, nullptr) != SQLITE_OK)
			{
				if(result)
					sqlite3_free_table(result);

				return false;
			}

//			if(callback)
//				callback(result+nCol, nRow, nCol, arg);		// result+nCol  remove field name

			if(callback)
			{
				char** data = result+nCol;				//skip field name

				for(int i=0; i<nRow; ++i)
				{
					callback(data,nCol,arg);
					data += nCol;
				}
			}

			/*index = nCol;
			for(int i=0; i<nRow; ++i)
			{
				for(int j=0; j<nCol; ++j)
				{
					printf("%s %s\n", result[j], result[index]);
					++index;
				}
				printf("\n");
			}*/

			sqlite3_free_table(result);

			return true;
		}

		bool SQLiteInterface::SelectAllFromTable(const char* tablename, fun f)
		{
			char sqlstr[1024]={0};
			int size = snprintf(sqlstr,1024,"select * from %s",tablename);

			if(!Prepare(sqlstr,size))
				return false;

			while(true)
			{
				if(sqlite3_step(stmt) != SQLITE_ROW) //对于SELECT查询而言，如果有数据返回SQLITE_ROW，当到达结果集末尾时则返回SQLITE_DONE。
					break;

				if(f)
					f(stmt);
			}

			sqlite3_finalize(stmt);
			stmt = nullptr;

			return true;
		}

		bool SQLiteInterface::Insert()
		{
			const char* beginSQL = "BEGIN TRANSACTION";
			if(!Prepare(beginSQL,17))
				return false;

			if(!Step())
				return false;

			sqlite3_finalize(stmt);
			stmt = nullptr;

			const char* insertSQL = "INSERT INTO testname VALUES(?,?)";
			if(!Prepare(insertSQL,32))
				return false;

			int count = 10;
			for(int i=0; i<count; ++i)
			{
				sqlite3_bind_int(stmt, 1, i);
				sqlite3_bind_text(stmt, 2, "heh", 3, SQLITE_TRANSIENT);

				if(!Step())
				return false;

				sqlite3_reset(stmt);
			}

			sqlite3_finalize(stmt);
			stmt = nullptr;

			const char* commitSQL = "COMMIT";
			if(!Prepare(commitSQL))
				return false;

			if(!Step())
				return false;

			sqlite3_finalize(stmt);
			stmt = nullptr;

			return true;
		}
	}//namespace db
}//namespace kiss