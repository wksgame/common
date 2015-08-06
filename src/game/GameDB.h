#ifndef GAME_DB_H
#define GAME_DB_H

#include<string>
#include<unordered_map>
#include<deque>
#include<vector>

#include"gamedefines.h"
#include"Player.h"

namespace kiss
{
	namespace db
	{
		class DBInterface;
	}
}

namespace game
{
	// 创建数据表
	bool InitGameDB();

	class GameDB
	{
		kiss::db::DBInterface* db;
	public:

		GameDB();
		~GameDB();

		ACCOUNT_ID CreateAccount(const char* username, const char* password);
		game::UserInfo* CheckAccount(const char* username, const char* password);

		game::RoleInfo* CreateRole(const char* rolename, const ACCOUNT_ID accountid);
		void GetRoleList(const ACCOUNT_ID accountid, std::vector<game::RoleInfo*>* roles);
		game::RoleInfo* GetRole(const ROLE_ID roleid);
	};
}
#endif//GAME_DB_H