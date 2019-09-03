/* init.c */
#include "../myOS/myOS.h"

void myTSK0(void){
	int j=1;	
	while(j<=10){		
		myPrintf(0x7,"myTSK0::%d    \n",j);
		busy_n_second(1);
		j++;
	}	
	tskEnd();   //the task is end
}

void myTSK1(void){
	int j=1;	
	while(j<=10){
		myPrintf(0x7,"myTSK1::%d    \n",j);		
		busy_n_second(1);
		j++;
	}	
	tskEnd();   //the task is end
}

void myTSK2(void){
	int j=1;	
	while(j<=10){
		myPrintf(0x7,"myTSK2::%d    \n",j);		
		busy_n_second(1);
		j++;
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
	tskPara tskPara[5];

 	myPrintf(0x7,"********INIT START\n\n");
	
	initTskPara(&tskPara[0]);
	setTskPara(EXETIME, 1200, &tskPara[0]);  //ONLY FOR SJF
	setTskPara(PRIORITY, 12, &tskPara[0]);	 //ONLY FOR PRIORITY
	setTskPara(ARRTIME, 6, &tskPara[0]);    //for arrTime
	//setTskPara(SCHED_POLICY, SCHED_RR, &tskPara[0]); //only for selected policy
	createTsk(myTSK0,&tskPara[0]);

	initTskPara(&tskPara[1]);
	setTskPara(EXETIME, 80, &tskPara[1]);
	setTskPara(PRIORITY, 8, &tskPara[1]);
	setTskPara(ARRTIME, 150, &tskPara[1]);    //for arrTime
	setTskPara(SCHED_POLICY, SCHED_RR, &tskPara[1]); //only for selected policy
	createTsk(myTSK1,&tskPara[1]);

	if (getSysScheduler()!=SCHEDULER_PRIORITY0){
		initTskPara(&tskPara[2]);
		setTskPara(EXETIME, 50, &tskPara[2]);
		setTskPara(PRIORITY, 8, &tskPara[2]);
		setTskPara(ARRTIME, 550, &tskPara[2]);    //for arrTime
		setTskPara(SCHED_POLICY, SCHED_RR, &tskPara[2]); //only for selected policy
		createTsk(myTSK2,&tskPara[2]);

		initTskPara(&tskPara[3]);
		setTskPara(EXETIME, 30, &tskPara[3]);
		setTskPara(PRIORITY, 8, &tskPara[3]);
		setTskPara(ARRTIME, 550, &tskPara[3]);    //for arrTime
		//setTskPara(SCHED_POLICY, SCHED_RR, &tskPara[2]); //only for selected policy
		createTsk(myTSK3,&tskPara[2]);
	}

 	myPrintf(0x7,"********INIT END\n\n");	
  	tskEnd();  // init end
}
