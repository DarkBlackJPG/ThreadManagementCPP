/*
 * PCBList.cpp
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#include "PCBList.h"
#include "System.h"
#include <iostream.h>
#include "PCB.h"
PCBList::~PCBList() {
	 Node *temp = 0;
	while (first) {
		temp = first;
		first = first->next;
		delete temp;
	}

}

void PCBList::insert(PCB* data){
	Node * newNode = new Node(data);
	if(!first){
		first = last = newNode;
	} else {
		last->next = newNode;
		//INTERRUPT_DISABLE
		//cout << "Nit sa id "<< data->getId() << "ima last na" << last << "i last next " << last->next << "\n";
		//INTERRUPT_ENABLE
		last = last->next;
	}
};
PCB * PCBList::getNodeWithId(int index){
	if(index < 0)
		return 0;
	if(first == 0)
		return 0;

	Node* temp = first;
	for(int i = 0; i != index && temp != 0; temp = temp->next, i++);
	if(temp == 0)
		return 0;
	return temp->nodeData;
};
void PCBList::remove(PCB * data){
	Node *temp = first;
	Node *prev = 0;
	while(temp != 0 && temp->nodeData != data)
		temp = temp->next;
	if(temp == 0)
		return;
	if (prev == 0)
		first = temp->next;
	else
		prev->next = temp->next;

	if (temp->next == 0)
		last = prev;
	delete temp;
};
PCB * PCBList::pop_front(){
	if (!first)
		return 0;
	Node *temp = first;
	first = first->next;
	if (first == 0)
		last = 0;
	PCB *dat = temp->nodeData;
	delete temp;
	return dat;

};
PCB * PCBList::pop_back(){
	if(last == 0)
		return 0;
	Node * temp = first;
	while(temp->next != last){
		temp = temp->next;
	}
	last = temp;
	temp = last->next;
	PCB * dat = temp->nodeData;
	delete temp;
	return dat;
};
