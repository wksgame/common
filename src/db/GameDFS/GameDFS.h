#ifndef SHOWHAND_SERVER_GAME_DFS_INCLUDE
#define SHOWHAND_SERVER_GAME_DFS_INCLUDE

#include"GameDFSID.h"
#include"GameDFSKey.h"
#include"GameDFSSet.h"
#include"../../io/DataInputStream.h"
#include"../../io/DataOutputStream.h"
#include"../../io/MemoryInputStream.h"
#include"../../io/MemoryOutputStream.h"

namespace showhand
{
	/**
	* 排行榜 DFS节点
	*/
// 	enum GameDFSRankNode:int
// 	{
// 		dfsRankLevel=0,			///<人物等级
// 	//	dfsRankCombatPower,		///<战力
// 	//	dfsRankConsortium,		///<公会
// 	//	dfsRankCharm,			///<魅力
// 		dfsRankWealth,			///<财富
// 		dfsRankAchivement,		///<成就
// 	//	dfsRankFight,			///<决斗
// 	//	dfsRankTransMonster,	///<守护
// 		dfsRankEnd
// 	};
#define GAMEDFSBUFFSIZE		10240

	enum GameDFSNode
	{
		gdnNone=0,

		gdnMail,					///<邮件(一封具体的邮件)

		gdnEnd
	};//enum GameDFSNode

	enum GameDFSMailNode
	{
		dfsMailPersonal=0,		//个人邮箱

		dfsMailEnd
	};//

	class GameDFSSave
	{
        redis_string name;
        char byteArr[GAMEDFSBUFFSIZE];
		kiss::MemoryOutputStream mos;

    public:

		kiss::DataOutputStream dos;

	public:

        GameDFSSave(const enum GameDFSNode &node,const int64 id);
		~GameDFSSave();

        bool Close();
	};//class GameDFSSave

	class GameDFSLoad
	{
        redis_string name;
        char byteArr[GAMEDFSBUFFSIZE];
		kiss::MemoryInputStream* mis;
        char *data;

    public:

		kiss::DataInputStream* dis;

	public:

        GameDFSLoad(const enum GameDFSNode &node,const int64 id);
		~GameDFSLoad();

		void Close();

        const bool operator !()const{return !data;}
	};//class GameDFSLoad

    bool GameDFSDelete(const enum GameDFSNode &,const int64);
}//namespace showhand
#endif//SHOWHAND_SERVER_GAME_DFS_INCLUDE
