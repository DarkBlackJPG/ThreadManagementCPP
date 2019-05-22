/*
 * System.cpp
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */


#include "PCB.h"
#include "System.h"
#include "Idle.h"
#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#define GET_CONTEXT asm mov tsp, sp;\
	asm mov tss, ss;\
	asm mov tbp, bp;

#define PUT_CONTEXT asm mov sp, tsp;\
	asm mov ss, tss;\
	asm mov bp, tbp;

///////////////////////////////////////////
void tick() {}// MORA SE IZBRISATI!!!!!!!!
//////////////////////////////////////////

extern void dispatch();

class Thread;

void interrupt timer(...) {
	if(!System::timerCall){
		System::_oldTimerInterrupt();
		tick();
		--PCB::currentTimeSlice;
	}
	System::timerCall = 0;
	if(System::contextSwitch || ( !PCB::runNonStop && PCB::currentTimeSlice == 0 )){

		if(System::preemptionEnabled > 0){

			GET_CONTEXT

			System::running->sp = tsp;
			System::running->ss = tss;
			System::running->bp = tbp;


			if((System::running->myThread != System::idleThread) && (System::running->myState == PCB::READY)){
				Scheduler::put(System::running);
			}
			System::running = Scheduler::get();
			if(System::running == 0)
				System::running = System::idleThread->myPCB;

			tsp = System::running->sp;
			tss = System::running->ss;
			tbp = System::running->bp;

			 PCB::currentTimeSlice = System::running->myTimeSlice;
			 PCB::runNonStop = !(PCB::currentTimeSlice > 0);

			PUT_CONTEXT

			System::contextSwitch = 0;
		}
		else {
			System::contextSwitch = 1;
		}
	}
}

void interrupt (*System::_oldTimerInterrupt)(...) = 0;
void interrupt (*System::_newTimerInterrupt)(...) = timer;

int System::timerCall = 0;
void System::setTimerCall(){
	timerCall = 1;
}

int System::contextSwitch = 0;

System::~System() {
	// TODO Auto-generated destructor stub
}

PCBList* System::allUserThreads = new PCBList();
volatile int System::preemptionEnabled = 1;
PCB * System::running;
Thread * System::idleThread;

Thread * System::getThreadById(int id){
	return allUserThreads->getNodeWithId(id)->myThread;
};

void System::disablePreemption(){
	--System::preemptionEnabled;
};
void System::enablePreemption(){
	++System::preemptionEnabled;
	 if (System::preemptionEnabled > 0 && System::contextSwitch)
		dispatch();
};


void System::systemInitialization(){
	INTERRUPT_DISABLE
	// Preemption doesn't have to be disabled because there isn't a multithreaded
	// environment

#ifndef BCC_BLOCK_IGNORE
	_oldTimerInterrupt = getvect(0x08);
	setvect(0x08, timer);
#endif
	System::idleThread = new Idle();
	System::running = new PCB();

	INTERRUPT_ENABLE


};
void System::systemRestoration(){
	INTERRUPT_DISABLE
		// Preemption doesn't have to be disabled because there isn't a multithreaded
		// environment

	#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, _oldTimerInterrupt);
	#endif

	delete System::running;
	INTERRUPT_ENABLE

};


