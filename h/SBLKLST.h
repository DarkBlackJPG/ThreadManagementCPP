/*
 * SBLKLST.h
 *
 *  Created on: May 23, 2019
 *      Author: OS1
 */

#ifndef SBLKLST_H_
#define SBLKLST_H_
#include "Thread.h"
class PCB;
class KernelSem;

class SBLKLST {
	class Node{
		public:
			PCB * nodeData;
			KernelSem * semaphore;
			Time timeRemaining;
			Node* next;
			Node(KernelSem * semaphore, Time timeRemaining, PCB * nodeData, Node* next = 0) :
				semaphore(semaphore), timeRemaining(timeRemaining), nodeData(nodeData), next(next){};
		};
	Node* first, *last;
public:
	SBLKLST() :
		first(0), last(0){};

	void insert(PCB* data, Time, KernelSem* semaphore);
	void remove(PCB * data);
	void updateList();

	~SBLKLST();
};

#endif /* SBLKLST_H_ */
