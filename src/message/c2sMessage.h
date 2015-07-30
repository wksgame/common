#ifndef C2S_MESSAGE_H
#define C2S_MESSAGE_H

struct c2sMessage
{

};

struct c2sLogin:public c2sMessage
{
	int userID;
	int passward;
};
#endif//C2S_MESSAGE_H