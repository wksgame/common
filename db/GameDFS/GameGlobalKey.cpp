#include"GameGlobalKey.h"
#include"../../defines.h"

namespace showhand
{
	const char *dfs_global_key_node_name[32]=
	{
		"",

		"gk_sm",				///<拉霸彩金
		"gk_b",					///<全服广播ID

		"",
	};

	template<> bool GameGlobalKeyValue<int32>::load(){return GameGlobalKeyGetInteger(key,value);}
	template<> bool GameGlobalKeyValue<int64>::load(){return GameGlobalKeyGetInteger(key,value);}
	template<> bool GameGlobalKeyValue<uint32>::load(){return GameGlobalKeyGetUInteger(key,value);}
	template<> bool GameGlobalKeyValue<uint64>::load(){return GameGlobalKeyGetUInteger(key,value);}

	bool GameGlobalKeyGet(const GameGlobalKey &node,redis_string &value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		RedisDB *db=AcquireRedis();

		bool result=db->Get(dfs_global_key_node_name[node],value);

		ReleaseRedis(db);
		return(result);
	}

	bool GameGlobalKeySet(const GameGlobalKey &node,const redis_string &value)
	{
		if(node<=ggkNone||node>=ggkEnd)return(false);

		RedisDB *db=AcquireRedis();

		bool result=db->Set(dfs_global_key_node_name[node],value);

		ReleaseRedis(db);
		return(result);
	}
}//namespace showhand
