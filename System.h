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

static volatile int preemptionEnabled = 1;

#define lockPreemption preemptionEnabled--;

#define unlockPreemption preemptionEnabled++;\
	if (preemptionEnabled > 0 && System::contextSwitch)\
				dispatch();

#define INTERRUPT_DISABLE asm pushf;\
	asm cli;


#define INTERRUPT_ENABLE asm popf;


class System {
	friend void interrupt timer(...);
	friend void dispatch();


	static void interrupt (*_oldTimerInterrupt)(...);
	static void interrupt (*_newTimerInterrupt)(...);

public:
	volatile static int timerCall;

	static IVTEntry* interruptEntries[256];

	static PCBList* allUserThreads;

	static SBLKLST* blockedOnWaitList;

	static volatile int preemptionEnabled;
	volatile static int contextSwitch;
	static PCB * running;
	static Thread * idleThread;
	static Thread * getThreadById(int id);

	static void setTimerCall();

	static inline void disablePreemption();
	static inline void enablePreemption();

	static void systemInitialization();
	static void systemRestoration();
};


#endif /* SYSTEM_H_ */
