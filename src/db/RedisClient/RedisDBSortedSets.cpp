#include"RedisDB.h"
#include"RedisDBReply.h"
#include<cstring>
#include<stdlib.h>

//windows
//#define HGL_FMT_I64			"%I64d"
//linux
#define HGL_FMT_I64			"%lld"

using namespace std;

namespace hgl
{
	namespace redis
	{
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
	}//namespace redis
}//namespace hgl
