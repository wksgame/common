#ifndef KISS_THREAD_H
#define KISS_THREAD_H

#include<atomic>

namespace kiss
{
	class Thread
	{
		static void ThreadFunc(Thread* t);

		Thread(const Thread&)=delete;
		Thread& operator=(const Thread&)=delete;

	public:
		Thread();
		virtual ~Thread(){}
		void Start();

	protected:
		virtual void Run()=0;

	protected:
		//std::atomic<bool>	quit;
	};
}//namespace kiss
#endif//KISS_THREAD_H