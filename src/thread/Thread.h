#ifndef KISS_THREAD_H
#define KISS_THREAD_H

namespace kiss
{
	class Thread
	{
		friend void ThreadFunc(Thread* t);

	public:
		Thread(const char* thread_name);
		virtual ~Thread(){}
		void Start();

	protected:
		virtual void Run()=0;

	protected:
		char thread_name[32];
	};
}//namespace kiss
#endif//KISS_THREAD_H