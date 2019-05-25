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


/// dispatch() must be inside disabled preemprtion!
void dispatch() {
	if(System::preemptionEnabled <= 0)
		return;
	INTERRUPT_DISABLE
	System::timerCall = 1;
	System::contextSwitch = 1;
	asm int 0x08;
	INTERRUPT_ENABLE
};


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
	StackSize temp = (stackSize > 65536 ? 65536 : stackSize);
	System::disablePreemption();
	myPCB = new PCB(this, temp, timeSlice);
	System::enablePreemption();

}

Thread::~Thread() {
	waitToComplete();
	System::disablePreemption();
	delete myPCB;
	System::enablePreemption();
}

