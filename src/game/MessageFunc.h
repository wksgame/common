#ifndef MESSAGE_FUNC_H
#define MESSAGE_FUNC_H

#include"ClientSession.h"

typedef bool(*MessageProc)(ClientSession&, kiss::DataInputStream&);

enum MESSAGE_ID
{
	MSGID_NULL,
	MSGID_REGISTER,		// 注册用户
	MSGID_LOGIN,		// 登入
	MSGID_CREATE,		// 创建角色
	MSGID_SELECT,		// 选择角色
	MSGID_ATTACK,		// 战斗
	MSGID_MAX,
};

enum ResultCode
{
	RC_OK,
};

bool MessageFunc(ClientSession&, void*);
bool RegisterMessageProc(MESSAGE_ID id, MessageProc mp);
bool InitMessageProc();

bool Register(ClientSession&, kiss::DataInputStream&);
bool Login(ClientSession&, kiss::DataInputStream&);
bool CreateRole(ClientSession&, kiss::DataInputStream&);
bool SelectRole(ClientSession&, kiss::DataInputStream&);
bool Attack(ClientSession&, kiss::DataInputStream&);

#endif//MESSAGE_FUNC_H