#ifndef GAME_DB_H
#define GAME_DB_H

#include<string>
#include<unordered_map>
#include<deque>
#include<vector>

#include"Player.h"

// 创建数据表
bool InitGameDB();

// 添加数据
bool CreateUser(const char* username, const char* password);
game::UserInfo* GetUser(const char* username, const char* password);

game::RoleInfo* CreateRole(const char* rolename, const long long userID);
void GetRole(const long long userID, std::vector<game::RoleInfo*>* roles, int &count);

// 加载数据
void LoadGameDB();
void LoadAllUser();
void LoadAllRole();

extern std::unordered_map<long long, game::UserInfo*> all_users_by_id;
extern std::unordered_map<std::string, game::UserInfo*> all_users_by_name;
extern std::unordered_map<long long, game::RoleInfo*> all_roles_by_roleid;
extern std::unordered_map<long long, std::deque<game::RoleInfo*>> all_roles_by_userid;

#endif//GAME_DB_H