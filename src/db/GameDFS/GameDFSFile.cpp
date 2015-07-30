#include"GameDFS.h"
#include"../RedisClient/RedisDB.h"

using namespace hgl;
using namespace hgl::redis;
using namespace kiss;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	namespace
	{
		const char *dfs_file_node_name[]=
		{
			"",

			"Mail",

			""
		};//
	}//namespace

    GameDFSSave::GameDFSSave(const enum GameDFSNode &node,const int64 id):mos(byteArr,GAMEDFSBUFFSIZE),dos(mos)
    {
		if(node<=gdnNone||node>=gdnEnd||id<=0)
			return;

        name=std::string(dfs_file_node_name[node])+':'+to_string(id);
	}

	GameDFSSave::~GameDFSSave()
    {
	}

	bool GameDFSSave::Close()
    {
		RedisDB *db=AcquireRedis();

		const bool result=db->SetBinary(name,byteArr, mos.cur_pos());

		ReleaseRedis(db);
		return(result);
	}

    GameDFSLoad::GameDFSLoad(const enum GameDFSNode &node,const int64 id)
    {
        data=nullptr;

		if(node<=gdnNone||node>=gdnEnd||id<=0)
			return;

        name=string(dfs_file_node_name[node])+':'+to_string(id);

		RedisDB *db=AcquireRedis();

        int size=0;
        data=(char*)db->GetBinary(name,size);

        mis = new MemoryInputStream(data, size);
		dis = new DataInputStream(*mis);

		ReleaseRedis(db);
	}

	GameDFSLoad::~GameDFSLoad()
	{
		Close();
	}

	void GameDFSLoad::Close()
    {
        if(data)
            delete[] data;
        data=nullptr;
	}

	bool GameDFSDelete(const enum GameDFSNode &node,const int64 id)
	{
		if(node<=gdnNone||node>=gdnEnd||id<=0)
			return(false);

        string name=string(dfs_file_node_name[node])+':'+to_string(id);

		RedisDB *db=AcquireRedis();
		db->Del(name);
		ReleaseRedis(db);

		return(true);
	}
}//namespace showhand
