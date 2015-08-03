#include"Thread.h"
#include<thread>
#include<cstring>

namespace kiss
{
#ifndef LOG_WITHOUT_THREAD_NAME
	extern thread_local char thread_name[32];
#endif//LOG_WITHOUT_THREAD_NAME

	void ThreadFunc(Thread* t)
	{
#ifndef LOG_WITHOUT_THREAD_NAME
	strncpy(thread_name,t->thread_name,32);
#endif//LOG_WITHOUT_THREAD_NAME

		t->Run();

		delete t;
	}

	Thread::Thread(const char* thread_name)
	{
		strncpy(this->thread_name,thread_name,32);
	}

	void Thread::Start()
	{
		std::thread t(ThreadFunc,this);

		t.detach();
	}
}