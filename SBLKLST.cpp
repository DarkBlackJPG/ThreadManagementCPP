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
#include "SCHEDULE.H"
#include <stdio.h>


SBLKLST::~SBLKLST() {
	 Node *temp = 0;
	 System::disablePreemption();
	while (first) {
		temp = first;
		temp->nodeData->myState = PCB::READY;
		Scheduler::put(temp->nodeData);
		first = first->next;
		delete temp;
	}
	System::enablePreemption();
}

// TODO Aj proveri ovo leba ti
void SBLKLST::insert(PCB* data,Time timer, KernelSem* semaphore){
	Node * newNode = new Node(semaphore,timer, data);
	System::disablePreemption();
	cout << "Adresa newNode u globalnoj listi je " << newNode << endl;
	System::enablePreemption();
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
//	Node * temp = first;
//		System::disablePreemption();
//		temp = first;
//		while(temp){
//			cout << temp->timeRemaining << " | ";
//			temp = temp->next;
//		}
//		cout << endl;
//		System::enablePreemption();

};


//System::disablePreemption();
//		temp = first;
//		while(temp){
//			cout << temp->timeRemaining<<", "<< temp << " | ";
//			temp = temp->next;
//		}
//		cout << endl;
//		System::enablePreemption();
//		temp = first;
void SBLKLST::remove(PCB * data){
	Node *temp = first;
	Node *prev = 0;
	temp = first;
	while(temp != 0 && temp->nodeData != data){

		prev = temp;
		temp = temp->next;
	}
	if(temp == 0)
		return;

	if (temp->next){
		temp->next->timeRemaining += temp->timeRemaining; // because you can call signal
	}
	if (prev == 0)
			first = temp->next;
	else
			prev->next = temp->next;
	System::disablePreemption();
			temp = first;
			while(temp){
				cout << temp->timeRemaining<<", "<< temp << " | ";
				temp = temp->next;
			}
			cout << endl;
			System::enablePreemption();
			temp = first;
	delete temp;

};



void SBLKLST::updateList(){
	if (first == 0)
	      return;
	Node * temp = 0;
	first->timeRemaining -= 1;
	for(;first != 0 && first->timeRemaining == 0;){
		temp = first;
		first->semaphore->deblock(first->nodeData);
		first = first->next;
		delete temp;
	}
};
