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
#include <IOSTREAM.H>
#include "SCHEDULE.H"
/**
 *
 * ECB.cpp is used as the implementation of the user class "Event".
 *
 *
 *
 */

KernelEv::KernelEv(IVTNo ivtNo, Event * myEvent):
		myNo(ivtNo), myEvent(myEvent) {
	owner = System::running;
	value = 0;
};

void KernelEv::wait(){
	if(System::running == owner){
		INTERRUPT_DISABLE
		if (--value < 0) {
			System::running->myState = PCB::BLOCKED;
			dispatch();
			INTERRUPT_ENABLE
			return;
		}
		INTERRUPT_ENABLE
		return;
	} else return;
};

void KernelEv::signal(){
	lockPreemption 
	if(value < 0){
		owner->myState = PCB::READY;
		Scheduler::put(owner);
		value = 0;
		dispatch();
	}
	unlockPreemption 
};

