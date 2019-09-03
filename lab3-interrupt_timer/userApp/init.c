/* init.c */
#include "../myOS/myOS.h"

int iRow=4;

void myTSK0(void){
	int j=1;	
	while(j<=10){
		busy_n_second(1);
		myPrintf(0x7,"myTSK0::%d    \n",j);
		iRow++; j++;
	}	
	tskEnd();   //the task is end
}

void myTSK1(void){
	int j=1;	
	while(j<=10){
		busy_n_second(1);
		myPrintf(0x7,"myTSK1::%d    \n",j);		
		iRow++; j++;
	}	
	tskEnd();   //the task is end
}

void myTSK2(void){
	int j=1;	
	while(j<=10){
		busy_n_second(1);
		myPrintf(0x7,"myTSK2::%d    \n",j);		
		iRow++; j++;
	}	
	tskEnd();   //the task is end
}

void initTskBody(void){
 	myPrintf(0x7,"********INIT START\n\n");
	//put_chars("********INIT START\0",0x7,1,0);
	createTsk(myTSK0);

	busy_n_second(1);
	createTsk(myTSK1);

	busy_n_second(1);
	createTsk(myTSK2);
 	myPrintf(0x7,"********INIT END\n\n");
	//put_chars("********INIT END\0",0x7,3,0);
  tskEnd();  // init end
}
