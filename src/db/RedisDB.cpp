#include"RedisDB.h"
#include"hiredis/hiredis.h"
#include<string.h>
#include<stdlib.h>

using namespace std;

namespace hgl
{
	namespace redis
	{
		class REPLY
		{
			redisReply *reply;

		public:

			REPLY(redisContext *con,const char *format,...)
			{
				va_list ap;

				va_start(ap,format);
				reply = (redisReply *)redisvCommand(con,format,ap);
				va_end(ap);
			}

			REPLY(redisContext *con,int argc,const char **argv,const size_t *argvlen)
			{
				reply=(redisReply *)redisCommandArgv(con,argc,argv,argvlen);
			}

			~REPLY()
			{
				if(reply)
					freeReplyObject(reply);
			}

			const bool operator !()const
			{
				if(!reply)return(true);
				if(reply->type==REDIS_REPLY_ERROR)
					return(true);

				return(false);
			}

			const redisReply *operator ->()const{return reply;}
		};//class REPLY

		inline bool status_result(const REPLY &r)
		{
			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STATUS)return(false);

// 			LOG_INFO("Redis return status");
			return true;
		}

        inline int64 integer_result(const REPLY &r)
		{
			if(!r)return(-1);
			if(r->type!=REDIS_REPLY_INTEGER)return(-1);

// 			LOG_INFO("Redis return integer: "+UTF8String(r->integer));
			return r->integer;
		}

		inline bool bool_result(const REPLY &r)
		{
			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

// 			LOG_INFO("Redis return bool: "+UTF8String((r->integer?"true":"false")));
            return r->integer==1;       ///<1success 0fail
		}

		inline int string_array_result(const REPLY &r,redis_string_list &sl)
		{
			if(!r)return(-1);
			if(r->type!=REDIS_REPLY_ARRAY)return(-1);

// 			LOG_INFO("Redis return string array: "+UTF8String(r->elements));

			for(int i=0;i<r->elements;i++)
                sl.push_back(redis_string(r->element[i]->str,r->element[i]->len));

			return r->elements;
		}

		inline bool string_array_first_result(const REPLY &r,redis_string &str)
		{
			if(!r)return(false);
			if(r->type!=REDIS_REPLY_ARRAY)return(false);

// 			LOG_INFO("Redis return string array first: "+UTF8String(r->elements));

			if(r->elements<=0)
				return(false);

            //str.Set(r->element[0]->str,r->element[0]->len);
            //str.insert(0,r->element[0]->str,r->element[0]->len);
			str.assign(r->element[0]->str,r->element[0]->len);

			return true;
		}

		inline int pair_string_array_result(const REPLY &r,redis_string_list &field_list,redis_string_list &value_list)
		{
			if(!r)return(-1);
			if(r->type!=REDIS_REPLY_ARRAY)return(-1);

// 			LOG_INFO("Redis return string array: "+UTF8String(r->elements));

			int index=0;
			const int count=r->elements/2;

			for(int i=0;i<count;i++)
			{
				field_list.push_back(redis_string(r->element[index]->str,r->element[index]->len));
				++index;

				value_list.push_back(redis_string(r->element[index]->str,r->element[index]->len));
				++index;

				//redis_string_list_add(field_list,redis_string(r->element[index]->str,r->element[index]->len));++index;
				//redis_string_list_add(value_list,redis_string(r->element[index]->str,r->element[index]->len));++index;
			}

			return count;
		}

//		inline int hashs_pair_result(const REPLY &r,redis_hashs_item_list &result)
//		{
//			if(!r)return(-1);
//			if(r->type!=REDIS_REPLY_ARRAY)return(-1);

//			int index=0;
//			const int count=r->elements/2;

//			for(int i=0;i<count;i++)
//			{
//				UTF8String key(r->element[index]->str,r->element[index]->len);

//				++index;

//				result.Add(key,new HashsItem(r->element[index]->str,r->element[index]->len));

//				++index;
//			}

//			return count;
//		}

		#define return_status					return(status_result(r))
		#define return_integer					return(integer_result(r))
		#define return_bool						return(bool_result(r))
		#define return_str_array(r,sl)			return(string_array_result(r,sl))
		#define return_str_array_first(r,str)	return(string_array_first_result(r,str))
		#define return_pair_str_array(r,fl,vl)	return(pair_string_array_result(r,fl,vl))
//		#define return_hashs_pair(r,h)			return(hashs_pair_result(r,h))


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

		// string opeartor

