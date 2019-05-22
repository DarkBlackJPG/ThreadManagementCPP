/*
 * Idle.h
 *
 *  Created on: May 19, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"
#include "System.h"

class Idle: public Thread {
public:
	Idle() :
		Thread(512, 1) {};
protected:
	void run();
};

#endif /* IDLE_H_ */
