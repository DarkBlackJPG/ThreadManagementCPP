#ifndef _buffer_h_
#define _buffer_h_

#include "Semaphor.h"


class Buffer{
	private:
		int *array;
		int head;
		int tail;
		int size;
		Semaphore *itemAvailable;
		Semaphore *spaceAvailable;
		Semaphore *mutex;

	public:
		Buffer(int size);
		void put(int element);
		int getElement();
		int getSize();
};

int getSize();

#endif