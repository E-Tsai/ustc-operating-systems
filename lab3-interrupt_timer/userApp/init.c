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

void myTSK3(void){
	int j=1;	
	while(j<=10){
		myPrintf(0x7,"myTSK3::%d    \n",j);		
		busy_n_second(1);
		j++;
	}	
	tskEnd();   //the task is end
}

void myTSK4(void){
	int j=1;	
	while(j<=10){
		myPrintf(0x7,"myTSK4::%d    \n",j);		
		busy_n_second(1);
		j++;
	}	
	tskEnd();   //the task is end
}

void initTskBody(void){
	
 	myPrintf(0x7,"********INIT START\n\n");
	//put_chars("********INIT START\0",0x7,1,0);


	tskPara tskPara[5];
	//测试用例1

	initTskPara(&tskPara[0]);
	setTskPara(PRIORITY, 12, &tskPara[0]);	 //ONLY FOR PRIORITY
	setTskPara(ARRTIME, 116, &tskPara[0]);    //for arrTime
	setTskPara(SCHED_POLICY, SCHED_RR, &tskPara[0]); //only for selected policy
	createTsk(myTSK0,&tskPara[0]);

	initTskPara(&tskPara[1]);
	setTskPara(PRIORITY, 8, &tskPara[1]);
	setTskPara(ARRTIME, 150, &tskPara[1]);    //for arrTime
	setTskPara(SCHED_POLICY, SCHED_RR, &tskPara[1]); //only for selected policy
	createTsk(myTSK1,&tskPara[1]);

	initTskPara(&tskPara[2]);
	setTskPara(PRIORITY, 3, &tskPara[2]);
	setTskPara(ARRTIME, 250, &tskPara[2]);    //for arrTime
	createTsk(myTSK2,&tskPara[2]);

	initTskPara(&tskPara[3]);
	setTskPara(PRIORITY, 1, &tskPara[3]);
	setTskPara(ARRTIME, 5250, &tskPara[3]);    //for arrTime
	setTskPara(SCHED_POLICY, SCHED_RR, &tskPara[3]); //only for selected policy
	createTsk(myTSK3,&tskPara[3]);
 	myPrintf(0x7,"********INIT END\n\n");
	//put_chars("********INIT END\0",0x7,3,0);
  tskEnd();  // init end
}
