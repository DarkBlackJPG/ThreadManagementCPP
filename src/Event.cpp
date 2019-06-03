/*
 * Event.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "System.h"
#include "ECB.h"

/*
 *
 * System::interruptEntries[ivtNo] should always be firstly initialized because, otherwise, events will not work properly
 * i.e. Events will not work at all
 *
 */
Event::Event(IVTNo ivtNo) {
	lockPreemption
	myImpl = new KernelEv(ivtNo, this);
	if(System::interruptEntries[ivtNo] != 0)
			System::interruptEntries[ivtNo]->myEvent = myImpl;
	unlockPreemption
}

Event::~Event() {
	lockPreemption
	delete myImpl;
	unlockPreemption
}

void Event::wait(){
	myImpl->wait();
};
void Event::signal(){
	myImpl->signal();
};
