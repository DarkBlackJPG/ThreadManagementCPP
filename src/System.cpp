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


static volatile unsigned tsp;
static volatile unsigned tss;
static volatile unsigned tbp;

///////////////////////////////////////////
extern void tick(); // MORA SE IZBRISATI!!!
///////////////////////////////////////////

void interrupt timer(...) {

	if(!System::timerCall){
		System::_oldTimerInterrupt();
		tick();
		--PCB::currentTimeSlice;
		System::blockedOnWaitList->updateList();
	}

	// timerCall should be cleared here because you can call dispatch while in critical section
	// because of that you would have to put clearing inside both the if and else branch
	// which, in turn, translate to timerCall = 0 unconditionally being called
	// ========================================================================================
	   System::timerCall = 0; ////////////////////////////////////////////////////////////////
	// ========================================================================================
	if(System::contextSwitch ||
			(!PCB::runNonStop && PCB::currentTimeSlice == 0 )){

		if(preemptionEnabled == 1){

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

			System::contextSwitch = 0;

		}
		else {
			System::contextSwitch = 1;
		}

	}

}

/**
 *
 * This method was once used, not anymore
 *
 */
void System::setTimerCall(){
	timerCall = 1;
}


Thread * System::getThreadById(int id){
	return allUserThreads->getNodeWithId(id)->myThread;
};

/**
 *
 * This method was once used, not anymore
 *
 */
void System::disablePreemption(){
	System::preemptionEnabled = -1;
};

/**
 *
 * This method was once used, not anymore
 *
 */
void System::enablePreemption(){
	System::preemptionEnabled = 1;
	 if (System::preemptionEnabled > 0 && System::contextSwitch)
		dispatch();
};

/**
 *
 * During System initialization it is necessary to mask all interrupts because you are changing the IV table with setvect and getvect
 *
 */

void System::systemInitialization(){
	INTERRUPT_DISABLE

#ifndef BCC_BLOCK_IGNORE
	_oldTimerInterrupt = getvect(0x08);
	setvect(0x08, timer);
#endif

	System::idleThread = new Idle();
	System::running = new PCB();
	System::blockedOnWaitList = new SBLKLST();
	INTERRUPT_ENABLE


};

/**
 *
 * During System initialization it is necessary to mask all interrupts because you are changing the IV table with setvect
 * =====================================================================================================================
 * Warning!
 * The assembly code is put inside System restoration because there is a not so frequent bug where an interrupt happens
 * while you're expecting a confirmation of your I/O interrupt request, this sends that last ack and tells the keyboard
 * "Hey, you can send, I recieved everything you've sent towards me!"
 *
 * This occurs mostly in the public test.
 *  =====================================================================================================================
 *
 */
void System::systemRestoration(){
	INTERRUPT_DISABLE

	#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, _oldTimerInterrupt);
	asm{
			cli
			in      al, 61h
			or      al, 10000000b
			out     61h, al
			and     al, 01111111b
			out     61h, al
			mov     al, 20h
			out     20h, al
			sti
		}
	#endif

	delete System::running;
	delete System::idleThread;
	delete System::blockedOnWaitList;
	INTERRUPT_ENABLE

};


