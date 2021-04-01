/*
 * RTOS.c
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */

#include "RTOS.h"

uint32_t idle_stack[IDLE_STACK_SIZE];

uint32_t 	last_free_tcb_ind = 0;
TCB 		TCBList[MAX_TASKS_NO];

TCB* readyList = NULL;
TCB* waitingList = NULL;

TCB* 	currentTCB 	= NULL;
TCB* 	highestRdyTCB	= NULL;
uint8_t OSStared 	= 0;



void user_error(void){
	while(1);
}
static TCB* OSTaskInit(void (*pfTask)(void),uint32_t task_pr,
		uint32_t* task_stack_arr,uint32_t task_stack_size){
	/* Allocate TCB*/
	TCB* pTCB = &TCBList[last_free_tcb_ind];
	last_free_tcb_ind++;
	/* TCB Init*/
	pTCB->next       	= NULL;
	pTCB->prev       	= NULL;
	pTCB->eNext       	= NULL;
	pTCB->ePrev       	= NULL;
	pTCB->pfTaskName 	= pfTask;
	pTCB->taskPri 		= task_pr;
	pTCB->taskBasePri 	= 0;
	pTCB->sem 			= NULL;
	pTCB->waitTicks		= 0;
	pTCB->pTaskStack 	= ((uint32_t)(task_stack_arr + task_stack_size))&0xFFFFFFFC;
	/* Stack Init */
	/* PSR,PC,LR,R12,R3:R0
	 * EXC_RETURN, R11:4
	 *  */
	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = 0x01000000;
	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = ((uint32_t)pfTask)&0xFFFFFFFE;
	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = user_error;
	pTCB->pTaskStack = pTCB->pTaskStack -5;
	pTCB->pTaskStack--;
	*(pTCB->pTaskStack) = 0xFFFFFFFD;
	pTCB->pTaskStack = pTCB->pTaskStack -8;
	return pTCB;
}
void OSCreateTask(void (*pfTask)(void),uint32_t task_pr,
		uint32_t* task_stack_arr,uint32_t task_stack_size,TCB** ppTCB){

	TCB* pTCB = OSTaskInit(pfTask,task_pr,task_stack_arr,task_stack_size);
	if(ppTCB != NULL){
		(*ppTCB)  = pTCB;
	}
	LL_Insert(&readyList, pTCB, LL_OT_PRI, LL_OD_DESC);

	if(OSStared == 1 && pTCB->taskPri > currentTCB->taskPri)
	{
		/* The preempted task should go to the readyList */
		LL_Insert(&readyList, currentTCB, LL_OT_PRI, LL_OD_DESC);
		highestRdyTCB = LL_ExtractHead(&readyList);
		OSTaskSwitch();
	}
}
void OSStart(void){
	/* create idle
	 * disable int
	 * set Pr PENDSV and SYSTICK
	 * Tick setup
	 * make sure current tcb -> highest ready
	 * request SVC
	 *  */
	OSCreateTask(T_Idle,0,idle_stack,IDLE_STACK_SIZE,NULL);
	disableInterrupts();
	SCB->SHP[10] = 0xF0; /* PendSV */
	SCB->SHP[11] = 0xF0; /* SysTick */
	OSSysTickSetup();
	currentTCB = LL_ExtractHead(&readyList);
	OSStared = 1;
	asm("svc 0");
}
void OSDelay(uint32_t waitTicks){
	disableInterrupts();
	currentTCB->waitTicks = waitTicks;
	LL_Insert(&waitingList,currentTCB,LL_OT_WAIT_TICKS,LL_OD_ASC);
	highestRdyTCB = LL_ExtractHead(&readyList);
	OSTaskSwitch(); /*  */
	enableInterrupts();
}
void T_Idle(void){
   while(1){

   }
}
