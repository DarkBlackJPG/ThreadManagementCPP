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


#include <IOSTREAM.h>

KernelSem::KernelSem(int init) :
 value(init) {
	valueBlock = new List();
	timerBlockedList = new List();
};
KernelSem::~KernelSem(){

};



int KernelSem::wait(Time maxTimeToWait){
	int returnVal = 1;
	INTERRUPT_DISABLE
	    if (--value < 0) {
	        block(maxTimeToWait);
	        INTERRUPT_ENABLE
	        returnVal = System::running->timerRelease;
	    }
	INTERRUPT_ENABLE
	  return returnVal;
};


void KernelSem::signal(int n){
	INTERRUPT_DISABLE
	if(n == 0){
		if(value + 1 <= 0){
					deblock();
				}
				else
					value++;
	}
	for(int i = 0; i < n; i++){
		if(value + 1 <= 0){
			deblock();
		}
		else
			value++;
	}
	INTERRUPT_ENABLE
};



int KernelSem::val(){
	return value;
}




void KernelSem::block(Time t){
	System::running->myState = PCB::BLOCKED;
	if(t > 0){
		timerBlockedList->add(System::running);
		System::blockedOnWaitList->insert(System::running, t, this);
	} else { // blocked for value
		valueBlock->add(System::running);
	}
	dispatch();
};

void KernelSem::deblock(){
	PCB* tempDat = valueBlock->getPCB();
	if(!tempDat){

		tempDat = timerBlockedList->getPCB();

		System::blockedOnWaitList->remove(tempDat);
	}
	if(!tempDat)
	{
		INTERRUPT_DISABLE
		cout << "GRESKA KOD DEBLOCK()!!!\n";
		INTERRUPT_ENABLE
		return;
	}
	value++;
	tempDat->myState = PCB::READY;
	tempDat->timerRelease = 1;
	Scheduler::put(tempDat);
};

#include <stdio.h>
void KernelSem::deblock(PCB* data) {
	// OVO SE POZIVA IZ SISTEMA
	timerBlockedList->removePCB(data);
	value++;
	data->timerRelease = 0;
	data->myState = PCB::READY;
	Scheduler::put(data);
};


/*
 *
 * LISTA
 *
 */



KernelSem::List::List() : first(0), last(0){}
KernelSem::List::~List(){
	Node * temp = first;
	for(; temp != 0; temp = first, first = first->next){
		delete temp;
	}

};
void KernelSem::List::add(PCB* data){
	Node * node = new Node(data);
	last = (first ? last->next : first) = node;
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
