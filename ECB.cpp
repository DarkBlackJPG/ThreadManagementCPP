/*
 * ECB.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "System.h"
#include "ECB.h"
#include "IVTEntry.h"
#include "PCB.h"
#include "SCHEDULE.H"
KernelEv::KernelEv(IVTNo ivtNo, Event * myEvent):
		myNo(ivtNo), myEvent(myEvent) {
	owner = System::running;
	value = 0;
	if(System::interruptEntries[ivtNo] != 0)
		System::interruptEntries[ivtNo]->myEvent = this;
	else
		return;

};

void KernelEv::wait(){
	if(System::running == owner){
		INTERRUPT_DISABLE
				if (--value < 0) {
					owner->myState = PCB::BLOCKED;
					dispatch();
				}
		INTERRUPT_ENABLE
	}
};
void KernelEv::signal(){
	INTERRUPT_DISABLE
	if(value < 0){
		owner->myState = PCB::READY;
		Scheduler::put(owner);
		value = 0;
	} else {
		value = 1;
	}
	INTERRUPT_ENABLE
	dispatch();
};
