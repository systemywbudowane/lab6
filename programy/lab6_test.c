#include <io.h>
#include <os_cpu.h>
#include <stdio.h>
#include <sys/alt_alarm.h>
//#include <system.h>
#include <ucos_ii.h>
//#include "includes.h"

/*  --------------------------------------------------------------------------------------------- */
#define  SW0 0x00000001
#define  SW1 0x00000002
#define  SW2 0x00000004
#define  SW3 0x00000008
#define  SW4 0x00000010
#define  SW5 0x00000020
#define  SW6 0x00000040
#define  SW7 0x00000080
#define  SW8 0x00000100
#define  SW9 0x00000200
#define  SW10 0x00000400

//      pushbuttons
#define  KEY1 0x00000002
#define  KEY2 0x00000004
#define  KEY3 0x00000008
#define  KEY4 0x00000010

//      leds
#define  LED0 0x00000001
#define  LED1 0x00000002
#define  LED2 0x00000004
#define  LED3 0x00000008
#define  LED4 0x00000010
#define  LED5 0x00000020
#define  LED6 0x00000040
#define  LED7 0x00000080
#define  LED8 0x00000100
#define  LED9 0x00000200
#define  LED10 0x00000400

//      hex
#define  SEGA 0x00001
#define  SEGB 0x00002
#define  SEGC 0x00004
#define  SEGD 0x00008
#define  SEGE 0x00010
#define  SEGF 0x00020
#define  SEGG 0x00040

/*  --------------------------------------------------------------------------------------------- */
/* microC/OS-II */

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048
OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
OS_STK task3_stk[TASK_STACKSIZE];
OS_STK task4_stk[TASK_STACKSIZE];
OS_STK task5_stk[TASK_STACKSIZE];
OS_STK task6_stk[TASK_STACKSIZE];

/* Definition Mailbox handle */
OS_EVENT *SWBox1;

/* Definition of Task Priorities */
#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2
#define TASK3_PRIORITY 3
#define TASK4_PRIORITY 4
#define TASK5_PRIORITY 5
#define TASK6_PRIORITY 6

struct msg{
	int* sw;
};


void task1(void* pdata) {
	struct msg m;
	int sw = 0;
	m.sw = &sw;
    while (1) {
    	sw = IORD(SW_SLIDERS_BASE, 0);
    	OSMboxPostOpt(SWBox1, &m, OS_POST_OPT_BROADCAST);

	OSTimeDlyHMSM(0, 0, 1, 0);
    }
}


void task2(void* pdata) {
    while (1) {
    	INT8U err;

        struct msg* msg = (struct msg*)OSMboxPend(SWBox1, 0, &err);
        int sw = *(msg->sw);
        if(sw == 0)
        	IOWR(LEDS_BASE, 0, LED0);

        OSTimeDlyHMSM(0, 0, 2, 0);
    }
}

void task3(void* pdata) {
    while (1) {
    	INT8U err;

        struct msg* msg = (struct msg*)OSMboxPend(SWBox1, 0, &err);
        int sw = *(msg->sw);
        if(sw == SW0)
        	IOWR(LEDS_BASE, 0, LED1);

        OSTimeDlyHMSM(0, 0, 2, 0);
    }

}


/*  -----------------------MAIN------------------------------------------------------------------ */
int main(void) {

    SWBox1 = OSMboxCreate((void*) 0);

    OSTaskCreateExt(task1,
    NULL, (void *) &task1_stk[TASK_STACKSIZE - 1],
    TASK1_PRIORITY,
    TASK1_PRIORITY, task1_stk,
    TASK_STACKSIZE,
    NULL, 0);

    OSTaskCreateExt(task2,
    NULL, (void *) &task2_stk[TASK_STACKSIZE - 1],
    TASK2_PRIORITY,
    TASK2_PRIORITY, task2_stk,
    TASK_STACKSIZE,
    NULL, 0);

    OSTaskCreateExt(task3,
    NULL, (void *) &task3_stk[TASK_STACKSIZE - 1],
    TASK3_PRIORITY,
    TASK3_PRIORITY, task3_stk,
    TASK_STACKSIZE,
    NULL, 0);

    OSStart();
    return 0;
}
