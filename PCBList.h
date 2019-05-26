/*
 * PCBList.h
 *
 *  Created on: May 18, 2019
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

class PCB;

class PCBList {
	class Node{
		public:
			PCB * nodeData;
			Node* next;
			Node(PCB * nodeData, Node* next = 0) :
				nodeData(nodeData), next(next){};
		};
	Node* first, *last;
public:
	PCBList() :
		first(0), last(0){};

	void insert(PCB* data);
	PCB * getNodeWithId(int index);
	void remove(PCB * data);
	PCB * pop_front();

	~PCBList();
};

#endif /* PCBLIST_H_ */
