/*
 * System.h
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "PCBList.h"
#include "SBLKLST.h"

class Thread;
class IVTEntry;

#define INTERRUPT_DISABLE asm cli;


#define INTERRUPT_ENABLE asm sti;


class System {
	friend void interrupt timer(...);
	friend void dispatch();


	static void interrupt (*_oldTimerInterrupt)(...);
	static void interrupt (*_newTimerInterrupt)(...);
	static int timerCall;
public:

	static IVTEntry* interruptEntries[256];

	static PCBList* allUserThreads;

	static SBLKLST* blockedOnWaitList;

	static volatile int preemptionEnabled;
	static int contextSwitch;
	static PCB * running;
	static Thread * idleThread;
	static Thread * getThreadById(int id);

	static void setTimerCall();

	static inline void disablePreemption();
	static inline void enablePreemption();

	static void systemInitialization();
	static void systemRestoration();

	System();
	~System();
};


#endif /* SYSTEM_H_ */
