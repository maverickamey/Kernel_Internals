/*
We need 3 things
//Scheduler
//TCB
//Stack
The systick timer is a 24-bit countdown timer with an auto reload. It is generally used to provide a periodic interrupt for an RTOS scheduler. The default clock source for the systick timer is the Cortex-M CPU clock.

*/
#include "osKernel.h"
#include <stdint.h>
#define NUM_OF_THREADS 		3
#define STACK_SIZE				100        //this 2 will sum upto 300 bytes = 100 * 3
#define BUS_FREQ 					16000000
#define SYSPRI3 					(*((volatile uint32_t *)0XE000ED20))  //we cant access it directly so 
#define INTCTRL						(*((volatile uint32_t *)0XE000ED20))	
	
void osSchedulerLaunch(void);

uint32_t  MILLIS_PRESCALAR;

struct tcb{
	int32_t *stackpt;
	struct tcb *nextpt;
};

typedef struct tcb tcbType;
tcbType 	tcbs[NUM_OF_THREADS];    //array where all tcb descriptor are stored just like we did in socket programming
tcbType *currentpt;     //pointer to current running thread 

uint32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];   //we have made stack using 2 dimensional array here we put PC and XPSR of each thread here

void osKernalStackInit(int i)      //i is the number of threads and here we are initializing the stack ...
{
		tcbs[i].stackpt = &TCB_STACK[i][STACK_SIZE-16];
	  //xpsr bit 24 should be set it to 1 to tell that we are running in thread mode..
	  TCB_STACK[i][STACK_SIZE-1] = 0x01000000;            //1<<24;
}	

uint8_t oskernelAddThreads(void(*task0)(void),                  //task is nothing but thread so we have 3 threads which need to interchange in roundrobin fashion
													 void(*task1)(void),	
														void(*task2)(void))
{
			//to decide now which scheduler ie which task/thread to run first 		
//we want this to happen in critical section so it needs to be atomic and then so we need to disable interrupt	
		__disable_irq();
		//lets attach all the tcbs in circular linkedlist manner as below as we know earlier.
		tcbs[0].nextpt = &tcbs[1];
		tcbs[1].nextpt = &tcbs[2];
		tcbs[2].nextpt = &tcbs[0];
	//below we are initializing thread for task 0
	  osKernalStackInit(0);
	  TCB_STACK[0][STACK_SIZE-2]=(int32_t)(task0);//as we know this is location of PC and then pt to the thread0
		//similarly now we are initializing for task 1
		osKernalStackInit(1);
	  TCB_STACK[1][STACK_SIZE-2] = (int32_t)(task1);
	  //similarly initailizing for task 2
	  osKernalStackInit(2);
	  TCB_STACK[2][STACK_SIZE-2] = (int32_t)(task2);
	currentpt = &tcbs[0];
	 __enable_irq();
		return 1;
}			


void osKernelInit(void)
{
		__disable_irq();
		MILLIS_PRESCALAR = BUS_FREQ / 1000;
}	

void osKernelLaunch(uint32_t quanta)
{
		SysTick->CTRL = 0;//disable SysTick ie resetting the control register
		SysTick->VAL = 0;  //disable,Systick ie resetting the value register
		//set priority of systick with SYSPRI3 register but we dont have access to it so we will use it address as we have initilaed at top
		//set priority to loweest ie 7
		SYSPRI3 = (SYSPRI3 & 0x00FFFFFF) | 0xE0000000;   
		SysTick->LOAD = (quanta*MILLIS_PRESCALAR)-1;//updating quanta but in milliseconds
		SysTick->CTRL =0x00000007;//enable systick back
		//now scheduler save context and load the context
		osSchedulerLaunch();
}	
//Request interupt within a thread and can be done
void osThreadYield(void)
{
	//bring address of interrupt ctrl register
	INTCTRL = 0x04000000;//setting bit 26th trigger systick
	
}	
