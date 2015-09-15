#include"EpollThread.h"
#include"network/EpollManage.h"
#include"message/Session.h"

namespace kiss 
{
	EpollThread::EpollThread(const char* thread_name): Thread(thread_name)
	{
		manage = new EpollManage(1000);
	}
	
	EpollThread::~EpollThread()
	{
		delete manage;
		manage = nullptr;
	}
	
	bool EpollThread::Join(Session* sock)
	{
		return manage->Add(sock);
	}

	void EpollThread::Update()
	{
		manage->Update();
	}

}//namespace kiss