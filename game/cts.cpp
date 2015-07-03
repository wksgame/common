#include"cts.h"
#include"../io/DataOutputStream.h"
#include"../io/DataInputStream.h"

using namespace kiss;

bool ctsRegister::Read(DataInputStream& dis)
{
	if(dis>>username)
	if(dis>>password)
		return true;

	return false;
}

bool ctsRegister::Write(DataOutputStream& dos)const
{
	if(dos<<username)
	if(dos<<password)
		return true;

	return false;
}

bool ctsLogin::Read(DataInputStream& dis)
{
	if(dis>>username)
	if(dis>>password)
		return true;

	return false;
}

bool ctsLogin::Write(DataOutputStream& dos)const
{
	if(dos<<username)
	if(dos<<password)
		return true;

	return false;
}

bool ctsCreateRole::Read(DataInputStream& dis)
{
	if(dis>>rolename)
		return true;

	return false;
}

bool ctsCreateRole::Write(DataOutputStream& dos)const
{
	if(dos<<rolename)
		return true;

	return false;
}

bool ctsSelectRole::Read(DataInputStream& dis)
{
	if(dis>>roleID)
		return true;

	return false;
}

bool ctsSelectRole::Write(DataOutputStream& dos)const
{
	if(dos<<roleID)
		return true;

	return false;
}

bool ctsAttack::Read(DataInputStream& dis)
{
	if(dis>>id)
		return true;

	return false;
}

bool ctsAttack::Write(DataOutputStream& dos)const
{
	if(dos<<id)
		return true;

	return false;
}