/*
 * Linked_List.h
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */

#ifndef KERNEL_LINKED_LIST_H_
#define KERNEL_LINKED_LIST_H_

#include "data_types.h"

/* ready and waiting lists (next and prev)*/
void LL_Insert(TCB** pList, TCB* pTcb,
		OrderType order, OrderDirection direction);
TCB* LL_ExtractHead(TCB** pList);
void LL_Delete(TCB** pList, TCB* pTcb);

/* events  (enext and eprev)*/
void LL_E_Insert(TCB** pList, TCB* pTcb);
TCB* LL_E_ExtractHead(TCB** pList);
void LL_E_Delete(TCB** pList, TCB* pTcb);

#endif /* KERNEL_LINKED_LIST_H_ */
