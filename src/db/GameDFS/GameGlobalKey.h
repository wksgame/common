#ifndef GAME_GLOBAL_KEY_INCLUDE
#define GAME_GLOBAL_KEY_INCLUDE

#include"../RedisClient/RedisDB.h"

using namespace hgl::redis;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	/**
	* 全局KEY节点
	*/
	enum GameGlobalKey
	{
		ggkNone=0,

		ggkSlotMachine,			///<拉霸彩金
		ggkBroadcast,			///<全服广播ID

		ggkEnd
	};

	extern const char *dfs_global_key_node_name[32];

	bool GameGlobalKeyGet(const GameGlobalKey &node,redis_string &value);
	bool GameGlobalKeySet(const GameGlobalKey &node,const redis_string &value);

	template<typename T>
	bool GameGlobalKeyGetInteger(const GameGlobalKey &node,T &value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		redis_string str;

		if(!GameGlobalKeyGet(node,str))
		{
			value=0;
			return(false);
		}

        value=stoll(str);
        return(true);
        //return hgl::stoi(str.c_str(),value);
	}

	template<typename T>
	bool GameGlobalKeyGetUInteger(const GameGlobalKey &node,T &value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		redis_string str;

		if(!GameGlobalKeyGet(node,str))
		{
			value=0;
			return(false);
		}

        value=stoull(str);
        return(true);
        //return hgl::stou(str.c_str(),value);
	}

	template<typename T>
	bool GameGlobalKeyInc(const GameGlobalKey &node,T &new_value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Incr(dfs_global_key_node_name[node],new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
	bool GameGlobalKeyInc(const GameGlobalKey &node,T value,T &new_value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Incr(dfs_global_key_node_name[node],value,new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
	bool GameGlobalKeyDec(const GameGlobalKey &node,T &new_value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Decr(dfs_global_key_node_name[node],new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T>
	bool GameGlobalKeyDec(const GameGlobalKey &node,T value,T &new_value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Decr(dfs_global_key_node_name[node],value,new_value);

		ReleaseRedis(db);
		return(result);
	}

	template<typename T> class GameGlobalKeyValue
	{
		bool init;

		GameGlobalKey key;

		T value;

	private:

		bool load();

	public:

		GameGlobalKeyValue(GameGlobalKey k,T v):key(k),value(v)
		{
			init=false;
		}

		void save()
		{
			GameGlobalKeySet(key,value);
		}

		operator const T()const{return value;}

		void operator = (const T &v)
		{
			if(!init)
				if(value==v)return;

			value=v;
			save();
			init=false;
		}

		T operator ++(int){T o=value;GameGlobalKeyInc(key,value);return o;}
		T operator --(int){T o=value;GameGlobalKeyDec(key,value);return o;}
		T operator ++(){GameGlobalKeyInc(key,value);return value;}
		T operator --(){GameGlobalKeyDec(key,value);return value;}

		T operator += (const T &v){if(v==0)return value;GameGlobalKeyInc(key,v,value);return value;}
		T operator -= (const T &v){if(v==0)return value;GameGlobalKeyDec(key,v,value);return value;}
		T operator *= (const T &v){if(v==1)return value;value*=v;save();return value;}
		T operator /= (const T &v){if(v==1)return value;value/=v;save();return value;}

		T operator |= (const T &v){if(v==0)return value;value|=v;save();return value;}
	};//template<typename T> class GameGlobalKeyValue

	//注：redis内部传输int只有int64
    //using game_global_key_int32=GameGlobalKeyValue<int32>;
    //using game_global_key_int64=GameGlobalKeyValue<int64>;
	typedef GameGlobalKeyValue<int32> game_global_key_int32;
    typedef GameGlobalKeyValue<int64> game_global_key_int64;
}
#endif//GAME_GLOBAL_KEY_INCLUDE
