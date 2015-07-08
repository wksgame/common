#include"ClientSession.h"
#include"c2sMessage.h"
#include<iostream>

ClientSession::ClientSession()
{
	RegisterMessage(1, &ClientSession::OnLogin);
}

bool ClientSession::OnLogin(const c2sMessage* msg)
{
	c2sLogin* c2s = (c2sLogin*)msg;

	std::cout << c2s->userID << " " << c2s->passward << std::endl;
	return true;
}



