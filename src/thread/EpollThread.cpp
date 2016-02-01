#include"EpollThread.h"
#include"network/EpollManage.h"
#include"message/Session.h"

namespace kiss 
{
	EpollThread::EpollThread()
	{
		manage = new EpollManage(1000);
	}
	
	EpollThread::~EpollThread()
	{
		delete manage;
		manage = nullptr;
	}
	
	bool EpollThread::Add(AcceptSocket* sock)
	{
		return manage->Add(sock);
	}

	void EpollThread::Remove(AcceptSocket* s)
	{
		return manage->Remove(s);
	}

// 	void EpollThread::Run()
// 	{
// 		while(true)
// 		{
// 			Update();
// 		}
// 	}

	void EpollThread::Update()
	{
		manage->Update();
	}

}//namespace kiss