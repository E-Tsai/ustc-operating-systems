#include "myOS.h"

void rqFCFSInit(myTCB* idleTsk) {
	rqFCFS.head = (void*)0;	
	rqFCFS.tail = (void*)0;	
	rqFCFS.idleTsk = idleTsk;
}

int rqFCFSIsEmpty(void){
	return ((rqFCFS.head==(void*)0) && (rqFCFS.tail==(void*)0));
}

myTCB * nextFCFSTsk(void) {
	if (rqFCFSIsEmpty())
		return rqFCFS.idleTsk;
	else return rqFCFS.head;
}

/*
 *tskEnqueueFCFS: insert into the tail node
 */
void tskEnqueueFCFS(myTCB *tsk){

	if (rqFCFSIsEmpty()) {
		rqFCFS.head = tsk;
	} else 
		rqFCFS.tail->next = tsk;

	rqFCFS.tail = tsk;
}

/*
 *tskDequeueFCFS: delete the first node
 */
void tskDequeueFCFS(myTCB *tsk){
	rqFCFS.head = rqFCFS.head->next;

	if (tsk == rqFCFS.tail) 
		rqFCFS.tail = (void*)0;	
}

void tskStart(myTCB *tsk){
	tsk->state = TSK_RDY;
	tskEnqueueFCFS(tsk);
}

void tskEnd(void){
	tskDequeueFCFS(currentTsk);
	destroyTsk(currentTsk->tcbIndex);
}

void stack_init(unsigned long** stk,void (*task)(void)){
  *(*stk)-- = (unsigned long) 0x08;        // CS 高地址
  *(*stk)-- = (unsigned long) task;     // eip
  *(*stk)-- = (unsigned long) 0xAAAAAAAA;  // EAX 
  *(*stk)-- = (unsigned long) 0xCCCCCCCC;  // ECX
  *(*stk)-- = (unsigned long) 0xDDDDDDDD;  // EDX 
  *(*stk)-- = (unsigned long) 0xBBBBBBBB;  // EBX
  *(*stk)-- = (unsigned long) 0x44444444;  // ESP
  *(*stk)-- = (unsigned long) 0x55555555;  // EBP
  *(*stk)-- = (unsigned long) 0x66666666;  // ESI
  *(*stk) = (unsigned long) 0x77777777;  // EDI 低地址    

}

/* createTsk
 * tskBody():
 * return value: taskIndex or, if failed, -1
 */
int createTsk(void (*tskBody)(void)){

	myTCB * allocated=firstFreeTsk;

	if (firstFreeTsk == (void*)0) return -1;

	firstFreeTsk = allocated->next;		
	allocated->next = (void*)0;

	stack_init(&(allocated->stkTop),tskBody);
	tskStart(allocated);

	return allocated->tcbIndex;
}

/* destroyTsk
 * takIndex:
 * return value: void
 */
void destroyTsk(int takIndex){

	// back to freelist
	tcbPool[takIndex].next = firstFreeTsk;
	firstFreeTsk = &tcbPool[takIndex]; 
	schedule();
}

void context_switch(myTCB *prevTsk, myTCB *nextTsk){	
	prevTSK_StackPtr = prevTsk->stkTop;
	nextTSK_StackPtr = nextTsk->stkTop;
	CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}

void scheduleFCFS(void){
	myTCB * prevTsk;
	prevTsk = currentTsk;
	currentTsk = nextFCFSTsk();
	context_switch(prevTsk,currentTsk);
}

void schedule(void){
	scheduleFCFS();
}

void tskIdleBdy(void){

	char *message1 = "********************************\0";
  char *message2 = "*        Idle  Idle !          *\0";
	int row=0,col=0;

	row=22; col=4; put_chars(message1,WHITE,&row,&col);
	row++; col=4; put_chars(message2,WHITE,&row,&col);
	row++; col=4; put_chars(message1,WHITE,&row,&col);

	while(1); // {//do something else	schedule();	}
}

void TaskManagerInit(void){
	int i;
	myTCB *thisTCB;
	
	for (i=0;i<TASK_NUM;i++){
	  thisTCB=&tcbPool[i];

		// init index
		thisTCB->tcbIndex = i;
		
		// init freelist
		if (i==TASK_NUM-1)
			thisTCB->next = (myTCB *)0;
		else
			thisTCB->next = &tcbPool[i+1];

		// init stkTop
		thisTCB->stkTop = thisTCB->stack + STACK_SIZE -1;
	}

	//task0:idle, create and start
	idleTsk = &tcbPool[0];
	stack_init(&(idleTsk->stkTop),tskIdleBdy);	
	rqFCFSInit(idleTsk);

	firstFreeTsk = &tcbPool[1];
	
	//task1:init
  createTsk(initTskBody);	
}

unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;

void osStart(void){
	TaskManagerInit();
	
	//start multitasking
  BspContext = BspContextBase + STACK_SIZE -1;
	prevTSK_StackPtr = BspContext;
	currentTsk = nextFCFSTsk();
	nextTSK_StackPtr = currentTsk->stkTop;
  CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}
