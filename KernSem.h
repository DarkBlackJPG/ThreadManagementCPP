/*
 * KernSem.h
 *
 *  Created on: May 22, 2019
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "semaphor.h"

class PCB;

class KernelSem {
	//friend class Semaphore;
	volatile int value;

public:
	int val();
	int wait(Time maxTimeToWait);
	void signal(int n);
	KernelSem(int);
	~KernelSem();

protected:
	friend class SBLKLST;
	class List {
	public:
		class Node {
				public:
					Node(PCB* data, Node* next = 0) :
						data(data), next(next){};
					~Node(){};
					PCB* data;
					Node* next;
				};
		List();
		~List();
		void add(PCB* data);
		PCB * getPCB();
		void removePCB(PCB* data);
	private:
		friend class KernelSem;
		Node* first, *last;
	};
	void block(Time);
	void deblock();
	void deblock(PCB*);

private:
	List* valueBlock, * timerBlockedList;
};

#endif /* KERNSEM_H_ */
