﻿#include"RedisDB.h"
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
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
	}//namespace redis
}//namespace hgl
