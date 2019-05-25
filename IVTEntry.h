/*
 * IVTEntry.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "Event.h"


class IVTEntry {
public:
	IVTEntry(IVTNo num, void interrupt (*)(...));
	~IVTEntry();
	static void signal(IVTNo num);
	static void activateOld(IVTNo num);
private:
	friend class KernelEv;
	void interrupt (*oldInterrupt)(...);
	KernelEv * myEvent;
	IVTNo myNumber;
};

#endif /* IVTENTRY_H_ */
