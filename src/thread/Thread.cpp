#include"Thread.h"
#include<thread>
#include<cstring>

namespace kiss
{
	void Thread::ThreadFunc(Thread* t)
	{
		t->Run();
	}

	Thread::Thread()
	{
		//quit=false;
	}

	void Thread::Start()
	{
		std::thread t(ThreadFunc,this);

		t.detach();
	}
}