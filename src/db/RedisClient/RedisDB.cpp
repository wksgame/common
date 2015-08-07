#include"RedisDB.h"
#include"hiredis/hiredis.h"
#include"RedisDBReply.h"
#include<string.h>
#include<stdlib.h>

namespace hgl
{
	namespace redis
	{
		RedisDB::RedisDB(redisContext *rc)
		{
			con=rc;
		}

		RedisDB::~RedisDB()
		{
			if(con)
				redisFree(con);
		}

		bool RedisDB::Info(char *result)
		{
			REPLY r(con,"INFO");

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

			strcpy(result,r->str);
			return(true);
		}

		/**
		* 选择一个数据库
		* @param db_index 数据库索引,从0开始
		*/
		bool RedisDB::SelectDB(int db_index)
		{
			REPLY r(con,"SELECT %d",db_index);

			if(!r)return(false);

			return(true);
		}

		int RedisDB::DBSize()
		{
			REPLY r(con,"DBSIZE");

			return_integer;
		}

		bool RedisDB::Ping(char *result)
		{
			REPLY r(con,"PING");

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STATUS)return(false);

			strcpy(result,r->str);
			return(true);
		}

		void RedisDB::Save		(){REPLY r(con,"SAVE");}
		void RedisDB::BGSave	(){REPLY r(con,"BGSAVE");}

		void RedisDB::FlushDB	(){REPLY r(con,"FLUSHDB");}
		void RedisDB::FlushAll	(){REPLY r(con,"FLUSHALL");}

		void RedisDB::Multi		(){REPLY r(con,"MULTI");}
		void RedisDB::Discard	(){REPLY r(con,"DISCARD");}
		void RedisDB::Exec		(){REPLY r(con,"EXEC");}

        int64 RedisDB::MultiParam	(const redis_string &cmd,const int number,const char **keys)
		{
			if(number<=0||!keys)return(-1);

            //windows下编译不过。。。替换下
            //const char *argv[number+1];
            //size_t argv_len[number+1];

            const char ** const argv=new const char*[number+1];
            size_t *argv_len =new size_t[number+1];

			argv[0]=cmd.c_str();
            argv_len[0]=cmd.length();

//			const char *s;

			for(int i=0;i<number;i++)
			{
				argv[i+1]=keys[i];
				argv_len[i+1]=strlen(keys[i]);
			}

			REPLY r(con,number+1,argv,argv_len);

			delete [] argv;
			delete [] argv_len;
			return_integer;
		}

        int64 RedisDB::MultiParam	(const redis_string &cmd, const redis_string &param, const int number, const char **keys)
		{
			if(number<=0||!keys)return(-1);

            //windows下编译不过。。。替换下
            //const char *argv[number+2];
            //size_t argv_len[number+2];

            const char ** const argv=new const char*[number+1];
            size_t *argv_len =new size_t[number+1];

			argv[0]=cmd.c_str();
            argv_len[0]=cmd.length();
			argv[1]=param.c_str();
            argv_len[1]=param.length();

//            const char *s;

			for(int i=0;i<number;i++)
			{
				argv[i+2]=keys[i];
				argv_len[i+2]=strlen(keys[i]);
			}

			REPLY r(con,number+2,argv,argv_len);

			delete [] argv;
			delete [] argv_len;
			return_integer;
		}

		int RedisDB::Keys(const char *str, redis_string_list &key_list)
		{
			if(!str||!*str)return(-1);

			REPLY r(con,"keys %s",str);

			return_str_array(r,key_list);
		}
	}//namespace redis

	namespace redis
	{
		RedisDB *CreateRedisConnect(redisContext *con)
		{
			if(!con)
				return(nullptr);

			if(con->err)
			{
				printf("redisConnect error:%s\n",con->errstr);
				return(nullptr);
			}

			return(new RedisDB(con));
		}

        RedisDB *ConnectRedisDB(const char *ip,uint32 port)
		{
			return CreateRedisConnect(redisConnect(ip,port));
		}
#ifdef WIN32
#else
        RedisDB *ConnectRedisDB(const char *path)
        {
            return CreateRedisConnect(redisConnectUnix(path));
        }
#endif//WIN32
	}//namespace redis
}//namespace hgl
