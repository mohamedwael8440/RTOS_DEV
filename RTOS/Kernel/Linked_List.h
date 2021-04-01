/*
 * Linked_List.h
 *
 *  Created on: Dec 20, 2020
 *      Author: M_WA
 */

#ifndef KERNEL_LINKED_LIST_H_
#define KERNEL_LINKED_LIST_H_

#include "stdint.h"
#include "stddef.h"

typedef struct tcb{
	uint32_t*  		pTaskStack;
	void (*pfTaskName)(void);
	uint32_t   		waitTicks;
	uint8_t    		taskPri;
	struct tcb* 	next;
}TCB;

typedef enum{
 LL_OT_PRI, LL_OT_WAIT_TICKS
}OrderType;

typedef enum{
 LL_OD_DESC, LL_OD_ASC
}OrderDirection;


void LL_Insert(TCB** pList, TCB* pTcb,
		OrderType order, OrderDirection direction);
TCB* LL_ExtractHead(TCB** pList);
TCB* LL_GetHead(TCB* list);


#endif /* KERNEL_LINKED_LIST_H_ */
