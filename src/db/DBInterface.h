#ifndef KISS_DATABASE_INTERFACE_H
#define KISS_DATABASE_INTERFACE_H

typedef void (*db_callback)(char** result, int nCol, void* out);

namespace kiss
{
	namespace db
	{
		class DBInterface
		{
		public:
			virtual ~DBInterface(){};

			virtual bool Open(const char* dbname)=0;
			virtual void Close()=0;

			virtual bool CreateTable(const char* sqlstr)=0;
			virtual bool DropTable(const char* tablename)=0;

			virtual bool Insert(const char* sqlstr)=0;
			virtual bool Update(const char* sqlstr)=0;

			virtual bool Select(const char* sqlstr, db_callback callback, void* output)=0;
		};
	}
}
#endif//KISS_DATABASE_INTERFACE_H