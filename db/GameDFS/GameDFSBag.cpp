#include"GameDFSBag.h"
#include"../RedisClient/RedisDB.h"

using namespace hgl;
using namespace hgl::redis;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	const char *game_dfs_bag_node[]=
	{
		"bag",
	};

	GameDFSBag::GameDFSBag(const GameDFSBagNode &node,const int64 uid)
	{
		if(node<gdbnBag||node>=gdbnEnd)
		{
			user_id=-1;
			return;
		}

		user_id=uid;

        nodename=string(game_dfs_bag_node[node])+':'+to_string(user_id);
	}

	void GameDFSBag::ClearZero()						//清除数量为0的物品
	{
		if(user_id<0)return;

		RedisDB *db=AcquireRedis();

		if(!db)return;

        db->ZRemRangeByScore(nodename.c_str(),0,0);

		ReleaseRedis(db);
	}

	bool GameDFSBag::Load(BagItemList &item_list)		//完整加载物品列表(注：此命令执行前，会自动调一次ClearZero，以保证不会取得数量为0的物品)
	{
		if(user_id<0)return(false);

		RedisDB *db=AcquireRedis();

		if(!db)return(false);

		redis_string_list sl;

        if(!db->ZAllMember(nodename.c_str(),sl))
		{
			ReleaseRedis(db);
			return(false);
		}

        for(int i=0;i<sl.size();i++)
		{
			int64 key;
			int64 value;

			sl[i].ToInt(key);

			db->ZScore(nodename,sl[i],value);

			item_list.Add(key,value);
		}

		ReleaseRedis(db);
		return(true);
	}

	int  GameDFSBag::GetCount(int64 item_id)			//取得一个物品的数量
	{
		if(user_id<0)return(-1);

		RedisDB *db=AcquireRedis();

		if(!db)return(-1);

		int64 value;

		if(!db->ZScore(nodename,item_id,value))
			value=0;

		ReleaseRedis(db);
		return(value);
	}

	bool GameDFSBag::Inc(int64 item_id,const int64 count,int64 &new_value)		//增加一个物品的数量
	{
		if(user_id<0)return(false);
		if(count<=0)return(false);

		RedisDB *db=AcquireRedis();

		if(!db)return(false);

		UTF8String result;

		if(db->ZInc(nodename,item_id,count,result))
		{
			result.ToInt(new_value);
			ReleaseRedis(db);
			return(true);
		}

		ReleaseRedis(db);
		return(false);
	}

	bool GameDFSBag::Dec(int64 item_id,const int64 count,int64 &new_value)		//减少一个物品的数量
	{
		if(user_id<0)return(false);
		if(count<=0)return(false);

		RedisDB *db=AcquireRedis();

		if(!db)return(false);

		int64 value;

		if(db->ZScore(nodename,item_id,value))
		{
			if(value>=count)
			{
				UTF8String result;

				if(db->ZInc(nodename,item_id,-count,result))
				{
					result.ToInt(new_value);
					ReleaseRedis(db);
					return(true);
				}
			}
		}

		ReleaseRedis(db);
		return(false);

	}
}//namespace showhand
