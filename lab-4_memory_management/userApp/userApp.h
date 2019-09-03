#ifndef __USER_APP_H__
#define __USER_APP_H__

//========for tick_hook==================================================
#define __TICK_HOOK__
void tick_hook(void);

//=======APP, user should define this==================================
#define USER_TASK_NUM 4   // init + myTSK0-2
#define MAX_USER_PRIORITY_NUM       15  // priority number: 0..16; init <- 0
#endif
