		AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
		EXTERN  currentpt   ;which task is working it specifies
		EXPORT	SysTick_Handler
		EXPORT  osSchedulerLaunch
					
SysTick_Handler			;saves r0,r1,r2,r3,r12,lr,pc,psr 
	CPSID 		I			;disable interrupt
	PUSH		{R4-R11}	;save r4,r5,r6,r7,r8,r9,r10,r11 
	LDR 		R0,=currentpt	;save sp in currentpt as it points top of stack r0 pts to currentpt
	LDR			R1,[R0]			;r1 =currentpt completes saving of context and now we need to load the context
	STR			SP,[R1]			;this helps while retriving back
	LDR			R1,[R1,#4]		;R1 = currentpt->next,second entry in TCB
	STR			R1,[R0]			;currentpt = R1
	LDR			SP,[R1]			;SP = currentpt->stackpt
	POP			{R4-R11}	
	CPSIE		I				;enable interrupt
	BX			LR				;r0-psr all this registers will be restored back  automatically here
	
	
	
osSchedulerLaunch
	LDR			R0,=currentpt;
	LDR			R2,[R0]			;R2=currentpt
	LDR			SP,[R2]			;SP=currentpt->stackpt
	POP			{R4-R11}		;pop out R4 to R11
	POP			{R0-R3}			;pop out R0-R3
	POP			{R12}				;POP R12
	ADD			SP,SP,#4		;we now nneed to pop LR
	POP			{LR}			;now pop out the LR
	ADD			SP,SP,#4		;now pop out PSR
	CPSIE 		I
	BX			LR
	
	ALIGN
	END