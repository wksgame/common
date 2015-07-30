#pragma once

#include<string>

namespace kiss
{
	class DataOutputStream;
	class DataInputStream;
}

struct ctsRegister
{
	std::string username;
	std::string password;

	bool Read(kiss::DataInputStream& dis);
	bool Write(kiss::DataOutputStream& dos)const;
};

struct ctsLogin
{
	std::string username;
	std::string password;

	bool Read(kiss::DataInputStream& dis);
	bool Write(kiss::DataOutputStream& dos)const;
};

struct ctsCreateRole
{
	std::string rolename;

	bool Read(kiss::DataInputStream& dis);
	bool Write(kiss::DataOutputStream& dos)const;
};

struct ctsSelectRole
{
	int roleID;

	bool Read(kiss::DataInputStream& dis);
	bool Write(kiss::DataOutputStream& dos)const;
};

struct ctsAttack
{
	int id;

	bool Read(kiss::DataInputStream& dis);
	bool Write(kiss::DataOutputStream& dos)const;
};
