#ifndef __MYOS_H__
#define __MYOS_H__

#include "../userApp/userApp.h"

#define TASK_NUM (2 + USER_TASK_NUM)   // at least: 0-idle, 1-init

#define STACK_SIZE	512  // 1024*4

typedef struct myTCB {
	unsigned long state;  // 0:rdy
	int tcbIndex;
	struct myTCB * next;
	unsigned long* stkTop;
	unsigned long stack[STACK_SIZE];  
} myTCB;

myTCB tcbPool[TASK_NUM];

myTCB * idleTsk;
myTCB * currentTsk;
myTCB * firstFreeTsk;

//========ready queue FCFS==================================
#define TSK_RDY 0
typedef struct rdyQueueFCFS{
	myTCB * head;
	myTCB * tail;
	myTCB * idleTsk;
} rdyQueueFCFS;

rdyQueueFCFS rqFCFS;

int rqFCFSIsEmpty(void);
void rqFCFSInit(myTCB *idleTsk);
myTCB * nextFCFSTsk(void);

//========context switch====================================
unsigned long **prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr;
void CTX_SW(unsigned long **prev_stkTop, unsigned long *next_stkTop);

//=======task manager======================================
void schedule(void);
void destroyTsk(int takIndex);

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
int createTsk(void (*tskBody)(void));
void tskEnd(void);

int put_chars(char *msg, char color, int _row, int _col);
int sprintf(char *buf, const char *fmt, ...);
int myPrintf(int color,const char *format, ...);
void clear_screen(void);

void testScrollOneRow(void);
//=======APP, user should define this==================================
void initTskBody(void);

#endif