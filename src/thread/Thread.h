#ifndef KISS_THREAD_H
#define KISS_THREAD_H

namespace kiss
{
	class Thread
	{
		friend void ThreadFunc(Thread* t);

	public:
		virtual ~Thread(){}
		void Start();

	protected:
		virtual void Run()=0;
	};
}//namespace kiss
#endif//KISS_THREAD_H