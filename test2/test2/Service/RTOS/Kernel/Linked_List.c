/*
 * Linked_List.c
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */
#include "Linked_List.h"

void LL_Insert(TCB** pList, TCB* pTcb,
		OrderType order, OrderDirection direction){
	TCB* currPtr = (*pList);
	/* Check if empty */
	if((*pList) == NULL){
		(*pList) = pTcb;
		pTcb->next = NULL;
		pTcb->prev = NULL;
	}
	else{
		if(order == LL_OT_PRI && direction == LL_OD_DESC){
			/* check if the inserted tcb pri is higher than the first tcb in the list */
			if(pTcb->taskPri > currPtr->taskPri){
				pTcb->next = currPtr;
				(*pList) = pTcb;
				pTcb->prev = NULL;
				currPtr->prev = pTcb;
			}
			else{
				/* Traverse for the node that has a next with pri < inserted tcb pri
				 *  or the list reaches to the end */
				while( currPtr->next != NULL &&
						currPtr->next->taskPri > pTcb->taskPri){
					currPtr = currPtr->next;
				}
				/* make sure that now we can insert */
				pTcb->next = currPtr->next;
				currPtr->next = pTcb;
				pTcb->prev = currPtr;
				pTcb->next->prev = pTcb;
			}
		}
		else if(order == LL_OT_WAIT_TICKS && direction == LL_OD_ASC){
			/* check if the inserted tcb ticks is lower than the first tcb in the list */
			if(pTcb->waitTicks < currPtr->waitTicks){
				pTcb->next = currPtr;
				(*pList) = pTcb;
				pTcb->prev = NULL;
				currPtr->prev = pTcb;
			}
			else{
				/* Traverse for the node that has a next with ticks > inserted tcb ticks
				 *  or the list reaches to the end */
				while( currPtr->next != NULL &&
						currPtr->next->waitTicks < pTcb->waitTicks){
					currPtr = currPtr->next;
				}
				/* make sure that now we can insert */
				pTcb->next = currPtr->next;
				currPtr->next = pTcb;
				pTcb->prev = currPtr;
				pTcb->next->prev = pTcb;
			}
		}
	}
}
TCB* LL_ExtractHead(TCB** pList){
	TCB* head = (*pList);
	(*pList) = (*pList)->next;
	head->next = NULL;
	head->prev = NULL;
	return head;
}
void LL_Delete(TCB** pList, TCB* pTcb){
	if((*pList) != NULL && pTcb != NULL){
		if(pTcb->prev == NULL && pTcb->next == NULL){
			(*pList) = NULL;
		}
		else if(pTcb->prev != NULL && pTcb->next != NULL){
			pTcb->prev->next = pTcb->next;
			pTcb->next->prev = pTcb->prev;
			pTcb->next = NULL;
			pTcb->prev = NULL;
		} else if(pTcb->next == NULL){
			pTcb->prev->next = NULL;
			pTcb->prev = NULL;
		}
		else if(pTcb->prev == NULL){
			(*pList) = pTcb->next;
			pTcb->next->prev = NULL;
			pTcb->next = NULL;
		}

	}
}

/* events  (enext and eprev)*/
void LL_E_Insert(TCB** pList, TCB* pTcb){
	pTcb->eNext = NULL;
	if((*pList) == NULL){
		(*pList) = pTcb;
		pTcb->ePrev = NULL;
	}
	else{
		pTcb->eNext = (*pList);
		(*pList)->prev = pTcb;
		(*pList) = pTcb;
		pTcb->ePrev = NULL;
	}
}
TCB* LL_E_ExtractHead(TCB** pList){
	TCB* head = (*pList);
	if(head != NULL){
		(*pList) = (*pList)->eNext;
		head->eNext = NULL;
		head->ePrev = NULL;
	}
	return head;
}
void LL_E_Delete(TCB** pList, TCB* pTcb){
	if((*pList) != NULL && pTcb != NULL){
		if(pTcb->ePrev == NULL && pTcb->eNext == NULL){
			(*pList) = NULL;
		}
		else if(pTcb->ePrev != NULL && pTcb->eNext != NULL){
			pTcb->ePrev->eNext = pTcb->eNext;
			pTcb->eNext->ePrev = pTcb->ePrev;
			pTcb->eNext = NULL;
			pTcb->ePrev = NULL;
		} else if(pTcb->eNext == NULL){
			pTcb->ePrev->eNext = NULL;
			pTcb->ePrev = NULL;
		}
		else if(pTcb->ePrev == NULL){
			(*pList) = pTcb->eNext;
			pTcb->eNext->ePrev = NULL;
			pTcb->eNext = NULL;
		}

	}
}
