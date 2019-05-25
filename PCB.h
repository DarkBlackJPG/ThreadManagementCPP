/*
 * PCB.h
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include "PCBList.h"


class PCB {
public:
	enum __STATE {
		RUNNING,
		BLOCKED,
		READY,
		CREATED,
		TERMINATED
	};
	PCB();
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice);
	~PCB();

	static ID getRunningId();
	static void wrapper();
	static Thread * getThreadById(int id);

	void start();
	void waitToComplete();
	ID getId() const;



	// Global variables
	static volatile int runNonStop;
	static volatile Time currentTimeSlice;

private:
	friend int main(int, char*[]);
	friend void dispatch();
	friend void interrupt timer(...);
	friend class KernelSem;
	friend class System;
	friend class KernelEv;
	friend class Idle;
	friend class SBLKLST;
	static int idGenerator;


	PCBList* blockedOnThread;
	unsigned *myStack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	volatile int timerRelease;
	ID myId;
	Thread* myThread;
	Time myTimeSlice;
	__STATE myState;
};

#endif /* PCB_H_ */
