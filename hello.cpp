
#include "Thread.h"
#include "PCB.h"
#include "System.h"
#include "semaphor.h"
#include "Event.h"
#include "IVTEntry.h"

#include "user.h"
#include "bounded.h"
#include "intLock.h"
#include "keyevent.h"

int userMain(int, char**);
int main(int argc, char* argv[]){
	System::systemInitialization();
	int res = userMain(argc, argv);
	System::systemRestoration();
	return 0;
}







