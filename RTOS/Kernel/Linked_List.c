/*
 * Linked_List.c
 *
 *  Created on: Dec 20, 2020
 *      Author: M_WA
 */
#include "Linked_List.h"

void LL_Insert(TCB** pList, TCB* pTcb,
		OrderType order, OrderDirection direction){
	TCB* currPtr = (*pList);
	/* Check if empty */
	if((*pList) == NULL){
		(*pList) = pTcb;
		pTcb->next = NULL;
	}
	else{
		if(order == LL_OT_PRI && direction == LL_OD_DESC){
			/* check if the inserted tcb pri is higher than the first tcb in the list */
			if(pTcb->taskPri > currPtr->taskPri){
				pTcb->next = currPtr;
				(*pList) = pTcb;
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
			}
		}
		else if(order == LL_OT_WAIT_TICKS && direction == LL_OD_ASC){
			/* check if the inserted tcb ticks is lower than the first tcb in the list */
			if(pTcb->waitTicks < currPtr->waitTicks){
				pTcb->next = currPtr;
				(*pList) = pTcb;
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
			}
		}
	}
}
TCB* LL_ExtractHead(TCB** pList){
	TCB* head = (*pList);
	(*pList) = (*pList)->next;
	return head;
}
TCB* LL_GetHead(TCB* list){
	return list;
}
