#include"GameDB.h"
#include<string>
#include<list>
#include"db/SQLiteInterface.h"

using namespace kiss::db;
using namespace std;
using namespace game;

//SQLiteInterface db;

unordered_map<long long, UserInfo*> all_users_by_id;
unordered_map<string, UserInfo*> all_users_by_name;
unordered_map<long long, RoleInfo*> all_roles_by_roleid;
unordered_map<long long, std::deque<RoleInfo*>> all_roles_by_userid;

namespace game
{
	bool InitGameDB()
	{
		DBInterface* db = new SQLiteInterface();

		if(!db->Open("gametest.db"))
			return false;

		// account--accountid|username||password
		if(!db->CreateTable("create table if not exists account(accountid INTEGER primary key AUTOINCREMENT, username TEXT UNIQUE, password TEXT)"))
			return false;

		// role--roleid|rolename|accountid|money|exp|level
		if(!db->CreateTable("create table if not exists role(roleid INTEGER primary key AUTOINCREMENT, rolename TEXT UNIQUE, accountid INTERGER, money INTERGER ,exp INTERGER, level INTERGER)"))
			return false;

		db->Close();
		delete db;

		return true;
	}

	GameDB::GameDB()
	{
		db = new SQLiteInterface();
		db->Open("gametest.db");
	}

	GameDB::~GameDB()
	{
		db->Close();
		delete db;
	}

	ACCOUNT_ID GameDB::CreateAccount(const char* username, const char* password)
	{
		const char* insertSQL = "insert into account (username, password) VALUES('%s','%s')";
		char sqlstr[1024] = {0};
		snprintf(sqlstr,1024, insertSQL, username, password);

		if(!db->Insert(sqlstr))
			return -1;

		ACCOUNT_ID account_id = ((SQLiteInterface*)db)->LastInsertRowID();
		return account_id;
	}

	UserInfo* GameDB::CheckAccount(const char* username, const char* password)
	{
		const char* selectSQL = "select * from account where username = '%s' and password = '%s'";
		char sqlstr[1024] = {0};
		snprintf(sqlstr,1024, selectSQL, username, password);

		auto user = new UserInfo();

		auto func = [](char** result, int nRow, int nCol, void* arg){
				auto user =(UserInfo*)arg;

				user->id = stoll(result[0]);
				user->username = result[1];
				user->password = result[2];

				all_users_by_id[user->id]=user;
				all_users_by_name[user->username]=user;
		};

		if(!((SQLiteInterface*)db)->Select(sqlstr, func, user))
			return nullptr;

		return user;
	}

	RoleInfo* GameDB::CreateRole(const char* rolename, const ACCOUNT_ID accountid)
	{
		int money = 0;
		int exp = 0;
		int level = 1;

		const char* insertSQL = "insert into role (rolename, accountid, money, exp, level) VALUES('%s', %d, %d, %d, %d)";
		char sqlstr[1024] = {0};
		snprintf(sqlstr,1024, insertSQL, rolename, accountid, money, exp, level);

		if (!db->Insert(sqlstr))
			return nullptr;

		auto role = new RoleInfo();

		role->roleID = ((SQLiteInterface*)db)->LastInsertRowID();
		role->rolename = rolename;
		role->accountID = accountid;
		role->money = money;
		role->exp = exp;
		role->level = level;

		return role;
	}

	void GameDB::GetRoleList(const ACCOUNT_ID accountid, std::vector< RoleInfo* >* roles)
	{
		const char* selectSQL = "select roleid,rolename,level from role where accountid = '%d'";
		char sqlstr[1024] = {0};
		snprintf(sqlstr,1024, selectSQL, accountid);

		auto func = [](char** result, int nRow, int nCol, void* arg){

			vector< game::RoleInfo* >* roles = (vector< game::RoleInfo* >*)arg;
			roles->resize(nRow);

			int index=0;
			for(int i=0; i<nRow; ++i)
			{
				(*roles)[i] = new RoleInfo();

				(*roles)[i]->roleID = stoll(result[index++]);
				(*roles)[i]->rolename = result[index++];
				(*roles)[i]->level = stoi(result[index++]);
			}
		};

		((SQLiteInterface*)db)->Select(sqlstr, func, roles);
	}

	RoleInfo* GameDB::GetRole(const ROLE_ID roleid)
	{
		const char* selectSQL = "select * from role where roleid = '%d'";
		char sqlstr[1024] = {0};
		snprintf(sqlstr,1024, selectSQL, roleid);

		auto func = [](char** result, int nRow, int nCol, void* arg){

			RoleInfo* roleinfo = (game::RoleInfo*)arg;

			if(nRow>0)
			{
				int index=0;
				roleinfo = new RoleInfo();

				roleinfo->roleID = stoll(result[index++]);
				roleinfo->rolename = result[index++];
				roleinfo->accountID = stoll(result[index++]);
				roleinfo->money = stoll(result[index++]);
				roleinfo->exp = stoll(result[index++]);
				roleinfo->level = stoi(result[index++]);
			}
		};

		RoleInfo* roleinfo = nullptr;
		if(!((SQLiteInterface*)db)->Select(sqlstr, func, roleinfo))
			return nullptr;
	}
}//namespace game

// void LoadGameDB()
// {
// 	LoadAllUser();
// 	LoadAllRole();
// }
//
// void LoadAllUser()
// {
// 	auto func = [](char** result, int nRow, int nCol, void* out){
// 		int index=0;
// 		for(int i=0; i<nRow; ++i)
// 		{
// 			auto u = new UserInfo();
//
// 			u->id = stoll(result[index++]);
// 			u->username = result[index++];
// 			u->password = result[index++];
//
// 			all_users_by_id[u->id]=u;
// 			all_users_by_name[u->username]=u;
// 		}
// 	};
//
// 	((SQLiteInterface*)db)->Select("select * from account", func, nullptr);
// }
//
// void LoadAllRole()
// {
// 	auto func = [](char** result, int nRow, int nCol, void* out){
// 		int index=0;
// 		for(int i=0; i<nRow; ++i)
// 		{
// 			auto role = new RoleInfo();
//
// 			role->roleID = stoll(result[index++]);
// 			role->rolename = result[index++];
// 			role->accountID = stoll(result[index++]);
// 			role->money = stoll(result[index++]);
// 			role->exp = stoll(result[index++]);
// 			role->level = stoi(result[index++]);
//
// 			all_roles_by_roleid[role->roleID] = role;
// 			all_roles_by_userid[role->accountID].push_back(role);
// 		}
// 	};
//
// 	((SQLiteInterface*)db)->Select("select * from role", func, nullptr);
// }