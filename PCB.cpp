/*
 * PCB.cpp
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "SCHEDULE.H"
#include <dos.h>
#include "System.h"

extern void dispatch();

int PCB::idGenerator = 0;
volatile int PCB::runNonStop = 0;
volatile Time PCB::currentTimeSlice = defaultTimeSlice;


/**
 *
 * Constructor invocation in System::systemInitialize(); no need to lock, surrounded with locks
 *
 */
PCB::PCB() :
		myThread(0), myTimeSlice(defaultTimeSlice), myId(-8086), ss(0), sp(0), bp(0),
		myState(PCB::READY), myStack(0), timerRelease(1) {
	blockedOnThread = new PCBList();
	System::allUserThreads->insert(this);
}

ID PCB::getRunningId(){
	return System::running->getId();
}

void PCB::wrapper(){

	System::running->myThread->run();

	lockPreemption
	for(PCB* temp = System::running->blockedOnThread->pop_front();
			 temp != 0;
		 	 temp = System::running->blockedOnThread->pop_front())
	{

		temp->myState = PCB::READY;
		Scheduler::put(temp);

	}

	System::running->myState = PCB::TERMINATED;

	unlockPreemption

	dispatch();
}

/*
 *
 * Constructor located in Thread constructor -> it's locked so no need to lock this part
 *
 */
PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice) :
	myThread(myThread), myTimeSlice(timeSlice), myId(++idGenerator), ss(0), sp(0), bp(0),
	myState(PCB::CREATED), myStack(0), timerRelease(1) {

	StackSize temp = (stackSize > 65536 ? 65536 : stackSize);
	StackSize realStackSize = temp / 2; // Stack size is half of given stack size

	myStack = new unsigned[realStackSize];

#ifndef BCC_BLOCK_IGNORE
	myStack[realStackSize - 2] = FP_SEG(PCB::wrapper);
	myStack[realStackSize - 3] = FP_OFF(PCB::wrapper);
#endif

	myStack[realStackSize - 1] = 0x200;

	blockedOnThread = new PCBList();

	int stackOffset = realStackSize - 12;

#ifndef BCC_BLOCK_IGNORE
	ss = FP_SEG(myStack + stackOffset);
	sp = FP_OFF(myStack + stackOffset);
	bp = sp;
#endif

	System::allUserThreads->insert(this);
}

/**
 *
 * Destructor invocked in thread destructor -> locked no need to lock here
 *
 */

PCB::~PCB() {
	System::allUserThreads->remove(this);
	delete blockedOnThread;
	if(myStack)
		delete [] myStack;
}

void PCB::start() {
	lockPreemption
		if(myState == PCB::CREATED)
		{
			myState = PCB::READY;
			Scheduler::put(this);
		}
	unlockPreemption
};

void PCB::waitToComplete(){
	lockPreemption
		if(myState == PCB::TERMINATED){
			return;
		}
		if(System::idleThread == myThread) {
			return;
		}
		if( myState == PCB::CREATED){
			return;
		}

		System::running->myState = PCB::BLOCKED;
		blockedOnThread->insert(System::running);

	unlockPreemption
	dispatch();

};

ID PCB::getId() const{
	return myId;
};
