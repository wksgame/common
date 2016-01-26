#ifndef KISS_SOCKET_MANAGE_H
#define KISS_SOCKET_MANAGE_H

namespace kiss
{
	class AcceptSocket;
	
	class ISocketManage
	{
	public:
		virtual ~ISocketManage(){}
		
		virtual bool Add(AcceptSocket* s)=0;
		virtual void Remove(AcceptSocket* s)=0;

		virtual bool Update()=0;
	};//class SocketManage
}

#endif//KISS_SOCKET_MANAGE_H