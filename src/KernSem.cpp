/*
 * KernSem.cpp
 *
 *  Created on: May 22, 2019
 *      Author: OS1
 */

#include "KernSem.h"
#include "System.h"
#include "PCB.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int init) :
 value(init) {
	lockPreemption
	valueBlock = new List();
	timerBlockedList = new List();
	unlockPreemption
};
KernelSem::~KernelSem(){
	lockPreemption
	delete valueBlock;
	delete timerBlockedList;
	unlockPreemption
};



int KernelSem::wait(Time maxTimeToWait){
	int returnVal = 1;
	INTERRUPT_DISABLE
	    if (--value < 0) {
	        block(maxTimeToWait);
	        returnVal = System::running->timerRelease;
	    }
	INTERRUPT_ENABLE
	  return returnVal;
};


int KernelSem::signal(int n){
	INTERRUPT_DISABLE
	int count = 0;
	if(n == 0){
		if(value + 1 <= 0){
					deblock();
					count = 1;
		}
		else
			value++;
	} else {
		for(int i = 0; i < n; i++){
			if(value + 1 <= 0){
				deblock();
				count++;
			}
			else
				value++;
		}
	}
	INTERRUPT_ENABLE
	return count;
};



int KernelSem::val(){
	return value;
}




void KernelSem::block(Time t){ // Block is called from wait and has interrupts disabled
	System::running->myState = PCB::BLOCKED;
	if(t > 0){
		timerBlockedList->add(System::running);
		System::blockedOnWaitList->insert(System::running, t, this);
	} else { // blocked for value
		valueBlock->add(System::running);
	}
	dispatch();

};

/*
 *
 * Deblock called exclusively from signal
 *
 */
void KernelSem::deblock(){
	if(valueBlock->first == 0 && timerBlockedList->first == 0)
		return;
	PCB* tempDat = valueBlock->getPCB();
	if(!tempDat){
		tempDat = timerBlockedList->getPCB();
		System::blockedOnWaitList->remove(tempDat);
	}
	if(!tempDat){ // This should never happen
		return;
	}
	value++;
	tempDat->myState = PCB::READY;
	tempDat->timerRelease = 1;
	Scheduler::put(tempDat);
};
/*
 *
 * System wide deblock -> System::updateWaitList(); ==> called from timer
 *
 */
void KernelSem::deblock(PCB* data) {
	timerBlockedList->removePCB(data);
	value++;
	data->timerRelease = 0;
	data->myState = PCB::READY;

	Scheduler::put(data);
};


/*
 *
 * KernelSem::List -> Semaphore list implementation
 *
 */



KernelSem::List::List() :
		first(0), last(0){}

KernelSem::List::~List(){
	Node * temp = first;
	for(; temp != 0; temp = first){
		temp->data->myState = PCB::READY;
		Scheduler::put(temp->data);
		first = first->next;
		delete temp;
	}

};
void KernelSem::List::add(PCB* data){
	last = (first ? last->next : first) = new Node(data);
};
PCB * KernelSem::List::getPCB(){
	if(!first)
		return 0;
	Node* temp = first;

	if(first->next == 0)
		last = 0;

	first = first->next;
	PCB* pointer = temp->data;
	delete temp;
	return pointer;
};
void KernelSem::List::removePCB(PCB* data){
	Node* temp = first, *prev = 0;
	for(; temp != 0 && temp->data != data; prev = temp, temp = temp->next);
	if(!temp)
		return;
	if(!prev){
		first = temp->next;
	} else {
		prev->next = temp->next;
	}
	if(temp->next == 0){
		last = prev;
	}

	delete temp;

}
