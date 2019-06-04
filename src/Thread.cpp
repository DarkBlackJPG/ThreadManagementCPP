/*
 * Thread.cpp
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#include "Thread.h"
#include "System.h"
#include "PCB.h"
#include <iostream.h>

/*
 *
 * ====== Dispatch =======
 *
 */
void dispatch() {
	if(preemptionEnabled == 0){
		return;
	}
	INTERRUPT_DISABLE
	System::timerCall = 1;
	System::contextSwitch = 1;
	asm int 0x08;
	INTERRUPT_ENABLE
};
/**
 *
 * =======================
 *
 */
void Thread::start(){
	myPCB->start();
};

void Thread::waitToComplete(){
	myPCB->waitToComplete();
};

ID Thread::getId(){
	return myPCB->getId();
};

ID Thread::getRunningId(){
	return PCB::getRunningId();
};

Thread* Thread::getThreadById(ID id){
	return System::getThreadById(id);
};

Thread::Thread(StackSize stackSize, Time timeSlice){
	lockPreemption
	myPCB = new PCB(this, stackSize, timeSlice);
	unlockPreemption

}

Thread::~Thread() {
	/*
	 *
	 * Independently locked, mustn't be locked here
	 *
	 */
	myPCB->waitToComplete();

	lockPreemption
	delete myPCB;
	unlockPreemption
}

