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
	lockPreemption
	myImpl = new KernelSem(init);
	unlockPreemption
};
Semaphore::~Semaphore(){
	lockPreemption
	delete myImpl;
	unlockPreemption
};

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
};
void Semaphore::signal(int n){
	myImpl->signal(n);
};

int Semaphore::val() const{
	int val = 0;
	lockPreemption
	val = myImpl->val();
	unlockPreemption
	return val;
};
