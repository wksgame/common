#include"Thread.h"
#include<thread>

namespace kiss
{
	void ThreadFunc(Thread* t)
	{
		t->Run();

		delete t;
	}

	void Thread::Start()
	{
		std::thread t(ThreadFunc,this);

		t.detach();
	}
}