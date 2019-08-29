#include<stdio.h>
#include"myOS.h"
//声明
void initTskBody(void);
void Idle(void);
void tskEnd(void);
//extern void CTX_SW(unsigned long*prevSP,unsigned long*nextSP);//声明切换上下文汇编程序
//0x66f60524 in ?? ()
#define TCB_SIZE 100
#define STACK_SIZE 1000
#define IDLE 1
#define Ready 2
#define RUNNING 3
#define GIVEUP 4
unsigned long stack1[TCB_SIZE][STACK_SIZE];//二维数组 每个任务一个堆栈
void CTX_SW(unsigned long *prevSP,unsigned long *nextSP)
{
	asm
	(
		"pusha\n\t"
		"movl %%esp, %0\n\t" 
		"movl %1, %%esp\n\t" 
		"popa\n\t"
		"ret"
		:
		:"m"(prevSP),"m"(nextSP)
	);
}
typedef struct myTCB   //任务控制块 保存任务堆栈指针 还可以加状态
{
	unsigned int pid;
	unsigned int state;
	unsigned long *ptrStack;
	struct myTCB* next;
}myTCB;
//就绪队列
myTCB* tcb;//新的tcb
myTCB tcbq[TCB_SIZE];
myTCB* RearPtr = NULL;//尾部TCB
myTCB* HeadPtr = NULL;//头部TCB
//运行
myTCB* CurrentPtr = NULL;//当前指针指向的TCB,正在运行
//idletcb
//myTCB idletcb,inittcb;
int EnterQ(myTCB *tcb1)
{
	//入队
	if(HeadPtr==NULL)
	{
		HeadPtr = tcb1;
		RearPtr = tcb1;
		//tcb->pre = NULL;
		tcb1->next = NULL;
	}
	else
	{
		tcb1->next = NULL;
		tcb1->pid = RearPtr->pid+1;
		//tcb->pre = RearPtr;
		RearPtr->next = tcb1;
		RearPtr = tcb1;
	}
	return 0;
}

myTCB* DeleteQ(void)
{
	//出队:最前面一个
	myTCB* pre = HeadPtr;
	HeadPtr = HeadPtr->next;
	return pre;
}
//栈初始化  保存任务的上下文信息
unsigned long* stack;
/*void stack_init(unsigned long** stk,void (*task)(void))
{
	*(*stk)--=(unsigned long)0x08;
	*(*stk)--=(unsigned long)task;
	*(*stk)--=(unsigned long)0xAAAAAAAA;
	*(*stk)--=(unsigned long)0xCCCCCCCC;
	*(*stk)--=(unsigned long)0xDDDDDDDD;
	*(*stk)--=(unsigned long)0xBBBBBBBB;
	*(*stk)--=(unsigned long)0x44444444;
	*(*stk)--=(unsigned long)0x55555555;
	*(*stk)--=(unsigned long)0x66666666;
	*(*stk)= (unsigned long)0x77777777;
}*/

//任务创建：由于使用FIFO算法，应该把TCB挂到队列上
int createTsk(void(*task)(void))
{
	if(tcb->pid ==0)
	{
	if(HeadPtr==NULL)
		tcb->pid = 1;
	else
		tcb->pid = RearPtr->pid+1;
	stack = (stack1[tcb->pid-1]+999);
	*stack--=(unsigned long)0x08;
	*stack--=(unsigned long)task;
	*stack--=(unsigned long)0xAAAAAAAA;
	*stack--=(unsigned long)0xCCCCCCCC;
	*stack--=(unsigned long)0xDDDDDDDD;
	*stack--=(unsigned long)0xBBBBBBBB;
	*stack--=(unsigned long)0x44444444;
	*stack--=(unsigned long)0x55555555;
	*stack--=(unsigned long)0x66666666;
	*stack= (unsigned long)0x77777777;
	//stack_init(stack,task);
	tcb->ptrStack = stack;//信息
	if(task==Idle)
		tcb->state = IDLE;
	else
		tcb->state = Ready;
	//stack_init(&(tcb.ptrStack),task);//pc指向task
	EnterQ(tcb);
	if(tcb->pid % TCB_SIZE == 0)
		tcb = tcbq + TCB_SIZE - 1;
	else 
		tcb--;
	}
	return 0;
}

int destroyTsk(myTCB*giveup_tcb)
{
	giveup_tcb->pid = 0;
	giveup_tcb->state = GIVEUP;
	return 1;
}
//FIFO
int schedule()
{
		/*//如果切换的是idle，那就把idletcb放到就绪队列尾部
		if(CurrentPtr->ptrStack == idletcb.ptrStack)
			EnterQ(&idletcb);*/
		if(HeadPtr!=NULL)
		{
			
		unsigned long*prevSP = CurrentPtr->ptrStack;
		destroyTsk(CurrentPtr);
		CurrentPtr = DeleteQ();
		if(CurrentPtr->state!=IDLE)
			CurrentPtr->state = RUNNING;
		unsigned long*nextSP = CurrentPtr->ptrStack;
		CTX_SW(prevSP,nextSP);
		//切换上下文
		}
		else if(CurrentPtr->state !=IDLE)
		{
			createTsk(Idle);
			Idle();
		}
		return 0;
}

void osStart(void)
{
	//initial:let currentptr point to the IDLE task;
	//idle
	//stack_init(&(idletcb.ptrStack),Idle);
	//init
	//stack= (unsigned long*)0xffffffc0; 
	//unsigned long* test;
	//*test = (unsigned long*)0xffffffc0;
	//*test =(unsigned long)0x08;
	//stack = stack1;
	tcb = tcbq + TCB_SIZE - 1;
	for(int i = 0;i<TCB_SIZE;i++)
	{
		tcbq[i].pid = 0;
		tcbq[i].state = GIVEUP;
	}
	createTsk(initTskBody);
	//createTsk(Idle);
	CurrentPtr = HeadPtr;
	CurrentPtr->state = RUNNING;
	HeadPtr = HeadPtr->next;
	initTskBody();
}

void Idle(void)
{
	while(1)
		schedule();
}

void tskEnd(void)
{
	schedule();
}
