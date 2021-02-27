#ifndef  _OS_KERNEL_H     //if not define OSKERNEL_H below find engine
#define  _OS_KERNEL_H
#include <stdint.h>
#include "stm32f4xx.h"                  // Device header

void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);
uint8_t oskernelAddThreads(void(*task0)(void),                  //task is nothing but thread so we have 3 threads which need to interchange in roundrobin fashion
													 void(*task1)(void),	
														void(*task2)(void));

#endif      //ending definition of OS_KERNEL_H							 