/*
 * PCB.cpp
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>
#include "System.h"

extern void dispatch();





int PCB::idGenerator = 0;
volatile int PCB::runNonStop = 0;
volatile Time PCB::currentTimeSlice = defaultTimeSlice;








PCB::PCB() :
		myThread(0), myTimeSlice(0), myId(-200), ss(0), sp(0), bp(0),
		myState(PCB::READY), myStack(0), timerRelease(1) {
	blockedOnThread = new PCBList();
	System::allUserThreads->insert(this);
}

ID PCB::getRunningId(){
	return System::running->getId();
}

void PCB::wrapper(){
	System::running->myThread->run();
	System::disablePreemption();
	for(PCB* temp = System::running->blockedOnThread->pop_front();
			 temp != 0;
		 	 temp = System::running->blockedOnThread->pop_front())
	{
		temp->myState = PCB::READY;
		Scheduler::put(temp);
	}
	System::running->myState = PCB::TERMINATED;

	System::enablePreemption();
	dispatch();
}

PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice) :
	myThread(myThread), myTimeSlice(timeSlice), myId(++idGenerator), ss(0), sp(0), bp(0),
	myState(PCB::CREATED), myStack(0), timerRelease(1) {

	StackSize realStackSize = stackSize / 2; // Stack size is half of given stack size

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


PCB::~PCB() {

	System::allUserThreads->remove(this);
	delete blockedOnThread;
	if(myStack)
		delete [] myStack;
}

void PCB::start() {
	System::disablePreemption();
	if(myState == PCB::CREATED)
	{
		myState = PCB::READY;
		Scheduler::put(this);
	}
	System::enablePreemption();
};
void PCB::waitToComplete(){ // OVDE JE MOGUCE PUCANJE!!!
	System::disablePreemption();
	if(this->myState == PCB::TERMINATED || System::idleThread == this->myThread){
		return;
	}
	System::running->myState = PCB::BLOCKED;
	blockedOnThread->insert(System::running);
	System::enablePreemption();
	dispatch();
	System::disablePreemption();
	cout << "dasdas\n";
	System::enablePreemption();

};
ID PCB::getId() const{
	return myId;
};
