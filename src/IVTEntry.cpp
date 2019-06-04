 /*
 * IVTEntry.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "System.h"
#include "ECB.h"
#include <dos.h>

IVTEntry::IVTEntry(IVTNo num, void interrupt (*newRoutine)(...)) {
	INTERRUPT_DISABLE
	oldInterrupt = 0;
	myNumber = num;
	myEvent = 0; //Because I dont know which event is mine
#ifndef BCC_BLOCK_IGNORE
	oldInterrupt = getvect(num);
	setvect(num, newRoutine);
#endif
	System::interruptEntries[num] = this;
	INTERRUPT_ENABLE
}

IVTEntry::~IVTEntry() {
	INTERRUPT_DISABLE
	#ifndef BCC_BLOCK_IGNORE
		setvect(myNumber, oldInterrupt);
	#endif
	oldInterrupt = 0;
	System::interruptEntries[myNumber] = 0;
	myEvent = 0;
	INTERRUPT_ENABLE
}

void IVTEntry::signal(IVTNo num){
	if(System::interruptEntries[num]->myEvent != 0){
		System::interruptEntries[num]->myEvent->signal();
	}
}
void IVTEntry::activateOld(IVTNo num){
	System::interruptEntries[num]->oldInterrupt();
};
