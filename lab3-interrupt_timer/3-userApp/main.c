#include "../myOS/myOS.h"
#include "userApp.h"

void testScrollOneRow(void){
    int i,j;
    for (i=0;i<80;i++){
			for (j=0;j<i;j++){
				myPrintf(0x2," ");
			}

			myPrintf(0x2,"%d\n",i%10);		
			busy_n_second(1);			
		}        
}

void testFormatIO(void){
	myPrintf(0x7,"testFormatIO: 02d: %02d %02d",33,3);
}

void tick_hook(void){
	int _h, _m, _s;
	char hhmmss[]="hh:mm:ss\0\0\0\0";

	getWallClock(&_h,&_m,&_s);
  sprintf(hhmmss,"%02d:%02d:%02d",_h,_m,_s);
	put_chars(hhmmss,0x9,24,72);
}

void doSomeTestBefore(void){
	myPrintf(0x7,"myMain:HELLO WORLD!\n");	
	setWallClock(18,59,59);
	//testScrollOneRow(); while(1);
	//testFormatIO(); while(1);
}

//=========================my Main===========================================
int myMain(void){

	clear_screen();
	
	doSomeTestBefore();

	//prior settings
	setSysScheduler(SCHEDULER_FCFS);//PRIORITY); //RR); //PRIORITY0); //SJF); FCFS);
	//setSysSchedulerPara(SCHED_RR_SLICE,1000);

	osStart();

	//never here    
	return 0;
}


