/*
 * Event.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

typedef unsigned char IVTNo;
class KernelEv;
#include "System.h"
#include "IVTEntry.h"

#define PREPAREENTRY(num, flag)\
void interrupt interruptRoutine##num(...) {\
    if (flag)\
		IVTEntry::activateOld(num);\
	IVTEntry::signal(num);\
};\
IVTEntry entry##num(num, interruptRoutine##num);

class Event {
public:

	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};

#endif /* EVENT_H_ */
