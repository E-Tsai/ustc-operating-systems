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
	//遍历等待队列让任务减10ms
	if(!WaitQIsEmpty())
	{
		myTCB*test = WaitQ.head;
		while(test!=(myTCB*)0)
		{
			if(test->para.arrTime<10)
				test->para.arrTime=0;
			else
				test->para.arrTime=test->para.arrTime-10;
			if(test==WaitQ.tail)
				test = (myTCB*)0;
			else
				test = test->next;
		}
	}
	//时间片轮转
	count1 = count1 + 1;
	if(currentTsk->para.schedPolicy==SCHED_RT_RR)
	{
		if(count1==RTRRSlice)
		{
			rqFCFS.tail->next = rqFCFS.head;
			rqFCFS.tail = rqFCFS.head;
			rqFCFS.head = rqFCFS.head->next;
			schedule();
		}
	}
	else if(currentTsk->para.schedPolicy==SCHED_RR)
	{
		if(count1==RRSlice)
		{
			rqRR.tail->next = rqRR.head;
			rqRR.tail = rqRR.head;
			rqRR.head = rqRR.head->next;
			schedule();
		}
	}

	//if(count1%200==0)
	//myPrintf(0x7,"********Tick\n\n");
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
	
	osStart();

	//never here    
	return 0;
}


