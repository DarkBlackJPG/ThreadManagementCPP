/*
 * ECB.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef ECB_H_
#define ECB_H_

#include "Event.h"
#include "IVTEntry.h"
#include "PCB.h"

class KernelEv {
public:
	KernelEv(IVTNo ivtNo, Event * myEvent);
	void wait();
	void signal();
private:
	IVTNo myNo;
	Event * myEvent;
	PCB* owner;
	volatile int value;
};

#endif /* ECB_H_ */
