#include"BattleData.h"
#include"../io/DataOutputStream.h"
#include"../io/DataInputStream.h"

bool BattleRecord::Write(kiss::DataOutputStream& dos)const
{
	if(dos<<attackerID)
	if(dos<<attackerHP)
	if(dos<<defenderID)
	if(dos<<defenderHP)
	if(dos<<skillID)
	if(dos<<damage)
	if(dos<<critical)
	if(dos<<dodge)
		return true;

	return false;
}

bool BattleRecord::Read(kiss::DataInputStream& dis)
{
	if(dis>>attackerID)
	if(dis>>attackerHP)
	if(dis>>defenderID)
	if(dis>>defenderHP)
	if(dis>>skillID)
	if(dis>>damage)
	if(dis>>critical)
	if(dis>>dodge)
		return true;

	return false;
}

//ostream& operator<<(ostream& os, const BattleRecord& br)
//{
//	os<<br.attackerID<<br.attackerHP<<br.defenderID<<br.defenderHP<<br.damage<<br.critical<<br.dodge;
//	return os;
//}