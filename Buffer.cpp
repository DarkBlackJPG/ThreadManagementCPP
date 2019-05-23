#include "Buffer.h"


Buffer::Buffer(int size_){
	array = new int[size_];
	size = size_;
	head = tail = 0;
	itemAvailable = new Semaphore(0);
	spaceAvailable = new Semaphore(size);
	mutex = new Semaphore(1);
}

void Buffer::put(int element){
	spaceAvailable->wait(0);
	mutex->wait(0);
	array[tail] = element;
	tail = (tail+1)%size;
	mutex->signal();
	itemAvailable->signal();
}

int Buffer::getElement(){
	itemAvailable->wait(0);
	mutex->wait(0);
	int element = array[head];
	head = (head+1)%size;
	mutex->signal();
	spaceAvailable->signal();
	return element;
}

int Buffer::getSize(){
	return size;
}