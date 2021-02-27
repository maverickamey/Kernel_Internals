#include "osKernel.h"
#define QUANTA  100000
volatile uint32_t count0,count1,count2;
uint32_t semaphore1,semaphore2;
volatile uint32_t shared_variable=0; 
void Task0(void)
{
	while(1)
	{
		count0++;
	}	
}	

void Task1(void)
{
		while(1)
		{
			count1++;
			osSignalwait(&semaphore2);
			shared_variable--;
			osSignalSet(&semaphore1);
			
		}
}	

void Task2(void)
{
	while(1)
	{
		count2++;
		osSignalwait(&semaphore1);
		shared_variable++;
		osSignalSet(&semaphore2);
	}
}
int main()
{
	osSemaphoreInit(&semaphore1,1);
	osSemaphoreInit(&semaphore2,1);
	osKernelInit();
	oskernelAddThreads(&Task0,&Task1,&Task2);
	osKernelLaunch(QUANTA);
	
}	

