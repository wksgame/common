#include"GameDFSHashs.h"

using namespace hgl;

namespace showhand
{
	RedisDB *AcquireRedis();
	void ReleaseRedis(RedisDB *db);

	const char *game_dfs_hashs_node[]=
	{
		"BagConsume",
		"BagMaterial",
		"BagEquip",
		"BagOther",

		"RoleEquip",
		"PetEquip",

		"BagExtra",
		"BagGevral"
	};//const char *game_dfs_hashs_node[]=

	bool GameHashs::Init(const GameDFSHashsNode n,const int64 i)
	{
		node=n;
		node_name=UTF8String(game_dfs_hashs_node[n])+':'+UTF8String(i);
		id=i;
	}

	/**
	* 加载所有数据
	* @return >=0 数据个数
	* @return <0 错误
	*/
	int GameHashs::Load()
	{
		items.ClearData();

		RedisDB *db=AcquireRedis();

		if(!db)
			RETURN_ERROR(-1);

		const int count=db->HGetAll(node_name,items);

		ReleaseRedis(db);

		return(count);
	}

	// int GameHashs::Load(const UTF8String &)
	// {
	// }
	//
	// int GameHashs::Load(const UTF8StringList &)
	// {
	// }

	/**
	* 删除一个指定字段
	* @param field 字段名称
	*/
	bool GameHashs::Delete(const UTF8String &field)
	{
		RedisDB *db=AcquireRedis();

		if(!db)
			RETURN_ERROR(-1);

		const bool count=db->HDel(node_name,field);

		ReleaseRedis(db);

		return(count);
	}

	/**
	* 删除多个指定字段
	* @param field_list 字段列表
	*/
	bool GameHashs::Delete(const UTF8StringList &field_list)
	{
		RedisDB *db=AcquireRedis();

		if(!db)
			RETURN_ERROR(-1);

		const bool count=db->HDel(node_name,field_list);

		ReleaseRedis(db);

		return(count);
	}

	/**
	* 保存所有变动过的数据
	* @return >=0 成功保存的数据个数
	* @return <0 出错
	*/
	int GameHashs::Save()
	{
		RedisDB *db=AcquireRedis();

		if(!db)
			RETURN_ERROR(-1);

		const int count=db->HSetUpdate(node_name,items);

		ReleaseRedis(db);

		return(count);
	}
}//namespace showhand