		/**
		* 设置一个数据
		* @param key 健值
		* @param value 内容
		* @param et 过期时间(以秒为单位,0表示永不过期)
		*/
		bool RedisDB::Set(const redis_string &key,const redis_string &value,int et)
		{
			if(et>0)
			{
                redis_string etstr(to_string(et));

				const char *argv[]=
				{
					"SETEX",
					key.c_str(),
					value.c_str(),
					etstr.c_str()
				};

				const size_t argvlen[]=
				{
					5,
                    (size_t)key.length(),
                    (size_t)value.length(),
                    (size_t)etstr.length()
				};

				REPLY r(con,4,argv,argvlen);
				//REPLY r(con,"SETEX %s %d %s",key,et,value);

				if(!r)return(false);
			}
			else
			{
				const char *argv[]=
				{
					"SET",
					key.c_str(),
					value.c_str()
				};

				const size_t argvlen[]=
				{
					3,
                    (size_t)key.length(),
                    (size_t)value.length()
				};

				REPLY r(con,3,argv,argvlen);
				//REPLY r(con,"SET %s %s",key,value);

				if(!r)return(false);
			}

			return(true);
		}

		/**
		* 设置一个数据，如果key已经存在，则放弃操作
		* @param key 健值
		* @param value 内容
		*/
		bool RedisDB::SetNx(const redis_string &key,const redis_string &value)
		{
			const char *argv[]=
			{
				"SETNX",
				key.c_str(),
				value.c_str()
			};

			const size_t argvlen[]=
			{
				5,
                (size_t)key.length(),
                (size_t)value.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"SETNX %s %s",key,value);

			return_bool;
		}

		bool RedisDB::Get(const redis_string &key,redis_string &result)
		{
			const char *argv[]=
			{
				"GET",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				3,
                (size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"GET %s",key);

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

            result.insert(0,r->str,r->len);
			return(true);
		}

		/**
		* 取得旧的数据，并写入新的数据
		* @param key 要操作的键
		* @param value 要写入的新的值
		* @param result 取出的旧值所放入的位置
		*/
		bool RedisDB::GetSet(const redis_string &key,const redis_string &value,redis_string &result)
		{
			const char *argv[]=
			{
				"GETSET",
				key.c_str(),
				value.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
                (size_t)value.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"GETSET %s %s",key,value);

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

            result.insert(0,r->str,r->len);
			return(true);
		}

		int RedisDB::Del(const redis_string &key)
		{
            if(key.empty())return(-1);

			const char *argv[]=
			{
				"DEL",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				3,
                (size_t)key.length(),
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"DEL %s",key);

			return_integer;
		}

		int RedisDB::Del(int number,const char **keys)
		{
			if(number<=0)return(number);
			if(!keys)return(-1);

			return MultiParam("DEL",number,keys);
		}

		bool RedisDB::RandomKey(redis_string &result)
		{
			const char *argv[]=
			{
				"RANDOMKEY",
			};

			const size_t argvlen[]=
			{
				9,
			};

			REPLY r(con,1,argv,argvlen);
			//REPLY r(con,"RANDOMKEY");

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)
				return(false);

            result.insert(0,r->str,r->len);
			return(true);
		}

		bool RedisDB::Exists(const redis_string &key)
		{
			const char *argv[]=
			{
				"EXISTS",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"EXISTS %s",key);

			return_bool;
		}

		/**
		* 重命名一个KEY
		* @param old_name 旧名称
		* @param new_name 新名称
		* @param nx 新名称是否必须不存在
		*/
		bool RedisDB::Rename(const redis_string &old_name,const redis_string &new_name,bool nx)
		{
			const char *argv[]=
			{
				nx?"RENAMENX":"RENAME",
				old_name.c_str(),
				new_name.c_str()
			};

			const size_t argvlen[]=
			{
				(size_t)(nx?8:6),
                (size_t)old_name.length(),
                (size_t)new_name.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"%s %s %s",nx?"RENAMENX":"RENAME",old_name,new_name);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STATUS)return(false);

