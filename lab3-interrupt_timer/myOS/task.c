#include "myOS.h"
//实现实时任务使用fcfs优先级算法，非实时使用RR算法
//等待队列

void WaitQInit(void)
{
	WaitQ.head = (void*)0;	
	WaitQ.tail = (void*)0;	
}

int WaitQIsEmpty(void){
	return ((WaitQ.head==(void*)0) && (WaitQ.tail==(void*)0));
}

void tskEnqueueWait(myTCB*tsk)
{
	if(WaitQIsEmpty())
	{
		WaitQ.head = tsk;
		WaitQ.tail = tsk;
	}
	else
	{
		myTCB * test = WaitQ.head;
		unsigned int arrTime = tsk->para.arrTime;
		if(arrTime<test->para.arrTime)
		{
			tsk->next = WaitQ.head;
			WaitQ.head = tsk;
		}
		else{
		while(test->next!=(void*)0&&arrTime>=test->next->para.arrTime)
		{
			test = test -> next;
		}
		tsk->next = test->next;
		test->next = tsk;
		if(tsk->next==(void*)0)
			WaitQ.tail = tsk;
		}
	}
}

void tskDequeueWait(myTCB *tsk){
	WaitQ.head = tsk->next;
	if(tsk->para.schedPolicy==SCHED_RR)
		tskEnqueueRR(tsk);
	else
		tskEnqueueFCFS(tsk);
	if (tsk == WaitQ.tail) 
	{
		WaitQ.tail = (void*)0;
		WaitQ.head = (void*)0;
	}	
}

void tskCheckWait(void)
{
	myTCB*test = WaitQ.head;
	while(test!=(void*)0)
	{
		if(test->para.arrTime==0)//有序
			tskDequeueWait(test);
		else if(test->para.arrTime>0)
			break;
		test = WaitQ.head;
	}
}
//实现RR
void rqRRInit(myTCB*idleTsk)
{
	rqRR.head = (void*)0;	
	rqRR.tail = (void*)0;	
	rqRR.idleTsk = idleTsk;
}

int rqRRIsEmpty(void){
	return ((rqRR.head==(void*)0) && (rqRR.tail==(void*)0));
}

myTCB * nextRRTsk(void) {
	if (rqRRIsEmpty())
		return rqRR.idleTsk;
	else return rqRR.head;
}

void tskEnqueueRR(myTCB*tsk)
{
	if(rqRRIsEmpty())
	{
		rqRR.head = tsk;
		rqRR.tail = tsk;
		rqRR.tail->next=(myTCB*)0;
	}
	else
	{
		rqRR.tail->next = tsk;
		rqRR.tail = tsk;
		rqRR.tail->next=(myTCB*)0;
	}
}

void tskDequeueRR(myTCB *tsk){
	if(tsk->tcbIndex!=0)
	{
		if (tsk == rqRR.tail) 
		{
			rqRR.tail = (void*)0;
			rqRR.head = (void*)0;
		}	
		else
			rqRR.head = rqRR.head->next;
	}
}
//首先实现fcfs优先级算法
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
/*void tskEnqueueFCFS(myTCB *tsk){

	if (rqFCFSIsEmpty()) {
		rqFCFS.head = tsk;
	} else 
		rqFCFS.tail->next = tsk;

	rqFCFS.tail = tsk;
}*/
//按优先级顺序入队
void tskEnqueueFCFS(myTCB *tsk){

	if (rqFCFSIsEmpty()) {
		rqFCFS.head = tsk;
		rqFCFS.tail = tsk;
		rqFCFS.tail->next=(void*)0;
	} else 
	{//优先级数越小表示优先级越高
		tsk->next = (void*)0;
		myTCB * test = rqFCFS.head;
		unsigned int priority = tsk->para.priority;
		if(currentTsk!=rqFCFS.head)
		{
			if(priority<test->para.priority)
			{
				tsk->next = rqFCFS.head;
				rqFCFS.head = tsk;
				return;
			}
		}
		while(test->next!=(void*)0&&priority>=test->next->para.priority)
		{
			test = test -> next;
		}
		tsk->next = test->next;
		test->next = tsk;
		if(tsk->next==(void*)0)
			rqFCFS.tail = tsk;
		/*rqFCFS.tail->next = tsk;
		rqFCFS.tail = tsk;*/
	}
}
/*
 *tskDequeueFCFS: delete the first node
 */
void tskDequeueFCFS(myTCB *tsk){
	if(tsk->tcbIndex!=0)
	{
		rqFCFS.head = rqFCFS.head->next;

		if (tsk == rqFCFS.tail) 
		{
			rqFCFS.tail = (void*)0;
			rqFCFS.head = (void*)0;
		}	
	}
}

void tskStart(myTCB *tsk){
	tsk->state = TSK_RDY;
	if(tsk->para.schedPolicy==SCHED_RT_FCFS||tsk->para.schedPolicy==SCHED_RT_RR)
		tskEnqueueFCFS(tsk);
	else
		tskEnqueueRR(tsk);
}
void tskWait(myTCB *tsk)
{
	tsk->state = TSK_WAIT;
	tskEnqueueWait(tsk);
}
void tskEnd(void){
	if(currentTsk->para.schedPolicy==SCHED_RT_FCFS||currentTsk->para.schedPolicy==SCHED_RT_RR)
		tskDequeueFCFS(currentTsk);
	else
		tskDequeueRR(currentTsk);
	destroyTsk(currentTsk->tcbIndex);
}

