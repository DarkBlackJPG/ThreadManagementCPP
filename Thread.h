/*
 * Thread.h
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time;
const Time defaultTimeSlice = 2;

typedef int ID;

class PCB;

static volatile unsigned tsp;
static volatile unsigned tss;
static volatile unsigned tbp;

class Thread {
public:

	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();

	static ID getRunningId();
	static Thread* getThreadById(ID id);

protected:
	friend class PCB;
	friend void dispatch();
	friend void interrupt timer(...);
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {};
private:
	PCB* myPCB;
};

void dispatch();

static volatile int currentTime = defaultTimeSlice;




#endif /* THREAD_H_ */
