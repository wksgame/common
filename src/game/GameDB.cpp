#include"GameDB.h"
#include<string>
#include<list>
#include"db/SQLiteInterface.h"

using namespace kiss::db;
using namespace std;
using namespace game;

//SQLiteInterface db;
DBInterface* db;

unordered_map<long long, UserInfo*> all_users_by_id;
unordered_map<string, UserInfo*> all_users_by_name;
unordered_map<long long, RoleInfo*> all_roles_by_roleid;
unordered_map<long long, std::deque<RoleInfo*>> all_roles_by_userid;

bool InitGameDB()
{
	db = new SQLiteInterface();

	if(!db->Open("gametest.db"))
		return false;

	if(!db->CreateTable("create table if not exists account(id INTEGER primary key AUTOINCREMENT, username TEXT UNIQUE, password TEXT)"))
		return false;

	if(!db->CreateTable("create table if not exists role(roleid INTEGER primary key AUTOINCREMENT, rolename TEXT UNIQUE, userid INTERGER, money INTERGER ,exp INTERGER, level INTERGER)"))
		return false;

	return true;
}

bool CreateUser(const char* username, const char* password)
{
	const char* insertSQL = "insert into account (username, password) VALUES('%s','%s')";
	char sqlstr[1024] = {0};
	snprintf(sqlstr,1024, insertSQL, username, password);

	if(!db->Insert(sqlstr))
		return false;

	auto user = new UserInfo();

	user->id = ((SQLiteInterface*)db)->LastInsertRowID();
	user->username = username;
	user->password = password;

	all_users_by_id[user->id]=user;
	all_users_by_name[user->username]=user;

	return true;
}

UserInfo* GetUser(const char* username, const char* password)
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

RoleInfo* CreateRole(const char* rolename, const long long userID)
{
	int money = 0;
	int exp = 0;
	int level = 1;

	const char* insertSQL = "insert into role (rolename, userid, money, exp, level) VALUES('%s', %d, %d, %d, %d)";
	char sqlstr[1024] = {0};
	snprintf(sqlstr,1024, insertSQL, rolename, userID, money, exp, level);

	if (!db->Insert(sqlstr))
		return nullptr;

	auto role = new RoleInfo();

	role->roleID = ((SQLiteInterface*)db)->LastInsertRowID();
	role->rolename = rolename;
	role->userID = userID;
	role->money = 0;
	role->exp = 0;
	role->level = 1;

	all_roles_by_roleid[role->roleID] = role;
	all_roles_by_userid[role->userID].push_back(role);

	return role;
}

void GetRole(const long long int userID, vector< game::RoleInfo* >* roles, int& count)
{
	const char* selectSQL = "select * from role where userid = '%d'";
	char sqlstr[1024] = {0};
	snprintf(sqlstr,1024, selectSQL, userID);
	count = 0;
	
	auto func = [](char** result, int nRow, int nCol, void* arg){
		
		vector< game::RoleInfo* >* roles = (vector< game::RoleInfo* >*)arg;

		int index=0;
		for(int i=0; i<nRow; ++i)
		{
			(*roles)[i] = new RoleInfo();

			(*roles)[i]->roleID = stoll(result[index++]);
			(*roles)[i]->rolename = result[index++];
			(*roles)[i]->userID = stoll(result[index++]);
			(*roles)[i]->money = stoll(result[index++]);
			(*roles)[i]->exp = stoll(result[index++]);
			(*roles)[i]->level = stoi(result[index++]);

			all_roles_by_roleid[(*roles)[i]->roleID] = (*roles)[i];
			all_roles_by_userid[(*roles)[i]->userID].push_back((*roles)[i]);
		}
	};

	if(!((SQLiteInterface*)db)->Select(sqlstr, func, roles))
		count = 0;
}

void LoadGameDB()
{
	LoadAllUser();
	LoadAllRole();
}

void LoadAllUser()
{
	auto func = [](char** result, int nRow, int nCol, void* out){
		int index=0;
		for(int i=0; i<nRow; ++i)
		{
			auto u = new UserInfo();

			u->id = stoll(result[index++]);
			u->username = result[index++];
			u->password = result[index++];

			all_users_by_id[u->id]=u;
			all_users_by_name[u->username]=u;
		}
	};

	((SQLiteInterface*)db)->Select("select * from account", func, nullptr);
}

void LoadAllRole()
{
	auto func = [](char** result, int nRow, int nCol, void* out){
		int index=0;
		for(int i=0; i<nRow; ++i)
		{
			auto role = new RoleInfo();

			role->roleID = stoll(result[index++]);
			role->rolename = result[index++];
			role->userID = stoll(result[index++]);
			role->money = stoll(result[index++]);
			role->exp = stoll(result[index++]);
			role->level = stoi(result[index++]);

			all_roles_by_roleid[role->roleID] = role;
			all_roles_by_userid[role->userID].push_back(role);
		}
	};

	((SQLiteInterface*)db)->Select("select * from role", func, nullptr);
}