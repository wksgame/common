#ifndef PLAYER_H
#define PLAYER_H

#include<string>
#include<unordered_map>

namespace kiss
{
	class DataInputStream;
	class DataOutputStream;
}

namespace game
{
	struct UserInfo
	{
		long long id;
		std::string username;
		std::string password;
	};

	struct RoleInfo
	{
		std::string rolename;
		long long roleID;
		long long userID;
		long long money;
		long long exp;
		int level;

		bool Read(kiss::DataInputStream& dis);
		bool Write(kiss::DataOutputStream& dos)const;
	};

	struct Player
	{
		std::string name;
		int id;
		int hp;
		int damage;
		int defend;
		int hit;
		int dodge;
		int critical;

		Player();
		Player(std::string name);
		Player(std::string name, const int id, const int hp, const int damage, const int defend, const int hit, const int dodge, const int critical);

		bool Read(kiss::DataInputStream& dis);
		bool Write(kiss::DataOutputStream& dos)const;
	};

	std::ostream& operator<<(std::ostream& os, const Player& p);

	extern std::unordered_map<int, Player*> g_allPlayers;
}
#endif//PLAYER_H