#ifndef KISS_CLIENT_SESSION_H
#define KISS_CLIENT_SESSION_H

#include"MessageProcess.h"

struct c2sMessage;

class ClientSession :public MessageProcess<ClientSession, c2sMessage>
{
public:
	ClientSession();

	bool OnLogin(const c2sMessage* msg);
};

#endif//KISS_CLIENT_SESSION_H