#include"SelectThread.h"
#include"network/SelectManage.h"
#include"message/Session.h"

using namespace std;

namespace kiss
{
	SelectThread::SelectThread()
	{
		manage = new SelectManage();
		cur_time = 0;
		last_time = 0;
		sleep_time = 0;
	}

	SelectThread::~SelectThread()
	{
	}

	bool SelectThread::Add(AcceptSocket* s)
	{
		bool result = false;

		joinLock.lock();
			result = manage->Add(s);
		joinLock.unlock();

		return result;
	}

	void SelectThread::Remove(AcceptSocket* s)
	{
		quitLock.lock();
			manage->Remove(s);
		quitLock.unlock();
	}

	void SelectThread::Update()
	{
		if(joinLock.try_lock())
		{
			manage->ProcessAdd();
			joinLock.unlock();
		}

		if(quitLock.try_lock())
		{
			manage->ProcessRemove();
			quitLock.unlock();
		}

		manage->Update();
	}
}//namespace kiss