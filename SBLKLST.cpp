/*
 * SBLKLST.cpp
 *
 *  Created on: May 23, 2019
 *      Author: OS1
 */

#include "SBLKLST.h"
#include "System.h"
#include "KernSem.h"
#include "PCB.h"
#include <iostream.h>
#include <stdio.h>
SBLKLST::~SBLKLST() {
	 Node *temp = 0;
	while (first) {
		temp = first;
		first = first->next;
		delete temp;
	}
}

// TODO Aj proveri ovo leba ti
void SBLKLST::insert(PCB* data,Time timer, KernelSem* semaphore){
	Node * newNode = new Node(semaphore,timer, data);
	if(!first){
		first = last = newNode;
		return;
	}
	if(first->timeRemaining < newNode->timeRemaining){
		Node * temp = first, *prev = 0;
		while(temp && temp->timeRemaining <= timer){
			timer -= temp->timeRemaining;
			prev = temp;
			temp = temp->next;
		}
		newNode->next = temp;
		newNode->timeRemaining = timer;
		if(prev) {
			prev->next = newNode;
		} else { // This should never happen

			return;
		}
		 if (temp)
		        temp->timeRemaining-= timer;

	} else {
		newNode->next = first;
		first = newNode;
		first->next->timeRemaining -= first->timeRemaining;

	}

};



void SBLKLST::remove(PCB * data){
	Node *temp = first;

	Node *prev = 0;
	while(temp != 0 && temp->nodeData != data)
		temp = temp->next;
	if(temp == 0)
		return;
	if (prev == 0)
		first = temp->next;
	if (temp->next){
		temp->next->timeRemaining += temp->timeRemaining;
		prev->next = temp->next;
	}
	delete temp;

};



void SBLKLST::updateList(){

	if (first == 0)
	      return;
	Node * temp = 0;
	first->timeRemaining -= 1;
	for(;first != 0 && first->timeRemaining == 0; first = first->next){
		temp = first;
		first->semaphore->deblock(first->nodeData);
		delete temp;
	}
//	System::disablePreemption();
//	temp = first;
//	while(temp){
//		cout << temp->timeRemaining << " | ";
//		temp = temp->next;
//	}
//	cout << endl;
//	System::enablePreemption();


};
