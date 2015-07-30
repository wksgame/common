#include"GameDFSID.h"
#include"../RedisClient/RedisDB.h"

using namespace hgl;
using namespace hgl::redis;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	namespace
	{
		const char *dfs_id_node_name[]=
		{
			"Account",
			"Mail",

			""
		};
	}//namespace

	int64 GameIDAcquire	(const GameDFSIDNode &node)
	{
		if(node<dfsIDAccount||node>=dfsIDEnd)return(-1);

		RedisDB *db=AcquireRedis();

		int64 id;

		if(!db->Incr(redis_string("ID_")+dfs_id_node_name[node],id))
			return(-1);

		ReleaseRedis(db);

		return id;
	}

	bool GameIDAcquire(const GameDFSIDNode &node,const redis_string &key,int64 &id)
	{
		if(node<dfsIDAccount||node>=dfsIDEnd)return(false);
		if(key.empty())return(false);

		RedisDB *db=AcquireRedis();

		if(db->ZScore(redis_string("Set_")+dfs_id_node_name[node],key,id))
		{
			ReleaseRedis(db);
			return(false);
		}

        if(!db->Incr(redis_string("ID_")+dfs_id_node_name[node],id))
			return(false);

        const int64 result=db->ZAdd(redis_string("Set_").append(dfs_id_node_name[node]).c_str(),id,key.c_str());

		ReleaseRedis(db);
        return result!=-1;
	}

	bool GameIDSet		(const GameDFSIDNode &node,const redis_string &key,const int64 id)
	{
		if(node<dfsIDAccount||node>=dfsIDEnd)return(false);
		if(key.empty())return(false);

		RedisDB *db=AcquireRedis();

		int64 old_id;

		if(!db->ZScore(redis_string("Set_")+dfs_id_node_name[node],key,old_id))
		{
			ReleaseRedis(db);
			return(false);
		}

        const bool result=db->ZAdd(redis_string("Set_").append(dfs_id_node_name[node]).c_str(),id,key.c_str());

		ReleaseRedis(db);
		return result;
	}

	int64 GameIDGet		(const GameDFSIDNode &node,const redis_string &key)
	{
		if(node<dfsIDAccount||node>=dfsIDEnd)return(-1);
		if(key.empty())return(-1);

		RedisDB *db=AcquireRedis();

		int64 id=-1;

        db->ZScore(redis_string("Set_").append(dfs_id_node_name[node]).c_str(),key,id);

		ReleaseRedis(db);
		return id;
	}

	bool GameIDGetKey(const GameDFSIDNode &node,const int64 id,redis_string &key)
	{
		if(node<dfsIDAccount||node>=dfsIDEnd)return(false);

		RedisDB *db=AcquireRedis();

        const bool result=db->ZRangeByScore(redis_string("Set_").append(dfs_id_node_name[node]).c_str(),id,key);

		ReleaseRedis(db);
		return(result);
	}

	bool GameIDLogin(const GameDFSIDNode &node,const redis_string &key,int64 &id,bool &new_id)
	{
		if(node<dfsIDAccount||node>=dfsIDEnd)return(false);
		if(key.empty())return(false);

		RedisDB *db=AcquireRedis();

		const redis_string set_name=redis_string("Set_")+dfs_id_node_name[node];

		if(db->ZScore(set_name,key,id))		//成功取到了
		{
			new_id=false;

			ReleaseRedis(db);
			return(true);
		}

		new_id=true;

		//没取到，那创建新的
        if(!db->Incr(redis_string("ID_").append(dfs_id_node_name[node]).c_str(),id))		//得到新的id
			return(false);

        const bool result=db->ZAdd(set_name.c_str(),id,key.c_str());					//添加到合集

		ReleaseRedis(db);
		return(result);
	}

	bool GameIDDelete(const GameDFSIDNode &node,const redis_string &key)
	{
		if(node<dfsIDAccount||node>=dfsIDEnd)return(false);
		if(key.empty())return(false);

		RedisDB *db=AcquireRedis();

        const bool result=db->ZRem(redis_string("Set_").append(dfs_id_node_name[node]),key);

		ReleaseRedis(db);
		return(result);
	}
}//namespace showhand
