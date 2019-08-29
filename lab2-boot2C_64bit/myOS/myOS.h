//函数声明
void osStart(void);

int createTsk(void (*tskBody)(void));

void tskEnd(void);

void initTskBody(void);

//#define USER_TASK_NUM
