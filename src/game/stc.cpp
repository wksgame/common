#include"stc.h"
#include"io/DataOutputStream.h"
#include"io/DataInputStream.h"

using namespace kiss;

namespace game
{
	bool stcRegister::Write(DataOutputStream& dos)const
	{
		if(dos<<resultCode)
			return true;

		return false;
	}

	bool stcRegister::Read(DataInputStream& dis)
	{
		if(dis>>resultCode)
			return true;

		return false;
	}

	bool stcLogin::Write(DataOutputStream& dos)const
	{
		if(dos<<resultCode)
		if(dos<<roles)
			return true;

		return false;
	}

	bool stcLogin::Read(DataInputStream& dis)
	{
		if(dis>>resultCode)
		if(dis>>roles)
			return true;

		return false;
	}

	bool stcCreateRole::Write(DataOutputStream& dos)const
	{
		if(dos<<resultCode)
		if(dos<<p)
			return true;

		return false;
	}

	bool stcCreateRole::Read(DataInputStream& dis)
	{
		if(dis>>resultCode)
		if(dis>>p)
			return true;

		return false;
	}

	bool stcSelectRole::Write(DataOutputStream& dos)const
	{
		if(dos<<resultCode)
		if(dos<<p)
			return true;

		return false;
	}

	bool stcSelectRole::Read(DataInputStream& dis)
	{
		if(dis>>resultCode)
		if(dis>>p)
			return true;

		return false;
	}

	bool stcAttack::Write(DataOutputStream& dos)const
	{
		if(dos<<resultCode)
		if(dos<<record)
			return true;

		return false;
	}

	bool stcAttack::Read(DataInputStream& dis)
	{
		if(dis>>resultCode)
		if(dis>>record)
			return true;

		return false;
	}
}