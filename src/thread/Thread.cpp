#include"Thread.h"
#include<thread>
#include<cstring>

namespace kiss
{
	void ThreadFunc(Thread* t)
	{
		t->Run();

		delete t;
	}

	Thread::Thread(const char* thread_name)
	{
		strncpy(this->thread_name,thread_name,32);
		quit=false;
	}

	void Thread::Start()
	{
		std::thread t(ThreadFunc,this);

		t.detach();
	}

	void Thread::Run()
	{
		while(!quit)
		{
			Update();
		}
	}
}