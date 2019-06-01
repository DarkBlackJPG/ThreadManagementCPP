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

extern void dispatch();

class Thread;



void interrupt (*System::_oldTimerInterrupt)(...) = 0;
void interrupt (*System::_newTimerInterrupt)(...) = timer;
PCBList* System::allUserThreads = new PCBList();
volatile int System::preemptionEnabled;
PCB * System::running;
Thread * System::idleThread;
volatile int System::timerCall = 0;
volatile int System::contextSwitch = 0;
SBLKLST* System::blockedOnWaitList;
IVTEntry* System::interruptEntries[256];

///////////////////////////////////////////
extern void tick(); // MORA SE IZBRISATI!!!!!!!!
//////////////////////////////////////////

void interrupt timer(...) {
	if(!System::timerCall){
		System::_oldTimerInterrupt();
		tick();
		--PCB::currentTimeSlice;
		System::blockedOnWaitList->updateList();
	}
	System::timerCall = 0;

	if(System::contextSwitch || ( !PCB::runNonStop && PCB::currentTimeSlice == 0 )){

		if(System::preemptionEnabled > 0){


			System::contextSwitch = 0;

			GET_CONTEXT

			System::running->sp = tsp;
			System::running->ss = tss;
			System::running->bp = tbp;



			if((System::running->myThread != System::idleThread) && (System::running->myState == PCB::READY)){
				Scheduler::put(System::running);
			}
			System::running = Scheduler::get();

			if(System::running == 0){
				System::running = System::idleThread->myPCB;
			}


			tsp = System::running->sp;
			tss = System::running->ss;
			tbp = System::running->bp;

			 PCB::currentTimeSlice = System::running->myTimeSlice;
			 PCB::runNonStop = !(PCB::currentTimeSlice > 0);


			PUT_CONTEXT


		}
		else {
			System::contextSwitch = 1;
		}

	}

}

void System::setTimerCall(){
	timerCall = 1;
}

System::~System() {
	// TODO Auto-generated destructor stub
}

Thread * System::getThreadById(int id){
	return allUserThreads->getNodeWithId(id)->myThread;
};

void System::disablePreemption(){
	System::preemptionEnabled = -1;
};

void System::enablePreemption(){
	System::preemptionEnabled = 1;
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

	System::preemptionEnabled = 1;
	System::blockedOnWaitList = new SBLKLST();
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
	delete System::idleThread;
	INTERRUPT_ENABLE

};


