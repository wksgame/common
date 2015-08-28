#ifndef KISS_THREAD_H
#define KISS_THREAD_H

#include<atomic>

namespace kiss
{
	class Thread
	{
		friend void ThreadFunc(Thread* t);

		Thread(const Thread&)=delete;
		Thread& operator=(const Thread&)=delete;

	public:
		Thread(const char* thread_name);
		virtual ~Thread(){}
		void Start();

	protected:
		virtual void Run();
		virtual void Update()=0;

	protected:
		char thread_name[32];
		std::atomic<bool>	quit;
	};
}//namespace kiss
#endif//KISS_THREAD_H