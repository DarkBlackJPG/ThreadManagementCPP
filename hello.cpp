
#include <iostream.h>
#include "Thread.h"
#include "System.h"


class myThread : public Thread {
public:
	myThread(StackSize stackSize, Time timeSlice) : Thread(stackSize, timeSlice) {};
	void run();
};

void myThread::run(){
	int p = 0;

	while(p < 50){
#ifndef BCC_BLOCK_IGNORE
		INTERRUPT_DISABLE
		System::disablePreemption();
		cout << "Nit broj: " << Thread::getRunningId() <<" " <<p<<"\n" ;
		System::enablePreemption();
		INTERRUPT_ENABLE
#endif
		for(int i= 0; i < 10000; i++)
			for(int j = 0; j < 10000; j++);
		p++;
	}

}


class Test3:public Thread{
	public:
		Test3():Thread(4096,2){};
		~Test3(){
			waitToComplete();
		}
		void run();
};


void Test3::run(){

	while(1){
		for (int i=0;i<10000;i++){
			for (int j=0;j<10000;j++){}
		}
		System::disablePreemption();
		cout <<Thread::getRunningId() << endl;
		System::enablePreemption();
	}
}


int main(int argc, char* argv[]){

	System::systemInitialization();


	const int numOfThreads = 140;
	Test3 *test[numOfThreads];
	for (int i=0;i<numOfThreads;i++){
		test[i] = new Test3();
		test[i]->start();
	}
	for (int j =numOfThreads-1;j>=0;j--)
		delete test[j];

	System::systemRestoration();

#ifndef BCC_BLOCK_IGNORE
		INTERRUPT_DISABLE
		cout << "HappyEnd\n";
		INTERRUPT_ENABLE
#endif
	return 0;
}







