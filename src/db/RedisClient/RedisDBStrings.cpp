﻿#include"RedisDB.h"
#include"RedisDBReply.h"
#include"stdlib.h"
#include"stdio.h"
#include<cstring>

namespace hgl
{
	namespace redis
	{
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
	}//namespace redis
}//namespace hgl
