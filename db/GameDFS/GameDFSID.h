#ifndef SHOWHAND_SERVER_GAME_DFS_ID_INCLUDE
#define SHOWHAND_SERVER_GAME_DFS_ID_INCLUDE

#include<string>
typedef std::string redis_string;
typedef long long int int64;

namespace showhand
{
	/**
	* ID DFS节点
	*/
	enum GameDFSIDNode
	{
		dfsIDAccount=0,			///<帐号
		dfsIDMail,				///<邮件

		dfsIDEnd
	};

	int64	GameIDAcquire	(const GameDFSIDNode &);													///<请求一个新的ID
	bool	GameIDAcquire	(const GameDFSIDNode &,const redis_string &,int64 &);							///<请求一个新的ID
	bool	GameIDSet		(const GameDFSIDNode &,const redis_string &,const int64);						///<设置一个Key的ID
	int64	GameIDGet		(const GameDFSIDNode &,const redis_string &);									///<取得指定Key的ID

	bool	GameIDGetKey	(const GameDFSIDNode &,const int64,redis_string &);							///<取得指定ID的Key

	bool	GameIDLogin		(const GameDFSIDNode &,const redis_string &,int64 &,bool &);

	bool	GameIDDelete	(const GameDFSIDNode &,const redis_string &);
}//namespace showhand
#endif//SHOWHAND_SERVER_GAME_DFS_ID_INCLUDE
