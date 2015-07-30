#pragma once

#include"../game/BattleData.h"
#include"Player.h"
#include<list>

struct stcRegister
{
	int resultCode;

	bool Write(kiss::DataOutputStream& dos)const;
	bool Read(kiss::DataInputStream& dis);
};

struct stcLogin
{
	int resultCode;
	std::list<RoleInfo> roles;

	bool Write(kiss::DataOutputStream& dos)const;
	bool Read(kiss::DataInputStream& dis);
};

struct stcCreateRole
{
	int resultCode;
	Player p;

	bool Write(kiss::DataOutputStream& dos)const;
	bool Read(kiss::DataInputStream& dis);
};

struct stcSelectRole
{
	int resultCode;
	Player p;

	bool Write(kiss::DataOutputStream& dos)const;
	bool Read(kiss::DataInputStream& dis);
};

struct stcAttack
{
	int resultCode;
	std::list<BattleRecord> record;

	bool Write(kiss::DataOutputStream& dos)const;
	bool Read(kiss::DataInputStream& dis);
};