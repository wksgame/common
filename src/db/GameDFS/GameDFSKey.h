#ifndef SHOWHAND_SERVER_GAME_DFS_KEY_INCLUDE
#define SHOWHAND_SERVER_GAME_DFS_KEY_INCLUDE

#include"../RedisClient/RedisDB.h"

using namespace hgl;
using namespace hgl::redis;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	/**
	* KEY 节点，用于保存单个重要数据
	*/
	enum GameDFSKey
	{
		gfkNone=0,

		gfkAccountMoney,			///<金钱
		gfkLevel,					///<玩家等级
		gfkExp,						///<经验值
		gfkTotalExp,				///<总经验值
		gfkNickName,				///<玩家昵称
		gfkPlayerIcon,				///<玩家图标
		gfkSex,						///<玩家性别

		gfkEnd
	};

	extern const char *dfs_key_node_name[];
	//--------------------------------------------------------------------------------------------------
    bool GameKeyGet(const redis_string &key,redis_string &value);
    bool GameKeySet(const redis_string &key,const redis_string &value);
    bool GameKeyDelete(const redis_string &key);
	//--------------------------------------------------------------------------------------------------
    bool GameKeyGet(const GameDFSKey &node,const int64 id,redis_string &value);
    bool GameKeySet(const GameDFSKey &node,const int64 id,const redis_string &value);

    bool GameKeyGet(const GameDFSKey &node,const redis_string &key,redis_string &value);
    bool GameKeySet(const GameDFSKey &node,const redis_string &key,const redis_string &value);
    bool GameKeyDelete(const GameDFSKey &node,const redis_string &key);
    bool GameKeyExpire(const GameDFSKey &node,const redis_string &key, int valid_time);
	//--------------------------------------------------------------------------------------------------
	template<typename T>
    bool GameKeyGetInteger(const redis_string &node_name,T &value)
	{
        if(node_name.empty())return(false);

        redis_string str;

		if(!GameKeyGet(node_name,str))
			return(false);

        value=stoll(str);
        return(true);
        //return hgl::stoi(str.c_str(),value);
	}

	template<typename T>
    bool GameKeyGetUInteger(const redis_string &node_name,T &value)
	{
        if(node_name.empty())return(false);

        redis_string str;

		if(!GameKeyGet(node_name,str))
			return(false);

        value=stoull(str);
        return(true);
        //eturn hgl::stou(str.c_str(),value);
	}

	template<typename T>
    bool GameKeyInc(const redis_string &node_name,T &new_value)
	{
        if(node_name.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Incr(node_name,new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
    bool GameKeyInc(const redis_string &node_name,T value,T &new_value)
	{
		if(value==0)
		{
			new_value=value;
			return(true);
		}

        if(node_name.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Incr(node_name,value,new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
    bool GameKeyDec(const redis_string &node_name,T &new_value)
	{
        if(node_name.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Decr(node_name,new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
    bool GameKeyDec(const redis_string &node_name,T value,T &new_value)
	{
		if(value==0)
		{
			new_value=value;
			return(true);
		}

        if(node_name.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Decr(node_name,value,new_value);

		ReleaseRedis(db);
		return(result);
	}
	//--------------------------------------------------------------------------------------------------
	template<typename T>
	bool GameKeyGetInteger(const GameDFSKey &node,const int64 id,T &value)
	{
		if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

        redis_string str;

		if(!GameKeyGet(node,id,str))
			return(false);

        value=stoll(str);
        return(true);
        //return hgl::stoi(str.c_str(),value);
	}

	template<typename T>
	bool GameKeyGetUInteger(const GameDFSKey &node,const int64 id,T &value)
	{
		if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

        redis_string str;

		if(!GameKeyGet(node,id,str))
			return(false);

        value=stoull(str);
        return(true);
        //return hgl::stou(str.c_str(),value);
	}

	template<typename T>
	bool GameKeyInc(const GameDFSKey &node,const int64 id,T &new_value)
	{
		if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

		RedisDB *db=AcquireRedis();

        const bool result=db->Incr(dfs_key_node_name[node]+redis_string(id),new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
	bool GameKeyInc(const GameDFSKey &node,const int64 id,T value,T &new_value)
	{
		if(value==0)
		{
			new_value=value;
			return(true);
		}

		if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

		RedisDB *db=AcquireRedis();

        const bool result=db->Incr(dfs_key_node_name[node]+redis_string(id),value,new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
	bool GameKeyDec(const GameDFSKey &node,const int64 id,T &new_value)
	{
		if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

		RedisDB *db=AcquireRedis();

        const bool result=db->Decr(dfs_key_node_name[node]+redis_string(id),new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
	bool GameKeyDec(const GameDFSKey &node,const int64 id,T value,T &new_value)
	{
		if(value==0)
		{
			new_value=value;
			return(true);
		}

		if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

		RedisDB *db=AcquireRedis();

        const bool result=db->Decr(dfs_key_node_name[node]+redis_string(id),value,new_value);

		ReleaseRedis(db);
		return(result);
	}
	//--------------------------------------------------------------------------------------------------
	template<typename T> class GameKeyValue
	{
		bool init;

		GameDFSKey key;
		int64 id;
        redis_string node_name;

		T value;

	public:

		bool load();

	public:

		GameKeyValue(GameDFSKey k,int64 i,T v)
		{
			Init(k,i,v);
		}

		bool Init(GameDFSKey k,int64 i,T v)
		{
			key=k;
			id=i;
			value=v;

			if(key<=gfkNone||key>=gfkEnd||id<=0)
			{
				init=false;
                return(false);
			}

			init=true;
            node_name=dfs_key_node_name[key]+redis_string(id);
			if(!load())
			{
				value=v;
				save();
				return(false);
			}

			return(true);
		}

		void save()
		{
			if(init)
				GameKeySet(node_name,value);
		}

		operator const T()const{return value;}

		void operator = (const T &v)
		{
			value=v;

			if(init)
				save();
		}

		const bool operator == (const T &v)const{return value==v;}
		const bool operator != (const T &v)const{return value!=v;}

		T operator ++(int){if(init){T o=value;GameKeyInc(node_name,value);return o;}else return(value++);}
		T operator --(int){if(init){T o=value;GameKeyDec(node_name,value);return o;}else return(value--);}
		T operator ++(){if(init){GameKeyInc(node_name,value);return value;}else return(++value);}
		T operator --(){if(init){GameKeyDec(node_name,value);return value;}else return(--value);}

		T operator += (const T &v){if(v==0)return value;if(init)GameKeyInc(node_name,v,value);else value+=v;return value;}
		T operator -= (const T &v){if(v==0)return value;if(init)GameKeyDec(node_name,v,value);else value-=v;return value;}
		T operator *= (const T &v){if(v==1)return value;value*=v;save();return value;}
		T operator /= (const T &v){if(v==1)return value;value/=v;save();return value;}

		T operator |= (const T &v){if(v==0)return value;value|=v;save();return value;}
	};//template<typename T> class GameKeyValue

	//注：redis内部传输int只有int64
	typedef GameKeyValue<int32> game_key_int32;
	typedef GameKeyValue<int64> game_key_int64;
}//namespace showhand
#endif//SHOWHAND_SERVER_GAME_DFS_KEY_INCLUDE
