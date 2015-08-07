﻿#include"RedisDB.h"
#include"RedisDBReply.h"
#include<stdlib.h>

using namespace std;

namespace hgl
{
	namespace redis
	{
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
