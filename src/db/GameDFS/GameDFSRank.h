#ifndef SHOWHAND_SERVER_GAME_DFS_RANK_INCLUDE
#define SHOWHAND_SERVER_GAME_DFS_RANK_INCLUDE

#include<hgl/dfs/dfsClientRank.h>

using namespace hgl;
using namespace hgl::dfs;

namespace showhand
{
	/**
	* 排行榜 DFS节点
	*/
	enum GameDFSRankNode:int
	{
		dfsNone=0,
		dfsRankWealth,			///<财富排行
		dfsRankLevel,			///<等级排行
		dfsRankAchievement,		///<成就排行
		dfsRankBetSuccess,		///<押注赢排行
		dfsRankRecharge,		///<充值排行

		dfsRankEnd
	};

//	void GameRankCommitRecord(const GameDFSRankNode &node,hgl::int64 id,hgl::int64 value,void *,int);
	dfsClientRank *GetDFSClientRank(GameDFSRankNode index);
}//namespace showhand
#endif//SHOWHAND_SERVER_GAME_DFS_RANK_INCLUDE
