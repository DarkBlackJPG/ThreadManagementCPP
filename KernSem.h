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
	int signal(int n);
	KernelSem(int);
	~KernelSem();


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

protected:
	friend class SBLKLST;

	void block(Time);
	void deblock();
	void deblock(PCB*);

private:
	List* valueBlock, * timerBlockedList;
};

#endif /* KERNSEM_H_ */
