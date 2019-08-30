#ifndef __MYOS_H__
#define __MYOS_H__

#include "../userApp/userApp.h"
//#include "dLinkList.h"
//#include "bitmap.h"

#define NULL (void*)0

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define STACK_SIZE	512  // 1024*4

typedef struct tskPara{
	unsigned int priority;
	unsigned int exeTime;
	unsigned int arrTime;
	unsigned int schedPolicy;
} tskPara;
#define TSK_RDY 0
typedef struct myTCB {
	unsigned long state;  // 0:rdy
	int tcbIndex;
	struct myTCB * next;
	unsigned long* stkTop;
	unsigned long stack[STACK_SIZE]; 
	tskPara para; 	
} myTCB;

myTCB tcbPool[TASK_NUM];

myTCB * idleTsk;
myTCB * currentTsk;
myTCB * firstFreeTsk;
//system 
unsigned int sysScheduler;
//========task para=========================================
#define PRIORITY	 1
#define EXETIME		 2
#define ARRTIME		 3
#define SCHED_POLICY 4
#define TSK_WAIT 1
#define UNDEF_PRIORITY 0xFFFFFFFF
#define MAX_EXETIME	0xFFFFFFFF

#ifdef MAX_USER_PRIORITY_NUM
// idle = MAX_USER_PRIORITY_NUM+1
#define MAX_PRIORITY_NUM MAX_USER_PRIORITY_NUM+1
#else
#define MAX_PRIORITY_NUM UNDEF_PRIORITY
#endif

tskPara initTsk_para;
extern tskPara defaultTskPara;

void _setTskPara(myTCB *task, tskPara *para);

void initTskPara(tskPara *buffer);
void setTskPara(unsigned int option, unsigned int value, tskPara *buffer);//要实现
void getTskPara(int taskID, tskPara *buffer);

void tskStart(myTCB *tsk);
void tskStartDelayed(myTCB* tsk);

void initArrList(void);
//========scheduler manager==================================
#include "mytask_sched.h"

//========ready queue FCFS==================================
extern struct scheduler scheduler_FCFS;

typedef struct rdyQueueFCFS{
	myTCB * head;
	myTCB * tail;
	myTCB * idleTsk;
} rdyQueueFCFS;

rdyQueueFCFS rqFCFS;
int rqFCFSIsEmpty(void);
void rqFCFSInit(myTCB *idleTsk);
void tskEnqueueFCFS(myTCB *tsk);
void tskDequeueFCFS(myTCB *tsk);
myTCB * nextFCFSTsk(void);
unsigned int count1;
//===========RR=============
extern unsigned int defaultSlice;
extern unsigned int defaultRtSlice;
unsigned int RRSlice;
unsigned int RTRRSlice;
struct scheduler scheduler_RR;
typedef struct rdyQueueRR{
	myTCB * head;
	myTCB * tail;
	myTCB * idleTsk;
} rdyQueueRR;
rdyQueueRR rqRR;//RR队列
void rqRRInit(myTCB*idleTsk);
int rqRRIsEmpty(void);
myTCB * nextRRTsk(void);
void tskEnqueueRR(myTCB*tsk);
void tskDequeueRR(myTCB *tsk);
/*//=======ready queue SJF============================
void initLeftExeTime_sjf(myTCB* tsk);
extern struct scheduler scheduler_SJF;

//=======ready queue PRIO0============================
extern struct scheduler scheduler_PRIO0;

//=======ready queue PRIO=============================
extern struct scheduler scheduler_PRIO;
*/

//========context switch====================================
unsigned long **prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr;
void CTX_SW(unsigned long **prev_stkTop, unsigned long *next_stkTop);
void context_switch(myTCB *prevTsk, myTCB *nextTsk);

//=======task manager======================================
void destroyTsk(int takIndex);
void TaskManagerInit(void);

//========task arriving=====================================
void tick_hook_arr(void);

//=======interrupt & timer manager======================================
void init8259A();
void init8253();
void enable_interrupt();
void disable_interrupt();
void tick();
volatile int tick_number;

void setWallClock(int h, int m, int s);
void getWallClock(int *h, int *m, int *s);

void busy_n_second(int n);
//=======OS API to user ============================================
void osStart(void);
//int createTsk(void (*tskBody)(void));
int createTsk(void (*tskBody)(void), tskPara *para);
void tskEnd(void);

int put_chars(char *msg, char color, int _row, int _col);
int sprintf(char *buf, const char *fmt, ...);
int myPrintf(int color,const char *format, ...);
void clear_screen(void);

void testScrollOneRow(void);
//=======APP, user should define this==================================
void initTskBody(void);
//WAIT
typedef struct Wait
{
	myTCB* head;
	myTCB * tail;
}WaitQueue;
WaitQueue WaitQ;
void WaitQInit(void);
int WaitQIsEmpty(void);
void tskEnqueueWait(myTCB*tsk);
void tskDequeueWait(myTCB *tsk);
#endif