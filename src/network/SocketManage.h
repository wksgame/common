#ifndef KISS_SOCKET_MANAGE_H
#define KISS_SOCKET_MANAGE_H

namespace kiss
{
	class AcceptSocket;
	
	class SocketManage
	{
	public:
		virtual ~SocketManage(){}
		
		virtual bool Add(kiss::AcceptSocket* s)=0;
		virtual void Remove(kiss::AcceptSocket* s)=0;

		virtual bool Update()=0;
	};//class SocketManage
}

#endif//KISS_SOCKET_MANAGE_H