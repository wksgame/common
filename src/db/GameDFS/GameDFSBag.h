#ifndef SHOWHAND_GAMEDFS_BAG_INCLUDE
#define SHOWHAND_GAMEDFS_BAG_INCLUDE

#include<map>
#include<string>

typedef long long int int64;

namespace showhand
{
	enum GameDFSBagNode
	{
		gdbnBag=0,					//主背包

		gdbnEnd
	};//

    using BagItemList=std::map<int64,int64>;

	class GameDFSBag
	{
		int64 user_id;
        std::string nodename;

	public:

		GameDFSBag(const GameDFSBagNode &,const int64 user_id);

		void ClearZero();									//清除数量为0的物品

		bool Load(BagItemList &item_list);					//完整加载物品列表(注：此命令执行前，会自动调一次ClearZero，以保证不会取得数量为0的物品)

		int  GetCount(int64 item_id);						//取得一个物品的数量
		bool Inc(int64 item_id,const int64 count,int64 &);	//增加一个物品的数量
		bool Dec(int64 item_id,const int64 count,int64 &);	//减少一个物品的数量
	};//class GameDFSBag
}//namespace showhand
#endif//SHOWHAND_GAMEDFS_BAG_INCLUDE
