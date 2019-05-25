/*
 * Event.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "System.h"
#include "ECB.h"


Event::Event(IVTNo ivtNo) {
	System::disablePreemption();
	myImpl = new KernelEv(ivtNo, this);
	System::enablePreemption();
}

Event::~Event() {
	System::disablePreemption();
	delete myImpl;
	System::enablePreemption();
}

void Event::wait(){
	myImpl->wait();
};
void Event::signal(){
	myImpl->signal();
};
