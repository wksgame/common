#ifndef GAME_DFS_SET_INCLUDE
#define GAME_DFS_SET_INCLUDE

#include"../../defines.h"
#include"../RedisClient/RedisDB.h"
#include<set>

typedef string redis_string;
typedef list<string> redis_string_list;

namespace showhand
{
	/**
	* 合集节点，用于保存不可重复数据
	*/
	enum GameDFSSetNode
	{
		gdsNone=0,

		gdsFriendIDList,				///<好友ID列表
		gdsFriendRequest,				///<他人向自己申请好友的列表
		gdsSelfFriendRequest,			///<自己向他人申请好友的列表
		gdsDailyMissionFinish,			///<每日任务完成
		gdsDailyMissionReward,			///<每日任务领取了奖励
		gdsNormalMissionFinish,			///<一般任务完成
		gdsNormalMissionReward,			///<一般任务领取了奖励
		gdsAchivementMissionFinish,		///<成就任务完成
		gdsAchivementMissionReward,		///<成就任务领取了奖励

		gdsReceiveMail,					///<接收邮件列表
		gdsDeleteMail,					///<删除邮件列表
		gdsSendMail,					///<发送邮件列表
		gdsNewMail,						///<新邮件列表(目前GM用GM工具发送时会放入其中)

		gdsEnd
	};//enum GameDFSSetNode

	extern const char *dfs_set_node_name[];

    bool GameSetLoad(const redis_string &,redis_string_list &);
    bool GameSetLoad(const GameDFSSetNode &node,const int64 id,redis_string_list &);

	template<typename T>
    bool GameSetLoadInteger(const redis_string &node_name,set<T> &sl)
	{
        redis_string_list ul;

        sl.clear();

		if(!GameSetLoad(node_name,ul))
			return(false);

//        const int count=ul.size();

//		for(int i=0;i<count;i++)
//		{
//			T value;

//            hgl::stoi(ul[i].c_str(),value);

//			sl.Add(value);
//		}

        for(auto i:ul)
        {
            T value;
            value=stoll(i);
            sl.insert(value);
        }

		return(true);
	}

	template<typename T>
    bool GameSetLoadUInteger(const redis_string &node_name,set<T> &sl)
	{
        redis_string_list ul;

        sl.clear();

		if(!GameSetLoad(node_name,ul))
			return(false);

//		const int count=ul.GetCount();

//		for(int i=0;i<count;i++)
//		{
//			T value;

//			hgl::stou(ul[i].c_str(),value);

//			sl.Add(value);
//		}

        for(auto i:ul)
        {
            T value;
            value=stoll(i);
            sl.insert(value);
        }

		return(true);
	}

	template<typename T>
    bool GameSetLoadInteger(const GameDFSSetNode &node,const int64 id,set<T> &sl)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)return(false);

        return GameSetLoadInteger<T>(dfs_set_node_name[node]+to_string(id),sl);
	}

	template<typename T>
    bool GameSetLoadUInteger(const GameDFSSetNode &node,const int64 id,set<T> &sl)
	{
		if(node<=gdsNone||node>=gdsEnd||id<=0)return(false);

        return GameSetLoadUInteger<T>(dfs_set_node_name[node]+to_string(id),sl);
	}

	bool GameSetAppend	(const redis_string &,const redis_string &member);
	bool GameSetRemove	(const redis_string &,const redis_string &member);
	bool GameSetCheck	(const redis_string &,const redis_string &member);

	bool GameSetAppend	(const GameDFSSetNode &node,const int64 id,const redis_string &member);
	bool GameSetRemove	(const GameDFSSetNode &node,const int64 id,const redis_string &member);
	bool GameSetCheck	(const GameDFSSetNode &node,const int64 id,const redis_string &member);
	bool GameSetDelete	(const GameDFSSetNode &node,const int64 id);							///<删除Set
	int  GameSetGetCount(const GameDFSSetNode &node,const int64 id);							///<获取Set中元素个数

	/**
	* 游戏数据合集
	*/
	template<typename T> class game_set
	{
		bool init;
		GameDFSSetNode node;
		int64 id;
		redis_string node_name;

        set<T> set_data;

	protected:

		bool load();

	public:

		game_set(const GameDFSSetNode n,int64 i)
		{
			LoadData(n,i);
		}

		game_set()
		{
		}

		bool LoadData(const GameDFSSetNode n,int64 i)
		{
			node=n;
			id=i;

			if(node<=gdsNone||node>=gdsEnd||id<=0)
			{
				init=false;
				return(false);
			}

			node_name=dfs_set_node_name[node]+redis_string(id);
            set_data.ClearData();
			init=true;
			return load();
		}

        const int 	Find(const T value)const{return set_data.Find(value);}

        const T *	GetData()const{return set_data.GetData();}
        const int	GetCount()const{return set_data.GetCount();}
        const bool	IsMember(const T v)const{return set_data.IsMember(v);}

        T operator[](int index)const{return set_data[index];}

        operator const set<T> &()const{return set_data;}

		bool Add(const T value)
		{
            if(set_data.Add(value)==-1)return(false);

			if(init)
				GameSetAppend(node_name,value);

			return(true);
		}

		bool Delete(const T value)
		{
            if(!set_data.Delete(value))return(false);

			if(init)
				GameSetRemove(node_name,value);

			return(true);
		}
	};//class game_set:public Set<T>

	typedef game_set<int32> game_set_int32;
	typedef game_set<int64> game_set_int64;
	typedef game_set<uint32> game_set_uint32;
	typedef game_set<uint64> game_set_uint64;
}
#endif//GAME_DFS_SET_INCLUDE
