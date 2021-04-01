/*
 * Semaphore.c
 *
 *  Created on: Dec 21, 2020
 *      Author: MahmoudH
 */

#include "Semaphore.h"

extern TCB* 	readyList;
extern TCB* 	waitingList;

extern TCB* 	currentTCB;
extern TCB* 	highestRdyTCB;
extern uint8_t 	OSStared;

OSSem 		semList[MAX_SEMAPHORE_NO];
uint32_t 	last_free_sem_ind = 0;

OSSem* 	OSSemCreate(SemType semType, uint32_t initValue){
	OSSem* sem = &semList[last_free_sem_ind];
	last_free_sem_ind++;
	sem->semType 			= semType;
	sem->semValue 			= initValue;
	sem->semWaitingList 	= NULL;
	return sem;
}
void	OSSemGive(OSSem* sem){
	TCB* pTCB = NULL;
	disableInterrupts();

	if(sem->semType == SEM_BINARY|| (sem->semType == SEM_MUTEX && currentTCB == sem->semOwner)){
		/* back to the base priority */
		if(sem->semType == SEM_MUTEX){
			if(currentTCB->taskBasePri > 0){
				currentTCB->taskPri 	= currentTCB->taskBasePri;
				currentTCB->taskBasePri = 0;
			}
			sem->semOwner = NULL;
		}
		sem->semValue++;
		pTCB = LL_E_ExtractHead(&sem->semWaitingList);
		while(pTCB != NULL){
			LL_Insert(&readyList, pTCB, LL_OT_PRI, LL_OD_DESC);
			LL_Delete(&waitingList,pTCB);
			pTCB->sem = NULL;
			pTCB = LL_E_ExtractHead(&sem->semWaitingList);
		}
		if(readyList != NULL && readyList->taskPri > currentTCB->taskPri){
			highestRdyTCB = LL_ExtractHead(&readyList);
			LL_Insert(&readyList, currentTCB, LL_OT_PRI, LL_OD_DESC);
			OSTaskSwitch();
		}
	}
	enableInterrupts();
}
Bool 	OSSemTake(OSSem* sem, uint32_t timeout){
	Bool result = FALSE;
	if(sem != NULL){
		disableInterrupts();
		if(sem->semValue == 0 && timeout > 0){
			if(sem->semType == SEM_MUTEX &&
					currentTCB->taskPri > sem->semOwner->taskPri){
				if(sem->semOwner->taskBasePri == 0){
					sem->semOwner->taskBasePri = sem->semOwner->taskPri;
				}
				sem->semOwner->taskPri = currentTCB->taskPri;
				if(sem->semOwner->waitTicks == 0){
					LL_Delete(&readyList,sem->semOwner);
					LL_Insert(&readyList, sem->semOwner, LL_OT_PRI, LL_OD_DESC);
				}
			}
			/* wait */
			currentTCB->waitTicks = timeout;
			currentTCB->sem = sem;
			LL_Insert(&waitingList,currentTCB,LL_OT_WAIT_TICKS,LL_OD_ASC);
			LL_E_Insert(&sem->semWaitingList, currentTCB);
			highestRdyTCB = LL_ExtractHead(&readyList);
			OSTaskSwitch();
		}
		if(sem->semValue > 0){
			if(sem->semType == SEM_MUTEX){
				sem->semOwner = currentTCB;
			}
			sem->semValue--;
			result = TRUE;
		}
		enableInterrupts();
	}
	return result;
}
