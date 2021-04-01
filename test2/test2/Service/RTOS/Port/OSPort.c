/*
 * OSPort.c
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */

#include "OSPort.h"

extern TCB* 	readyList;
extern TCB* 	waitingList;

extern TCB* 	currentTCB;
extern TCB* 	highestRdyTCB;
extern uint8_t 	OSStared;

void OSSysTickSetup(void){
	SysTick->LOAD = (CPU_CLOCK/TICK_RATE) -1;
	SysTick->CTRL = (	SysTick_CTRL_CLKSOURCE_Msk|
			SysTick_CTRL_ENABLE_Msk|
			SysTick_CTRL_TICKINT_Msk
	);
}
void SVC_Handler(void){
	asm(".align 4  \n");
	/* task_sp (R0) for current */
	asm("	ldr r0,=currentTCB				\n"); /*&&TCB*/
	asm("	ldr r0,[r0]						\n"); /*&TCB*/
	asm("	ldr r0,[r0]						\n"); /*r0 = task_sp*/
	asm("											\n");
	asm("	ldmia r0!, {r4-r11, r14}				\n");
	asm("	msr psp, r0				\n");
	asm("					\n");
	asm("	mov r0,#0				\n");
	asm("	msr basepri, r0				\n");
	asm("	bx r14				\n");
}
void PendSV_Handler(void){
	asm(".align 4  \n");
	/* task_sp (R0) for current */
	asm("	mrs r0, psp										\n");
	asm("											\n");
	asm("	ldr r3,=currentTCB				\n"); /*&&TCB*/
	asm("	ldr r2,[r3]						\n"); /*&TCB*/
	asm("											\n");
	asm("	tst	r14, #0x10									\n");
	asm("	it  eq										\n");
	asm("	vstmdbeq r0!, {s16-s31}										\n");
	asm("											\n");
	asm("	stmdb r0!, {r4-r11, r14}										\n");
	asm("											\n");
	asm("	str r0,[r2]										\n");
	asm("											\n");
	asm("	ldr r0,=highestRdyTCB				\n"); /*&&TCB*/
	asm("	ldr r0,[r0]				\n");
	asm("	str r0,[r3]						\n");
	asm("											\n");
	asm("	ldr r0,[r0]				\n");
	asm("											\n");
	asm("	ldmia r0!, {r4-r11, r14}				\n");
	asm("											\n");
	asm("	tst	r14, #0x10									\n");
	asm("	it  eq										\n");
	asm("	vldmiaeq r0!, {s16-s31}										\n");
	asm("											\n");
	asm("	msr psp, r0				\n");
	asm("					\n");
	asm("	bx r14				\n");
}
void SysTick_Handler(void){
	/*
	 * for all waiting list -> decrement ticks,
	 *  if 0 -> ready, if higher -> CS
	 * */
	TCB* pTCB = waitingList;
	disableInterrupts();
	while(pTCB != NULL){
		pTCB->waitTicks--;
		if(pTCB->waitTicks == 0){
			pTCB = LL_ExtractHead(&waitingList);
			LL_Insert(&readyList, pTCB, LL_OT_PRI, LL_OD_DESC);
			if(pTCB->sem != NULL){
				LL_E_Delete(&pTCB->sem->semWaitingList,pTCB);
				pTCB->sem = NULL;
			}
			/* back to the waiting list */
			pTCB = waitingList;
		}else{
			pTCB = pTCB->next;
		}
	}
	if(readyList != NULL && readyList->taskPri > currentTCB->taskPri){
		highestRdyTCB = LL_ExtractHead(&readyList);
		LL_Insert(&readyList, currentTCB, LL_OT_PRI, LL_OD_DESC);
		OSTaskSwitch();
	}
	enableInterrupts();
}
