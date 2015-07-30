#include"GameDFSKey.h"
#include"../RedisClient/RedisDB.h"

using namespace hgl;
using namespace hgl::redis;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	RedisDB *AcquireRedis(int);
	void ReleaseRedis(int);

	#define LESS_GAME_KEY	gfkRankWealth		//次要游戏KEY

	const char *dfs_key_node_name[]=
	{
		"",

		"k_am:",			//AccountMoney
		"k_lv:",			//Level
		"k_exp:",			//exp
		"k_texp:",			//TotalExp
		"k_nn:",			//NickName
		"k_icon:",			//PlayerIcon
		"k_sex:",			//PlayerSex

		""
	};

	template<> bool GameKeyValue<int32>::load(){return GameKeyGetInteger(node_name,value);}
	template<> bool GameKeyValue<int64>::load(){return GameKeyGetInteger(node_name,value);}
	template<> bool GameKeyValue<uint32>::load(){return GameKeyGetUInteger(node_name,value);}
	template<> bool GameKeyValue<uint64>::load(){return GameKeyGetUInteger(node_name,value);}

	bool GameKeyGet(const redis_string &key,redis_string &value)
	{
        if(key.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Get(key,value);

		ReleaseRedis(db);
        //return(result);
		return(result);
	}

	bool GameKeySet(const redis_string &key,const redis_string &value)
	{
        if(key.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Set(key,value);

		ReleaseRedis(db);
        return(result);
	}

	bool GameKeyDelete(const redis_string &key)
	{
        if(key.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->Del(key);

		ReleaseRedis(db);
        return(result);
	}

//	bool GameKeyExpire(const redis_string &key,int valid_time)
//	{
//        if(key.empty())return(false);

//		RedisDB *db=AcquireRedis();

//		const bool result=db->Expire(key, valid_time);

//		ReleaseRedis(db);
//        return(result);
//	}

	bool GameKeyGet(const GameDFSKey &node,const int64 id,redis_string &value)
	{
        if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

        return GameKeyGet(dfs_key_node_name[node]+to_string(id),value);
	}

	bool GameKeySet(const GameDFSKey &node,const int64 id,const redis_string &value)
	{
        if(node<=gfkNone||node>=gfkEnd||id<=0)return(false);

        return GameKeySet(dfs_key_node_name[node]+to_string(id),value);
	}

	bool GameKeyGet(const GameDFSKey &node,const redis_string &key,redis_string &value)			///key值为字符串
	{
        if(node<=gfkNone||node>=gfkEnd/*||key == nullptr*/|| key.empty())return(false);

		return GameKeyGet(dfs_key_node_name[node]+key,value);
	}

	bool GameKeySet(const GameDFSKey &node,const redis_string &key,const redis_string &value)			///key值为字符串
	{
        if(node<=gfkNone||node>=gfkEnd/*||key == nullptr*/|| key.empty())return(false);

		/*if(node>=LESS_GAME_KEY)
			return GameKeySet(node,dfs_key_node_name[node]+key,value);
		else*/
			return GameKeySet(dfs_key_node_name[node]+key,value);
	}

	bool GameKeyDelete(const GameDFSKey &node,const redis_string &key)			///key值为字符串
	{
        if(node<=gfkNone||node>=gfkEnd/*||key == nullptr*/|| key.empty())return(false);

		return GameKeyDelete(dfs_key_node_name[node]+key);
	}

//	bool GameKeyExpire(const GameDFSKey &node,const redis_string &key, int valid_time)			///key值为字符串
//	{
//        if(node<=gfkNone||node>=gfkEnd/*||key == nullptr*/|| key.empty())return(false);

//		return GameKeyExpire(dfs_key_node_name[node]+key,valid_time);
//	}

    uint32 GetExchangeCodeItem(const redis_string &code)
	{
		RedisDB *db=AcquireRedis();

		redis_string item_string;

		if(!db->Get(code,item_string))
		{
			ReleaseRedis(db);
			return(0);
		}

        uint32 item_id;

        //hgl::stou(item_string.c_str(),item_id);
        item_id=stoull(item_string);

		ReleaseRedis(db);
		return(item_id);
	}

	void DeleteExchangeCodeItem(const redis_string &code)
	{
		RedisDB *db=AcquireRedis();

		db->Del(code);

		ReleaseRedis(db);
	}
}//namespace showhand
