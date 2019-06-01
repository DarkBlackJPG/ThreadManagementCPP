
#include "Thread.h"
#include "PCB.h"
#include "System.h"
#include "semaphor.h"
#include "Event.h"
#include "IVTEntry.h"
#include <stdlib.h>
#include <IOSTREAM.H>
#include <DOS.H>

#include "user.h"
#include "bounded.h"
#include "intLock.h"
#include "keyevent.h"

int userMain(int, char**);
int main(int argc, char* argv[]){

	System::systemInitialization();
	int res = userMain(argc, argv);
	System::systemRestoration();

#ifndef BCC_BLOCK_IGNORE
		INTERRUPT_DISABLE
		cout << "\n**********\n*HappyEnd*\n**********\n";
		INTERRUPT_ENABLE
#endif
	return 0;
}







