#include"RedisDB.h"
#include"RedisDBReply.h"

namespace hgl
{
	namespace redis
	{
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
	}//namespace redis
}//namespace hgl
