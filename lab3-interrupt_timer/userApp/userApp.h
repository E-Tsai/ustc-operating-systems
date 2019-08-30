#ifndef __USER_APP_H__
#define __USER_APP_H__

//========output API, user defined=======================================
//#define WHITE 0x7
//void put_chars(char *msg, char color, int *_row, int *_col);

//========for tick_hook==================================================
#define __TICK_HOOK__
void tick_hook(void);

//=======APP, user should define this==================================
#define USER_TASK_NUM 4   // init + myTSK0-2

#endif
