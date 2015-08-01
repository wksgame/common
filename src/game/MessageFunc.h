#ifndef MESSAGE_FUNC_H
#define MESSAGE_FUNC_H

#include"ClientSession.h"

typedef bool(*MessageProc)(ClientSession&, kiss::DataInputStream&);

enum MESSAGE_ID
{
	MSGID_NULL,
	MSGID_REGISTER,		// ע���û�
	MSGID_LOGIN,		// ����
	MSGID_CREATE,		// ������ɫ
	MSGID_SELECT,		// ѡ����ɫ
	MSGID_ATTACK,		// ս��
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