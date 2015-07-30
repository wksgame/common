#include"GameDFSSet.h"
#include"../RedisClient/RedisDB.h"

using namespace hgl;
using namespace hgl::redis;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	const char *dfs_set_node_name[]=
	{
		"",

		"s_fil:",			///<FriendIDList
		"s_fr:",			///<FriendRequest
		"s_sfr:",			///<SelfFriendRequest
		"s_dmf:",			///<DailyMissionFinish
		"s_dmr:",			///<DailyMissionReward
		"s_nmf:",			///<NormalMissionFinish
		"s_nmr:",			///<NormalMissionReward
		"s_amf:",			///<AchivementMissionFinish
		"s_amr:",			///<AchivementMissionReward

		"s_rm:",			///<ReceiveMail
		"s_dm:",			///<DeleteMail
		"s_sm:",			///<SendMail
		"s_nm:",			///<NewMail

		""
	};

    template<> bool game_set<int32>::load(){return GameSetLoadInteger(node_name,set_data);}
    template<> bool game_set<int64>::load(){return GameSetLoadInteger(node_name,set_data);}
    template<> bool game_set<uint32>::load(){return GameSetLoadUInteger(node_name,set_data);}
    template<> bool game_set<uint64>::load(){return GameSetLoadUInteger(node_name,set_data);}

    bool GameSetLoad	(const redis_string &node_name,redis_string_list &ul)
	{
		if(node_name.empty())return(false);

		RedisDB *db=AcquireRedis();

        const bool result=db->SMembers(node_name.c_str(),ul);

		ReleaseRedis(db);

		return(result);
	}

	bool GameSetAppend	(const redis_string &node_name,const redis_string &member)
	{
		if(node_name.empty()||member.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->SAdd(node_name,member);

		ReleaseRedis(db);
		//return(result);
		return(result);
	}

	bool GameSetRemove	(const redis_string &node_name,const redis_string &member)
	{
		if(node_name.empty()||member.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->SRem(node_name,member);

		ReleaseRedis(db);
		return(result);
	}

	bool GameSetCheck	(const redis_string &node_name,const redis_string &member)
	{
		if(node_name.empty()||member.empty())return(false);

		RedisDB *db=AcquireRedis();

		const bool result=db->SIsMember(node_name,member);

		ReleaseRedis(db);
		return(result);
	}

    bool GameSetLoad	(const GameDFSSetNode &node,const int64 id,redis_string_list &ul)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)return(false);

        return GameSetLoad(dfs_set_node_name[node]+to_string(id),ul);
	}

	bool GameSetAppend	(const GameDFSSetNode &node,const int64 id,const redis_string &member)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)return(false);

        return GameSetAppend(dfs_set_node_name[node]+to_string(id),member);
	}

	bool GameSetRemove	(const GameDFSSetNode &node,const int64 id,const redis_string &member)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)return(false);

        return GameSetRemove(dfs_set_node_name[node]+to_string(id),member);
	}

	bool GameSetCheck	(const GameDFSSetNode &node,const int64 id,const redis_string &member)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)return(false);

        return GameSetCheck(dfs_set_node_name[node]+to_string(id),member);
	}

	bool GameSetDelete(const GameDFSSetNode &node,const int64 id)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)
			return(false);

        redis_string name=redis_string(dfs_set_node_name[node])+to_string(id);

		RedisDB *db=AcquireRedis();
		db->Del(name);
		ReleaseRedis(db);

		return(true);
	}

	int GameSetGetCount(const GameDFSSetNode& node, const int64 id)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)
			return 0;

		int count=0;
        redis_string name=redis_string(dfs_set_node_name[node])+to_string(id);

		RedisDB *db=AcquireRedis();
        count=db->SCard(name.c_str());
		ReleaseRedis(db);

		return count;
	}

}//namespace showhand