void stack_init(unsigned long** stk,void (*task)(void)){
  *(*stk)-- = (unsigned long) 0x08;        // CS 高地址
  *(*stk)-- = (unsigned long) task;     // eip
  *(*stk)-- = (unsigned long) 0x0202;     // init eflags: IF=1, BIT1=1

  *(*stk)-- = (unsigned long) 0xAAAAAAAA;  // EAX 
  *(*stk)-- = (unsigned long) 0xCCCCCCCC;  // ECX
  *(*stk)-- = (unsigned long) 0xDDDDDDDD;  // EDX 
  *(*stk)-- = (unsigned long) 0xBBBBBBBB;  // EBX

  *(*stk)-- = (unsigned long) 0x44444444;  // ESP
  *(*stk)-- = (unsigned long) 0x55555555;  // EBP
  *(*stk)-- = (unsigned long) 0x66666666;  // ESI
  *(*stk)   = (unsigned long) 0x77777777;  // EDI 低地址    
}

void show_stack(unsigned long* stk){
	myPrintf(0x3,"%x::%x\n",stk,*stk++);  //低地址
	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	
	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	myPrintf(0x3,"%x::%x\n",stk,*stk++);

	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	myPrintf(0x3,"%x::%x\n",stk,*stk++);
	myPrintf(0x3,"%x::%x\n",stk,*stk++);  //高地址	
}

/* createTsk
 * tskBody():
 * return value: taskIndex or, if failed, -1
 */
int createTsk(void (*tskBody)(void),tskPara *para){

	myTCB * allocated=firstFreeTsk;

	if (firstFreeTsk == (void*)0) return -1;
	if(para->priority>MAX_PRIORITY_NUM)
		para->priority = MAX_PRIORITY_NUM;
	firstFreeTsk = allocated->next;		
	allocated->next = (void*)0;

	stack_init(&(allocated->stkTop),tskBody);
	allocated->para = *para;
	if(allocated->para.arrTime==0)
		tskStart(allocated);
	else
		tskWait(allocated);
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
	prevTSK_StackPtr = &(prevTsk->stkTop);
	nextTSK_StackPtr = nextTsk->stkTop;	
	CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}

void scheduleFCFS(void){
	static int idle_times=0;
	myTCB * prevTsk;
	prevTsk = currentTsk;
	currentTsk = nextFCFSTsk();
	//tskDequeueFCFS(currentTsk);
	if(prevTsk != currentTsk)
		context_switch(prevTsk,currentTsk);
	else
		enable_interrupt();
}

void scheduleRR(void)
{
	myTCB * prevTsk;
	prevTsk = currentTsk;
	currentTsk = nextRRTsk();
	//tskDequeueFCFS(currentTsk);
	if(prevTsk != currentTsk)
		context_switch(prevTsk,currentTsk);
}
void schedule(void){
	disable_interrupt();
	count1 = 0;
	tskCheckWait();
	if((rqFCFSIsEmpty()&&!rqRRIsEmpty()))
		scheduleRR();
	else
		scheduleFCFS();

}

void tskIdleBdy(void){
	int i=0;
	while(1) {		
		myPrintf(0x7,"********IDLE LOOP....................\n");
		schedule();
		busy_n_second(1);
	}
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
			thisTCB->next = (void*)0;
		else
			thisTCB->next = &tcbPool[i+1];

		// init stkTop
		thisTCB->stkTop = thisTCB->stack + STACK_SIZE -1;
	}

	//task0:idle, create and start
	idleTsk = &tcbPool[0];
	stack_init(&(idleTsk->stkTop),tskIdleBdy);
	rqFCFSInit(idleTsk);
	rqRRInit(idleTsk);
	WaitQInit();
	firstFreeTsk = &tcbPool[1];
	
	//task1:init
	tskPara para;
	initTskPara(&para);
  	createTsk(initTskBody,&para);	
}

void getTskPara(int taskID, tskPara *buffer)
{
	buffer = &tcbPool[taskID].para;
}

void setTskPara(unsigned int option, unsigned int value, tskPara *buffer)
{
	switch(option)
	{
		case PRIORITY:buffer->priority=value;break;
		case EXETIME:buffer->exeTime = value;break;
		case ARRTIME:buffer->arrTime = value;break;
		case SCHED_POLICY:buffer->schedPolicy = value;break;
		default:break;
	}
}

unsigned int getSysScheduler(void)
{
	return sysScheduler;
}
//SCHEDULER_MQ调度算法
void setSysScheduler(unsigned int what)
{
	sysScheduler = what;
}

void setSysSchedulerPara(unsigned int who, unsigned int para)
{
	if(who == SCHED_RT_RR)
		RTRRSlice = para;
	else
		RRSlice = para;
}
unsigned int getSysSchedulerPara(unsigned int who)
{
	if(who == SCHED_RT_RR)
		return RTRRSlice;
	else
		return RRSlice;
}
void initTskPara(tskPara *buffer)
{
	buffer->priority = 0;
	buffer->exeTime = 0;
	buffer->arrTime = 0;
	buffer->schedPolicy = SCHED_RT_FCFS;
}


unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;



void osStart(void){
	count1 = 101;
	init8259A();
	tick_number = 0;
	init8253();
	setSysScheduler(SCHEDULER_MQ);
	setSysSchedulerPara(SCHED_RT_RR,200);
	setSysSchedulerPara(SCHED_RR,100);
	enable_interrupt();
	myPrintf(0x7,"********SCHEDULER_MQ*********\n\n");
	TaskManagerInit();
	
	//start multitasking
	BspContext = BspContextBase + STACK_SIZE -1;
	prevTSK_StackPtr = &BspContext;
	currentTsk = nextFCFSTsk();
	//tskDequeueFCFS(currentTsk);
	nextTSK_StackPtr = currentTsk->stkTop;
	CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}


