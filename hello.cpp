
#include "Thread.h"
#include "System.h"
#include "semaphor.h"
#include "Buffer.h"
#include "Event.h"
#include "IVTEntry.h"
#include <stdlib.h>
#include <IOSTREAM.H>
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
//class MySem : public Semaphore {
//public:
//	MySem(int i ): Semaphore(i){};
//
//};
//
//class myThread : public Thread {
//
//	MySem * sem;
//
//public:
//	myThread(StackSize stackSize, Time timeSlice) : Thread(stackSize, timeSlice) {
//		sem = new MySem(2);
//	};
//	void run();
//};
//
//void myThread::run(){
//	int p = 0;
//
//	while(p < 50){
//#ifndef BCC_BLOCK_IGNORE
//		INTERRUPT_DISABLE
//		System::disablePreemption();
//		cout << "Nit broj: " << Thread::getRunningId() <<" " <<p<<"\n" ;
//		System::enablePreemption();
//		INTERRUPT_ENABLE
//#endif
//		sem->wait(19);
//		p++;
//	}
//
//}
//
//
//class Test3:public Thread{
//	public:
//		Test3():Thread(4096,2){};
//		~Test3(){
//			waitToComplete();
//		}
//		void run();
//};
//
//
//void Test3::run(){
//
//	while(1){
//		for (int i=0;i<20000;i++){
//			for (int j=0;j<22000;j++){}
//		}
//		System::disablePreemption();
//		cout << Thread::getRunningId() << endl;
//		System::enablePreemption();
//	}
//}
//




/*
 * myThread * thread1 = new myThread(512, 3);
	myThread * thread2 = new myThread(512, 3);
	myThread * thread3 = new myThread(512, 3);
	myThread * thread4 = new myThread(512, 3);
	myThread * thread5 = new myThread(512, 3);
	myThread * thread6 = new myThread(512, 3);
	myThread * thread7 = new myThread(512, 3);
	myThread * thread8 = new myThread(512, 3);
	myThread * thread9 = new myThread(512, 3);
	myThread * thread10 = new myThread(512, 3);
	myThread * thread11 = new myThread(512, 3);

	thread1->start();
	thread2->start();
	thread3->start();
	thread4->start();
	thread5->start();
	thread6->start();
	thread7->start();
	thread8->start();
	thread9->start();
	thread10->start();
	thread11->start();


	thread1->waitToComplete();
	thread2->waitToComplete();
	thread3->waitToComplete();
	thread4->waitToComplete();
	thread5->waitToComplete();
	thread6->waitToComplete();
	thread7->waitToComplete();
	thread8->waitToComplete();
	thread9->waitToComplete();
	thread10->waitToComplete();
	thread11->waitToComplete();
 */

//
//
//
//class Producer:public Thread{
//	private:
//		Buffer *buffer;
//	public:
//		Producer(Buffer *buffer_):Thread(4096,2){
//			buffer = buffer_;
//		};
//		~Producer(){
//			waitToComplete();
//		}
//		void run();
//};
//
//class Consumer:public Thread{
//	private:
//		Buffer *buffer;
//	public:
//		Consumer(Buffer *buffer_):Thread(4096,2){
//			buffer = buffer_;
//		};
//		~Consumer(){
//			waitToComplete();
//		}
//		void run();
//};
//
//
//void Producer::run(){
//	while(1){
//
//		buffer->put(rand()%200);
//		System::disablePreemption();
//		cout << "Element produced " << endl;
//		System::enablePreemption();
//		for (int i=0;i<25000;i++){
//			for (int j=0;j<5000;j++);
//		}
//	}
//}
//
//
//void Consumer::run(){
//	while(1){
//		int element = buffer->getElement();
//		System::disablePreemption();
//		cout << "Element " << element << " consumed " << endl;
//		System::enablePreemption();
//		for (int i=0;i<25000;i++){
//			for (int j=0;j<5000;j++);
//		}
//	}
//}
//
//




int syncPrintf(const char *format, ...)
{
	 System::disablePreemption();
	int res;
	va_list args;
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	System::enablePreemption();
		return res;
}



#define lock System::disablePreemption();
#define unlock System::enablePreemption();



/*
 	 Test: idleThread
*/


void tick(){}

Semaphore sem(0);

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{

	sem.wait(getId() * 5);
	System::disablePreemption();
	cout << "Zavrsena " << getId() << " nit\n";
	System::enablePreemption();
}


int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread *t1,*t2, *t3, *t4, *t5;
	t1 = new TestThread();
	t2 = new TestThread();
	t3 = new TestThread();
	t4 = new TestThread();
	t5 = new TestThread();
	t1->start();
	t2->start();
	t3->start();
	t4->start();
	t5->start();

	t1->waitToComplete();
	t2->waitToComplete();
	t3->waitToComplete();
	t4->waitToComplete();
	t5->waitToComplete();
//	delete t1;
//	delete t2;
//	delete t3;
//	delete t4;
//	delete t5;
////	while(1);
	syncPrintf("Test ends.\n");
	return 0;
}

int main(int argc, char* argv[]){

	System::systemInitialization();
	int res = userMain(argc, argv);
	System::systemRestoration();

#ifndef BCC_BLOCK_IGNORE
		INTERRUPT_DISABLE
		cout << "\n**********\n*HappyEnd*\n**********\n";
		INTERRUPT_ENABLE
#endif
	return 0;
}







