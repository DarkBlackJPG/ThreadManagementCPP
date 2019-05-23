/*
 * semaphor.cpp
 *
 *  Created on: May 22, 2019
 *      Author: OS1
 */

#include "semaphor.h"
#include "System.h"
#include "KernSem.h"


Semaphore::Semaphore(int init){
System::disablePreemption();
myImpl = new KernelSem(init);
System::enablePreemption();
};
Semaphore::~Semaphore(){
	System::disablePreemption();
	delete myImpl;
	System::enablePreemption();
};

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
};
void Semaphore::signal(int n){
	myImpl->signal(n);
};

int Semaphore::val() const{
	int val = 0;
	System::disablePreemption();
	val = myImpl->val();
	System::enablePreemption();
	return val;
};
