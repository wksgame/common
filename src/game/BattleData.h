#ifndef BATTLE_DATA_H
#define BATTLE_DATA_H

#include<string>

namespace kiss
{
	class DataOutputStream;
	class DataInputStream;
}

struct BattleRecord
{
	int attackerID;
	int attackerHP;
	int defenderID;
	int defenderHP;

	int skillID;

	int damage;
	bool critical;
	bool dodge;

	bool Write(kiss::DataOutputStream& dos)const;
	bool Read(kiss::DataInputStream& dis);
};

//std::ostream& operator<<(std::ostream& os, const BattleRecord& br);

#endif//BATTLE_DATA_H