#include "myOS.h"

int hh=0,mm=0,ss=0,ms=0;

void setWallClock(int h, int m, int s){
	if ( (h<0) || (h>24) ) hh=0;
	else hh = h;
	if ( (m<0) || (h>60) ) mm=0;
	else mm = m;
	if ( (s<0) || (s>60) ) ss=0;
	else ss = s;
}

void getWallClock(int *h, int *m, int *s){
	*h = hh;
	*m = mm;
	*s = ss;
}

#ifndef __TICK_HOOK__
#define __TICK_HOOK__
void tick_hook(void) {};
#endif

void tick(void){
	int temp_col, temp_row;	
	tick_number++;	
	//myPrintf(0x4,"tick::%d\n",tick_number);

	ms += 10;  // ?100HZ?
	if (ms>=1000) {ms=0;ss++;}
	if (ss>=60) {ss=0;mm++;}
	if (mm>=60) {mm=0;hh++;}
	if (hh>=24) hh=0;
	
	tick_hook();
}

//n second
void busy_n_second(int n){
	int second = 0x2000000;// 1秒内大约多少条乘法指令 毛估估
	int a=21,b=31,c;
	for(int i=0;i<n;i++)
		for(int j=0;j<second;j++) c=a*b;  //about 1sec
}
