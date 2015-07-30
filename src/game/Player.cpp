#include "Player.h"
#include"io/DataOutputStream.h"
#include"io/DataInputStream.h"

using namespace std;
using namespace kiss;

unordered_map<int, Player*> g_allPlayers;

bool RoleInfo::Write(DataOutputStream& dos)const
{
	if(dos<<rolename)
	if(dos<<roleID)
	if(dos<<userID)
	if(dos<<money)
	if(dos<<exp)
	if(dos<<level)
		return true;

	return false;
}

bool RoleInfo::Read(DataInputStream& dis)
{
	if(dis>>rolename)
	if(dis>>roleID)
	if(dis>>userID)
	if(dis>>money)
	if(dis>>exp)
	if(dis>>level)
		return true;

	return false;
}

Player::Player()
{
	hp = 1000;
	damage = 50;
	defend = 30;
	hit = 100;
	dodge = 20;
	critical = 15;
}

Player::Player(string name):name(name)
{
	hp = 1000;
	damage = 50;
	defend = 30;
	hit = 100;
	dodge = 20;
	critical = 15;
}

Player::Player(std::string name, const int id, const int hp, const int damage, const int defend, const int hit, const int dodge, const int critical):name(name)
{
	this->id = id;
	this->hp = hp;
	this->damage = damage;
	this->defend = defend;
	this->hit = hit;
	this->dodge = dodge;
	this->critical = critical;
}

// ostream& operator<<(ostream& os, const Player& p)
// {
// 	os<<"name:"<<p.name<<" hp:"<<p.hp<<" damage:"<<p.damage<<" defend:"<<p.defend<<" hit:"<<p.hit<<" dodge:"<<p.dodge<<" critical:"<<p.critical;
// 	return os;
// }
//
// istream& operator>>(istream& is, Player& p)
// {
// 	is>>p.name>>p.hp>>p.damage>>p.defend>>p.hit>>p.dodge>>p.critical;
// 	return is;
// }

bool Player::Write(DataOutputStream& dos)const
{
	if(dos<<name)
	if(dos<<id)
	if(dos<<hp)
	if(dos<<damage)
	if(dos<<defend)
	if(dos<<hit)
	if(dos<<dodge)
	if(dos<<critical)
		return true;

	return false;
}

bool Player::Read(DataInputStream& dis)
{
	if(dis>>name)
	if(dis>>id)
	if(dis>>hp)
	if(dis>>damage)
	if(dis>>defend)
	if(dis>>hit)
	if(dis>>dodge)
	if(dis>>critical)
		return true;

	return false;
}