			return(true);
		}

		bool RedisDB::Expire(const char *key,int t)
		{
			REPLY r(con,"EXPIRE %s %d",key,t);

			return_bool;
		}

		bool RedisDB::Persist(const redis_string &key)
		{
			const char *argv[]=
			{
				"PERSIST",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				7,
                (size_t)key.length(),
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"PERSIST %s",key);

			return_bool;
		}

		int  RedisDB::TTL(const redis_string &key)
		{
			const char *argv[]=
			{
				"TTL",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				3,
                (size_t)key.length(),
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"TTL %s",key);

			return_integer;
		}

		int  RedisDB::Strlen(const redis_string &key)
		{
			const char *argv[]=
			{
				"STRLEN",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"STRLEN %s",key);

			return_integer;
		}

		/**
		* 追加一段数据到原本的数据上
		* @param key 要追加数据的键
		* @param value 要追加的数据
		* @return 追加数据后新的数据长度,返回-1表示出错
		*/
		int RedisDB::Append(const redis_string &key,const redis_string &value)
		{
			const char *argv[]=
			{
				"APPEND",
				key.c_str(),
				value.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
                (size_t)value.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"APPEND %s %s",key,value);

			return_integer;
		}

		/**
		* 按范围获取数据
		* @param key 要获取数据的键
		* @param start 起始位置
		* @param end 结束位置
		* @param result 结果存放位置
		*/
		bool RedisDB::Get(const char *key,int start,int end,char *result)
		{
			REPLY r(con,"GETRANGE %s %d %d",key,start,end);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

            memcpy(result,r->str,r->len);
			result[r->len]=0;
			return(true);
		}

		// string number

		bool RedisDB::Incr(const redis_string &key,int32 &result)
		{
			const char *argv[]=
			{
				"INCR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"INCR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,const int32 value,int32 &result)
		{
            redis_string val_str(to_string(value));

			const char *argv[]=
			{
				"INCRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
                (size_t)val_str.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"INCRBY %s %d",key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,int64 &result)
		{
			const char *argv[]=
			{
				"INCR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"INCR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,const int64 value,int64 &result)
		{
            redis_string val_str(to_string(value));

			const char *argv[]=
			{
				"INCRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
                (size_t)val_str.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"INCRBY %s " HGL_FMT_I64,key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Incr(const redis_string &key,const float value,float &result)
		{
            redis_string val_str(to_string(value));

			const char *argv[]=
			{
				"INCRBYFLOAT",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				11,
                (size_t)key.length(),
                (size_t)val_str.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"INCRBYFLOAT %s %f",key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

            //return stof(r->str,r->len,result);
            result=stof(r->str);

            return(true);
		}

		bool RedisDB::Decr(const redis_string &key,int32 &result)
		{
			const char *argv[]=
			{
				"DECR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"DECR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const redis_string &key,const int32 value,int32 &result)
		{
            redis_string val_str(to_string(value));

			const char *argv[]=
			{
				"DECRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
                (size_t)val_str.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"DECRBY %s %d",key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const redis_string &key,int64 &result)
		{
			const char *argv[]=
			{
				"DECR",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"DECR %s",key);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		bool RedisDB::Decr(const redis_string &key,const int64 value,int64 &result)
		{
            redis_string val_str(to_string(value));

			const char *argv[]=
			{
				"DECRBY",
				key.c_str(),
				val_str.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
                (size_t)val_str.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"DECRBY %s " HGL_FMT_I64,key,value);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			result=r->integer;
			return(true);
		}

		// string binary
		/**
		* 设置一个数据
		* @param key 健值
		* @param value 内容
		* @param size 长度
		* @param et 过期时间(以秒为单位,0表示永不过期)
		*/
        bool RedisDB::SetBinary(const redis_string &key,const void *value,int size,int et)
		{
			if(et>0)
			{
                redis_string et_str(to_string(et));

				const char *argv[]=
				{
					"SETEX",
					key.c_str(),
					et_str.c_str(),
					(const char *)value
				};

				const size_t argvlen[]=
				{
					5,
                    (size_t)key.length(),
                    (size_t)et_str.length(),
					(size_t)size
				};

				//REPLY r(con,"SETEX %s %d %b",key,et,value,size);
				REPLY r(con,4,argv,argvlen);

				if(!r)return(false);
			}
			else
			{
				const char *argv[]=
				{
					"SET",
					key.c_str(),
					(const char *)value
				};

				const size_t argvlen[]=
				{
					3,
                    (size_t)key.length(),
					(size_t)size
				};

				//REPLY r(con,"SET %s %b",key,value,size);
				REPLY r(con,3,argv,argvlen);

				if(!r)return(false);
			}

			return(true);
		}

		/**
		* 设置一个数据，如果key已经存在，则放弃操作
		* @param key 健值
		* @param value 内容
		* @param size 长度
		*/
		bool RedisDB::SetNxBinary(const redis_string &key,const void *value,int size)
		{
			const char *argv[]=
			{
				"SETNX",
				key.c_str(),
				(const char *)value
			};

			const size_t argvlen[]=
			{
				5,
                (size_t)key.length(),
				(size_t)size
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"SETNX %s %b",key,value,size);

			return_bool;
		}

		/**
		 * 获取一个数据
		 * @param key 键值
		 * @param result 结果存放内存
		 * @param max_size 内存最大长度
		 * @return 取出的数据长度
		 */
		int RedisDB::GetBinary(const redis_string &key,void *result,int max_size)
		{
			const char *argv[]=
			{
				"GET",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				3,
                (size_t)key.length(),
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"GET %s",key);

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			const int len=r->len;

			if(len<max_size)
				memcpy(result,r->str,len);
			else
				memcpy(result,r->str,max_size);

			return(len);
		}

		/**
		 * 获取一个数据
		 * @param key 键值
		 * @param size 获取出的数据长度
		 * @return 取出的数据
		 */
		void *RedisDB::GetBinary(const redis_string &key,int &size)
		{
			const char *argv[]=
			{
				"GET",
				key.c_str(),
			};

			const size_t argvlen[]=
			{
				3,
                (size_t)key.length(),
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"GET %s",key);

			if(!r)return(nullptr);

			if(r->type!=REDIS_REPLY_STRING)
				return(nullptr);

			size=r->len;

			if(size<=0)
				return(nullptr);

			void *result=new char[size];

			memcpy(result,r->str,size);
			return(result);
		}

		/**
		* 取得旧的数据，并写入新的数据
		* @param key 要操作的键
		* @param data 要写入的新的值
		* @param size 要写入的新数据的长度
		* @param result 取出的旧值所放入的位置
		* @param max_size 最大存放长度
		*/
		int RedisDB::GetSetBinary(const redis_string &key,const void *data,int size,void *result,int max_size)
		{
			const char *argv[]=
			{
				"GETSET",
				key.c_str(),
				(const char *)data
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
				(size_t)size
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"GETSET %s %b",key,data,size);

			if(!r)return(false);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			if(r->len<max_size)
			{
				memcpy(result,r->str,r->len);
				return r->len;
			}
			else
			{
				memcpy(result,r->str,max_size);
				return max_size;
			}
		}

		/**
		* 追加一段数据到原本的数据上
		* @param key 要追加数据的键
		* @param value 要追加的数据
		* @param size 要追加的数据长度
		* @return 追加数据后新的数据长度,返回-1表示出错
		*/
		int RedisDB::AppendBinary(const redis_string &key,const void *data,int size)
		{
			const char *argv[]=
			{
				"APPEND",
				key.c_str(),
				(const char *)data
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
				(size_t)size
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"APPEND %s %b",key,data,size);

			return_integer;
		}

		// set

		bool RedisDB::SAdd(const redis_string &set,const redis_string &member)
		{
            if(set.empty()||member.empty())
                return(false);

			const char *argv[]=
			{
				"SADD",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)set.length(),
                (size_t)member.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"SADD %s %s",set,member);

			return_integer;
		}

        bool RedisDB::SAdd(const redis_string &set,int count,const char **members)
		{
//			if(!set||!(*set)||count<=0||!members)return(-1);

//			return MultiParam("SADD",set,count,members);

            if(set.empty()||count<=0||!members)return(false);

            return (MultiParam("SADD",set,count,members)!=-1);
		}

		bool RedisDB::SRem(const redis_string &set,const redis_string &member)
		{
            if(set.empty()||member.empty())
                return(false);

			const char *argv[]=
			{
				"SREM",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)set.length(),
                (size_t)member.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"SREM %s %s",set,member);

			return_integer;
		}

        bool RedisDB::SRem(const redis_string &set,int count,const char **members)
		{
//			if(!set||!(*set)||count<=0||!members)return(-1);

//			return MultiParam("SREM",set,count,members);

            if(set.empty()||count<=0||!members)return(false);

            return (MultiParam("SREM",set,count,members)!=-1);
		}

		bool RedisDB::SRandMember(const char *key,redis_string &result,int count)
		{
			if(!key||!(*key))return(false);

			if(count>0)
			{
				REPLY r(con,"SRANDMEMBER %s %d",key,count);

				if(r->type!=REDIS_REPLY_STRING)
					return(false);

				result=r->str;
			}
			else
			{
				REPLY r(con,"SRANDMEMBER %s",key);

				if(r->type!=REDIS_REPLY_STRING)
					return(false);

				result=r->str;
			}

			return(true);
		}

		bool RedisDB::SPop(const char *key,redis_string &result)
		{
			if(!key||!(*key))return(false);

			REPLY r(con,"SPOP %s",key);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			result=r->str;
			return(true);
		}

		bool RedisDB::SMove(const char *src,const char *dst,const char *member)
		{
			if(!src||!(*src)
			 ||!dst||!(*dst)
			 ||!member||!(*member))return(false);

			REPLY r(con,"SMOVE %s %s %s",src,dst,member);

			return_integer;
		}

		int RedisDB::SCard(const char *set)
		{
			if(!set||!(*set))return(-1);

			REPLY r(con,"SCARD %s",set);

			return_integer;
		}

		bool RedisDB::SIsMember(const redis_string &set,const redis_string &member)
		{
            if(set.empty()||member.empty())
                return(false);

			const char *argv[]=
			{
				"SISMEMBER",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				9,
                (size_t)set.length(),
                (size_t)member.length()
			};

			REPLY r(con,3,argv,argvlen);

            return_integer;     ///<成功返回1,失败返回0
		}

		bool RedisDB::SMembers(const char *set,redis_string_list &result_list)
		{
			REPLY r(con,"SMEMBERS %s",set);

			return_str_array(r,result_list);
		}

		bool RedisDB::SInter(const redis_string_list &set_list,redis_string_list &result_list)
		{
            if(set_list.size()<=1)return(false);

            redis_string str="SINTER ";

//            const int count=set_list.size();

//            for(int i=0;i<count;i++)
//			{
//				str+=set_list[i];
//				str+=' ';
//			}

            for(auto i:set_list)
            {
                str+=i;
                str+=' ';
            }

            REPLY r(con,str.c_str());
			return_str_array(r,result_list);
		}

		int RedisDB::SInterStore(const char *dst_set,const redis_string_list &set_list)
		{
			if(!dst_set||!(*dst_set))return(-1);
            if(set_list.size()<=1)return(-2);

            redis_string str="SINTERSTORE ";

			str+=dst_set;
			str+=' ';

//            const int count=set_list.size();

//            for(int i=0;i<count;i++)
//			{
//				str+=set_list[i];
//				str+=' ';
//			}

            for(auto i:set_list)
            {
                str+=i;
                str+=' ';
            }

            REPLY r(con,str.c_str());
			return_integer;
		}

		bool RedisDB::SUnion(const redis_string_list &set_list,redis_string_list &result_list)
		{
            if(set_list.size()<=1)return(false);

            redis_string str="SUNION ";

//            const int count=set_list.size();

//            for(int i=0;i<count;i++)
//			{
//				str+=set_list[i];
//				str+=' ';
//			}

            for(auto i:set_list)
            {
                str+=i;
                str+=' ';
            }

            REPLY r(con,str.c_str());
			return_str_array(r,result_list);
		}

		int RedisDB::SUnionStore(const char *dst_set,const redis_string_list &set_list)
		{
			if(!dst_set||!(*dst_set))return(-1);
            if(set_list.size()<=1)return(-2);

            redis_string str="SUNIONSTORE ";

			str+=dst_set;
			str+=' ';

//            const int count=set_list.size();

//            for(int i=0;i<count;i++)
//			{
//				str+=set_list[i];
//				str+=' ';
//			}

            for(auto i:set_list)
            {
                str+=i;
                str+=' ';
            }

            REPLY r(con,str.c_str());
			return_integer;
		}

		//sort set

        int64 RedisDB::ZAdd(const char *set,int64 score,const char *member)
		{
            if(!set||!(*set)||!member||!(*member))
                return(-1);

			REPLY r(con,"ZADD %s " HGL_FMT_I64 " %s",set,score,member);

            return_integer;
		}

        int64 RedisDB::ZAdd(const char *set,int count,int64 *scores,const char **members)
		{
            if(!set||!(*set)||count<=0||!scores||!members)
                return(-1);

			char str[1024]="ZADD ";
//			char tmp[32];
			char *p;
			int len;

			strcpy(str+5,set);

			p=str+strlen(str);

			for(int i=0;i<count;i++)
			{
//				*p++=' ';
//				len=itos_rl(tmp,32,scores[i]);
//				memcpy(p,tmp,len);
//				p+=len;
//				*p++=' ';
//				len=strlen(members[i]);
//				memcpy(p,members[i],len);
//				p+=len;

                *p++=' ';
                //len=itos_rl(tmp,32,scores[i]);
                //memcpy(p,tmp,len);
                string s=to_string(scores[i]);
                len=s.length();
                memcpy(p,s.c_str(),len);

                p+=len;
                *p++=' ';
                len=strlen(members[i]);
                memcpy(p,members[i],len);
                p+=len;
			}

			*p=0;

			REPLY r(con,str);
			return_integer;
		}

		bool RedisDB::ZRem(const redis_string &set,const redis_string &member)
		{
            if(set.empty()||member.empty())return(false);

			const char *argv[]=
			{
				"ZREM",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)set.length(),
                (size_t)member.length()
			};

			REPLY r(con,3,argv,argvlen);

			return_integer;
		}

        bool RedisDB::ZRem(const redis_string &set,int count,const char **members)
		{
            if(set.empty()||count<=0||!members)return(-1);

			return MultiParam("ZREM",set,count,members);
		}

		bool RedisDB::ZRemRangeByRank(const char *set,int64 start,int64 top)
		{
			REPLY r(con,"ZREMRANGEBYRANK %s " HGL_FMT_I64 " " HGL_FMT_I64,set,start,top);

			return_integer;
		}

		bool RedisDB::ZRemRangeByScore(const char *set,int64 min_score,int64 max_score)
		{
			REPLY r(con,"ZREMRANGEBYSCORE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,min_score,max_score);

			return_integer;
		}

        bool RedisDB::ZInc(const redis_string &set,const redis_string &member,const redis_string &value,redis_string &result)
		{
            if(set.empty()||member.empty()||stoll(value)==0)
                return(false);

			const char *argv[]=
			{
				"ZINCRBY",
				set.c_str(),
				value.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				7,
                (size_t)set.length(),
                (size_t)value.length(),
                (size_t)member.length()
			};

			REPLY r(con,4,argv,argvlen);

			result=r->str;
			return(true);
		}

		int RedisDB::ZCard(const redis_string &set)
		{
            if(set.empty())return(false);

			const char *argv[]=
			{
				"ZCARD",
				set.c_str(),
			};

			const size_t argvlen[]=
			{
				5,
                (size_t)set.length(),
			};

			REPLY r(con,2,argv,argvlen);

			return_integer;
		}

		int RedisDB::ZCount(const char *set,int64 min_score,int64 max_score)
		{
			REPLY r(con,"ZCOUNT %s " HGL_FMT_I64 " " HGL_FMT_I64,set,min_score,max_score);

			return_integer;
		}

		int RedisDB::ZRange(const char *set,int64 start,int64 stop,redis_string_list &sl)
		{
			REPLY r(con,"ZRANGE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,start,stop);

			return_str_array(r,sl);
		}

		int RedisDB::ZRangeByScore(const char *set,int64 min_score,int64 max_score,redis_string_list &sl)
		{
			REPLY r(con,"ZRANGEBYSCORE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,min_score,max_score);

			return_str_array(r,sl);
		}

		bool RedisDB::ZRange(const char *set,int64 score,redis_string &str)
		{
			REPLY r(con,"ZRANGE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,score,score);

			return_str_array_first(r,str);
		}

		bool RedisDB::ZRangeByScore(const char *set,int64 score,redis_string &str)
		{
			REPLY r(con,"ZRANGEBYSCORE %s " HGL_FMT_I64 " " HGL_FMT_I64,set,score,score);

			return_str_array_first(r,str);
		}

		bool RedisDB::ZScore(const redis_string &set,const redis_string &member,int64 &score)
		{
            if(set.empty()||member.empty())return(false);

			const char *argv[]=
			{
				"ZSCORE",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)set.length(),
                (size_t)member.length()
			};

			REPLY r(con,3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_STRING)return(false);

            //stoi(r->str,(std::size_t)score);
            score=stoll(r->str);

			return(true);
		}

		bool RedisDB::ZRank(const redis_string &set,const redis_string &member,int64 &rank)
		{
            if(set.empty()||member.empty())return(false);

			const char *argv[]=
			{
				"ZRANK",
				set.c_str(),
				member.c_str()
			};

			const size_t argvlen[]=
			{
				5,
                (size_t)set.length(),
                (size_t)member.length()
			};

			REPLY r(con,3,argv,argvlen);

			if(!r)return(false);
			if(r->type!=REDIS_REPLY_INTEGER)return(false);

			rank=r->integer;
			return(true);
		}

		//hash

		bool RedisDB::HSet(const redis_string &key,const redis_string &field,const char *value,const int size)
		{
			const char *argv[]=
			{
				"HSET",
				key.c_str(),
				field.c_str(),
				value
			};

			const size_t argvlen[]=
			{
				4,
                (size_t)key.length(),
                (size_t)field.length(),
				(size_t)size
			};

			REPLY r(con,4,argv,argvlen);
			//REPLY r(con,"HSET %s %s %s",key,field,value);

			return_bool;
		}

		bool RedisDB::HSetNX(const redis_string &key,const redis_string &field,const redis_string &value)
		{
			const char *argv[]=
			{
				"HSETNX",
				key.c_str(),
				field.c_str(),
				value.c_str()
			};

			const size_t argvlen[]=
			{
				6,
                (size_t)key.length(),
                (size_t)field.length(),
                (size_t)value.length()
			};

			REPLY r(con,4,argv,argvlen);
			//REPLY r(con,"HSETNX %s %s %s",key,field,value);

			return_bool;
		}

		bool RedisDB::HSet(const redis_string &key,const redis_string_list &field,const redis_string_list &value)
		{
            //if(!key||!(*key))return(false);
            if(key.empty())
                return(false);

			const int count=field.size();

			if(count<=0)return(true);

			if(count!=value.size())return(false);

			const char hmset[]="HMSET";

			const int argc=(count*2)+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hmset;
			argvlen[0]=5;

			argv[1]=key.c_str();
			argvlen[1]=key.length();

			int index=2;

// 			redis_string **fp=field.GetDataList();
// 			redis_string **vp=value.GetDataList();
//
// 			for(int i=0;i<count;i++)
// 			{
// 				argv[index]=(*fp)->c_str();
// 				argvlen[index]=(*fp)->length();
//
// 				++index;
//
// 				argv[index]=(*vp)->c_str();
// 				argvlen[index]=(*vp)->length();
//
// 				++index;
//
// 				++fp;
// 				++vp;
// 			}

			auto fp = field.begin();
			auto vp = value.begin();

			for(;fp != field.end(); ++fp,++vp)
			{
				argv[index]=(*fp).c_str();
				argvlen[index]=(*fp).length();

				++index;

				argv[index]=(*vp).c_str();
				argvlen[index]=(*vp).length();

				++index;
			}

			REPLY r(con,argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;
			return_status;
		}

// 		bool RedisDB::HSet(const redis_string &key,const redis_hashs_item_list &item_list)
// 		{
// 			if(!key||!(*key))return(false);
//
// 			const int count=item_list.GetCount();
//
// 			if(count<=0)return(true);
//
// 			const char hmset[]="HMSET";
//
// 			const int argc=(count*2)+2;
//
// 			const char **argv=new const char *[argc];
// 			size_t *argvlen=new size_t[argc];
//
// 			argv[0]=hmset;
// 			argvlen[0]=5;
//
// 			argv[1]=key.c_str();
// 			argvlen[1]=key.length();
//
// 			int index=2;
//
// 			auto **idp=item_list.GetDataList();
//
// 			for(int i=0;i<count;i++)
// 			{
// 				argv[index]=(*idp)->first.c_str();
// 				argvlen[index]=(*idp)->first.length();
//
// 				++index;
//
// 				argv[index]=(const char *)((*idp)->second->mos.GetData());
// 				argvlen[index]=(*idp)->second->mos.Tell();
//
// 				++index;
// 				++idp;
// 			}
//
// 			REPLY r(con,argc,argv,argvlen);
//
// 			delete[] argv;
// 			delete[] argvlen;
// 			return_status;
// 		}

// 		int RedisDB::HSetUpdate(const redis_string &key,const redis_hashs_item_list &item_list)
// 		{
// 			int update_count=0;
//
// 			const int count=item_list.GetCount();
//
// 			auto **idp=item_list.GetDataList();
//
// 			for(int i=0;i<count;i++)
// 			{
// 				if((*idp)->second->update)
// 					++update_count;
//
// 				++idp;
// 			}
//
// 			if(update_count<=0)
// 				return(update_count);
//
// 			const char hmset[]="HMSET";
//
// 			const int argc=(update_count*2)+2;
//
// 			const char **argv=new const char *[argc];
// 			size_t *argvlen=new size_t[argc];
//
// 			argv[0]=hmset;
// 			argvlen[0]=5;
//
// 			argv[1]=key.c_str();
// 			argvlen[1]=key.length();
//
// 			int index=2;
//
// 			idp=item_list.GetDataList();
//
// 			for(int i=0;i<count;i++)
// 			{
// 				if((*idp)->second->update)
// 				{
// 					argv[index]=(*idp)->first.c_str();
// 					argvlen[index]=(*idp)->first.length();
//
// 					++index;
//
// 					argv[index]=(const char *)((*idp)->second->mos.GetData());
// 					argvlen[index]=(*idp)->second->mos.Tell();
//
// 					++index;
// 				}
//
// 				++idp;
// 			}
//
// 			REPLY r(con,argc,argv,argvlen);
//
// 			delete[] argv;
// 			delete[] argvlen;
//
// 			if(!status_result(r))
// 				return(-update_count);
//
// 			idp=item_list.GetDataList();
//
// 			for(int i=0;i<count;i++)
// 			{
// 				(*idp)->second->update=false;
// 				++idp;
// 			}
//
// 			return(update_count);
// 		}

		bool RedisDB::HGet(const redis_string &key,const redis_string &field,redis_string &value)
		{
			const char *argv[]=
			{
				"HGET",
				key.c_str(),
				field.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.length(),
				(size_t)field.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"HGET %s %s",key,field);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			value.assign(r->str,r->len);
			return(true);
		}

		int RedisDB::HGet(const redis_string &key,const redis_string_list &field,redis_string_list &result_list)
		{
			const char hmget[]="HMGET";

			const int count=field.size();

			const int argc=count+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hmget;
			argvlen[0]=5;

			argv[1]=key.c_str();
			argvlen[1]=key.length();

			int index=2;

// 			auto **sp=field.GetDataList();
//
// 			for(int i=0;i<count;i++)
// 			{
// 				argv[index]=(*sp)->c_str();
// 				argvlen[index]=(*sp)->length();
//
// 				++index;
// 				++sp;
// 			}

			auto sp = field.begin();

			for(;sp != field.end(); ++sp,++index)
			{
				argv[index]=(*sp).c_str();
				argvlen[index]=(*sp).length();
			}

			REPLY r(con,argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;

			return_str_array(r,result_list);
		}

		int RedisDB::HGetAll(const redis_string &key,redis_string_list &field,redis_string_list &value)
		{
			const char *argv[]=
			{
				"HGETALL",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				7,
				(size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"HGETALL %s",key);

			return_pair_str_array(r,field,value);
		}

// 		int RedisDB::HGetAll(const redis_string &key,redis_hashs_item_list &result)
// 		{
// 			const char *argv[]=
// 			{
// 				"HGETALL",
// 				key.c_str()
// 			};
//
// 			const size_t argvlen[]=
// 			{
// 				7,
// 				(size_t)key.length()
// 			};
//
// 			REPLY r(con,2,argv,argvlen);
// 			//REPLY r(con,"HGETALL %s",key);
//
// 			return_hashs_pair(r,result);
// 		}

		int RedisDB::HVals(const redis_string &key,redis_string_list &result_list)
		{
			const char *argv[]=
			{
				"HVALS",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				5,
				(size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"HVALS %s",key);

			return_str_array(r,result_list);
		}

		bool RedisDB::HIncr(const char *key,const char *field,int &value)
		{
			REPLY r(con,"HINCRBY %s %s %d",key,field,value);

			if(r->type!=REDIS_REPLY_INTEGER)
				return(false);

			value=r->integer;
			return(true);
		}

		bool RedisDB::HIncrByFloat(const char *key,const char *field,double &value)
		{
			REPLY r(con,"HINCRBY %s %s " HGL_FMT_DOUBLE,key,field,value);

			if(r->type!=REDIS_REPLY_STRING)
				return(false);

			value = atof(r->str);
			return(true);
		}

		bool RedisDB::HDel(const redis_string &key,const redis_string &field)
		{
			const char *argv[]=
			{
				"HDEL",
				key.c_str(),
				field.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.length(),
				(size_t)field.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"HDEL %s %s",key,field);

			return_bool;
		}

		bool RedisDB::HDel(const redis_string &key,const redis_string_list &field)
		{
			const char hdel[]="HDEL";

			const int count=field.size();

			const int argc=count+2;

			const char **argv=new const char *[argc];
			size_t *argvlen=new size_t[argc];

			argv[0]=hdel;
			argvlen[0]=4;

			argv[1]=key.c_str();
			argvlen[1]=key.length();

			int index=2;

// 			auto **sp=field.GetDataList();
//
// 			for(int i=0;i<count;i++)
// 			{
// 				argv[index]=(*sp)->c_str();
// 				argvlen[index]=(*sp)->length();
//
// 				++index;
// 				++sp;
// 			}

			auto sp = field.begin();

			while(sp!=field.end())
			{
				argv[index]=(*sp).c_str();
				argvlen[index]=(*sp).length();

				++index;
				++sp;
			}

			REPLY r(con,argc,argv,argvlen);

			delete[] argv;
			delete[] argvlen;

			return_bool;
		}

		bool RedisDB::HExists(const redis_string &key,const redis_string &field)
		{
			const char *argv[]=
			{
				"HEXISTS",
				key.c_str(),
				field.c_str()
			};

			const size_t argvlen[]=
			{
				7,
				(size_t)key.length(),
				(size_t)field.length()
			};

			REPLY r(con,3,argv,argvlen);
			//REPLY r(con,"HEXISTS %s %s",key,field);

			return_bool;
		}

		bool RedisDB::HKeys(const redis_string &key,redis_string_list &result_list)
		{
			const char *argv[]=
			{
				"HKEYS",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				5,
				(size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"HKEYS %s",key);

			return_str_array(r,result_list);
		}

		int RedisDB::HLen(const redis_string &key)
		{
			const char *argv[]=
			{
				"HLEN",
				key.c_str()
			};

			const size_t argvlen[]=
			{
				4,
				(size_t)key.length()
			};

			REPLY r(con,2,argv,argvlen);
			//REPLY r(con,"HLEN %s",key);

			return_integer;
		}
	}//namespace redis
}//namespace hgl
