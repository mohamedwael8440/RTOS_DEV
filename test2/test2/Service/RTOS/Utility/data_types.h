/*
 * data_types.h
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */

#ifndef UTILITY_DATA_TYPES_H_
#define UTILITY_DATA_TYPES_H_

#include "stdint.h"
#include "stddef.h"

typedef enum{
	FALSE, TRUE
}Bool;

typedef  enum{
	SEM_BINARY, SEM_COUNTING, SEM_MUTEX
}SemType;

typedef struct ossem{
	SemType		semType;
	uint32_t	semValue;
	struct tcb* semWaitingList; /* Waiting List of this semaphore */
	struct tcb* semOwner;
}OSSem;

typedef struct tcb{
	uint32_t*  		pTaskStack;
	struct tcb* 	next;
	struct tcb* 	prev;
	struct tcb* 	eNext; /* Events */
	struct tcb* 	ePrev; /* Events */
	OSSem* 			sem;
	void (*pfTaskName)(void);
	uint32_t   		waitTicks;
	uint32_t   		taskPri;	/* inherited */
	uint32_t   		taskBasePri; /* old */
}TCB;

typedef enum{
 LL_OT_PRI, LL_OT_WAIT_TICKS
}OrderType;

typedef enum{
 LL_OD_DESC, LL_OD_ASC
}OrderDirection;

#endif /* UTILITY_DATA_TYPES_H_ */
