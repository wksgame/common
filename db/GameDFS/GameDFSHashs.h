#ifndef SHOWHAND_SERVER_GAME_DFS_HASHS_INCLUDE
#define SHOWHAND_SERVER_GAME_DFS_HASHS_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/io/MemoryInputStream.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/type/Map.h>
#include<hgl/type/StringList.h>
#include<hgl/db/RedisDB.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::redis;

namespace showhand
{
	enum GameDFSHashsNode
	{
		ghnBagConsume=0,		///<背包：消费品
		ghnBagMaterial,			///<背包：材料
		ghnBagEquip,			///<背包：装备
		ghnBagOther,			///<背包：其它

		ghnRoleEquip,			///<角色装备
		ghnPetEquip,			///<角色装备

		ghnBagExtra,			///<背包：仓库
		ghnBagGevral,			///<背包：祈福
	};//enum GameDFSHashNode

	/**
	* 游戏Hashs数据管理类
	*/
	class GameHashs
	{
		GameDFSHashsNode node;
		int64 id;
		UTF8String node_name;

		redis_hashs_item_list items;

	public:

		GameHashs()
		{
			id=0;
		}

		bool Init(const GameDFSHashsNode n,const int64 i);

		int Load();																	///<加载所有数据(注：这个操作会清除所有数据再加载)

	// 	int Load(const UTF8String &);												///<加载一个数据(注：这个操作不会清除现有数据)
	// 	int Load(const UTF8StringList &);											///<加载多个数据(注：这个操作不会清除现有数据)

		bool Delete(const UTF8String &);											///<删除一个数据
		bool Delete(const UTF8StringList &);										///<删除多个数据

		int Save();																	///<保存数据

	public:

		DataInputStream *Get(const UTF8String &name)
		{
			HashsItem *hi=items[name];

			if(!hi)return(nullptr);

			return hi->GetDIS();
		}

		DataOutputStream *Put(const UTF8String &name)
		{
			HashsItem *hi=items[name];

			if(!hi)
			{
				hi=new HashsItem();

				items.Add(name,hi);
			}

			hi->update=true;		//变动过了

			return hi->GetDOS();
		}
	};//class GameHashs
}//namespace showhand
#endif//SHOWHAND_SERVER_GAME_DFS_HASHS_INCLUDE
