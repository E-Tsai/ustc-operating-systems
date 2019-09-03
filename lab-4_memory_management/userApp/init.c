/* init.c */
#include "../myOS/myOS.h"

void myTSK0(void){  
	//======for malloc===============================
	char*buf1 = (char*)malloc(19);
	char*buf2 = (char*)malloc(24);
	for(int i=0;i<17;i++) *(buf1+i) = '*';
	*(buf1+17) = '\n';
	*(buf1+18) = '\000';

	for(int i=0;i<22;i++) *(buf2+i) = '#';
	*(buf2+22) = '\n';
	*(buf2+23) = '\000';

	myPrintf(0x7,buf1);
	myPrintf(0x7,buf2);
	myPrintf(0x7,"TSK0: BUF1: 0x%x:0x%x:19\n",&buf1,(unsigned long)buf1);	
	myPrintf(0x7,"TSK0: BUF2: 0x%x:0x%x:24\n",&buf2,(unsigned long)buf2);		
	myPrintf(0x7,"\n");
	free((unsigned long)buf1);
	free((unsigned long)buf2);

	tskEnd();   //the task is end
}

void myTSK1(void){
	//======for malloc===============================
	char*buf1 = (char*)malloc(11);
	char*buf2 = (char*)malloc(21);
	for(int i=0;i<9;i++) *(buf1+i) = '+';
	*(buf1+9) = '\n';
	*(buf1+10) = '\000';

	for(int i=0;i<19;i++) *(buf2+i) = ',';
	*(buf2+19) = '\n';
	*(buf2+20) = '\000';

	myPrintf(0x7,buf1);
	myPrintf(0x7,buf2);
	myPrintf(0x7,"TSK1: BUF1: 0x%x:0x%x:11\n",&buf1,(unsigned long)buf1);	
	myPrintf(0x7,"TSK1: BUF2: 0x%x:0x%x:21\n",&buf2,(unsigned long)buf2);			
	free((unsigned long)buf1);
	free((unsigned long)buf2);
	tskEnd();   //the task is end
}

void myTSK2(void){
	int i=0x1000;
	unsigned long x,x1,xHandler;
	int psize = 31;
	int n = 4;
	unsigned long y[4];
	int tsize;

	while(1){
		x = malloc(i); //myPrintf(0x7,"X:0x%x:0x%x ",x,i);
		if(x) free(x);
		else break;
		i+=0x1000;
	}
	myPrintf(0x7,"TSK2: MAX_MALLOC_SIZE: 0x%x\n",i);	

	//======for dPartition===============================
	tsize = 0x100;
	x = malloc(tsize); 
	if (x){
		myPrintf(0x7,"dP:0x%x:0x%x\n",x,tsize);

		xHandler = dPartitionInit(x,tsize);

		i=0x10;
		while(1){
			x1 = dPartitionAlloc(xHandler,i); 
			if(x1) {
				myPrintf(0x7,"EMB:0x%x:0x%x\n",x1,i);				
				dPartitionFree(xHandler,x1);
			}	else break;
			i+=0x10;
		}

		x1 = dPartitionAlloc(xHandler,0x10);
		if(x1) {
			myPrintf(0x7,"EMB_again:0x%x:0x10\n",x1);
			dPartitionFree(xHandler,x1);
		}
		
		free(x);
	}	else myPrintf(0x7,"TSK2: MALLOC FAILED, CAN't TEST dPartition\n");
	

	//======for eFPartition===============================
	tsize = eFPartitionTotalSize(psize,n);
	x = malloc(tsize); myPrintf(0x7,"X:0x%x:%d \n",x,tsize);
	if (x) {
		xHandler = eFPartitionInit(x,psize,n);
		x1=eFPartitionAlloc(xHandler); myPrintf(0x7,"X1:0x%x \n",x1);
		eFPartitionFree(xHandler,x1);

		x1=eFPartitionAlloc(xHandler); myPrintf(0x7,"X2:0x%x \n",x1);
		eFPartitionFree(xHandler,x1);

		for(i=0;i<n;i++) {
			y[i] = eFPartitionAlloc(xHandler); myPrintf(0x7,"Y%d:0x%x \n",i,y[i]);
		}

		for(i=0;i<n;i++) {
			eFPartitionFree(xHandler,y[i]);
		}

		x1=eFPartitionAlloc(xHandler); myPrintf(0x7,"X3:0x%x\n",x1);
		eFPartitionFree(xHandler,x1);
	} else myPrintf(0x7,"TSK2: MALLOC FAILED, CAN't TEST eFPartition\n");
	
	tskEnd();  //the task is end
}

void testMem0(void){ 	
	createTsk(myTSK0,NULL);
	createTsk(myTSK1,NULL);
	createTsk(myTSK2,NULL);	
}

void initTskBody(void){	
	myPrintf(0x7,"********INIT START\n");
	
	testMem0();	

 	myPrintf(0x7,"********INIT END\n\n");	
  	tskEnd();  // init end
}
