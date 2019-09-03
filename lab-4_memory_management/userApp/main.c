#include "../myOS/myOS.h"
#include "userApp.h"

void tick_hook(void){
	int _h, _m, _s;
	char hhmmss[]="hh:mm:ss\0\0\0\0";

	getWallClock(&_h,&_m,&_s);
    sprintf(hhmmss,"%02d:%02d:%02d",_h,_m,_s);
	put_chars(hhmmss,0x9,24,72);
}

void doSomeTestBefore(void){		
	setWallClock(18,59,59);		//set time 18:59:59
}

//=========================my Main===========================================
int myMain(void){

	clear_screen();
	myPrintf(0x7,"myMain:HELLO WORLD!\n");

	doSomeTestBefore();

	//prior settings
	setSysScheduler(SCHEDULER_FCFS);	

	osStart();

	//never here    
	return 0;
}