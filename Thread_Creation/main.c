#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx.h"                  // Device header

int32_t count1,count2,count3;
//Step1:Define the threads
void thread1(void const *arguement)
{
		while(1)
		{
				++count1;
		
		}
}	

void thread2(void const *arguement)
{
		while(1)
		{
			
				++count2;
		}
}	

void thread3(void const *arguement)
{
		while(1)
		{
			
				++count3;
		}
}	

//Step2:Define the thread
osThreadDef(thread1,osPriorityNormal,1,0);     //it takes 4 arguements(threadname,ospriority,thread instance,stacksize)
osThreadDef(thread2,osPriorityNormal,1,0);			//this defines a thread
osThreadDef(thread3,osPriorityNormal,1,0);
int main(void)
{
	//Step3:Create the thread
		osThreadCreate(osThread(thread1),NULL);
		osThreadCreate(osThread(thread2),NULL);
		osThreadCreate(osThread(thread3),NULL);
	while(1)
	{
		
	}	
	
}	
	