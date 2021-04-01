/*
 * RTOS.h
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */

#ifndef KERNEL_RTOS_H_
#define KERNEL_RTOS_H_

#include "stm32f4xx.h"
#include "Linked_List.h"
#include "data_types.h"
#include "OSConfig.h"
#include "OSPort.h"


void OSCreateTask(void (*pfTask)(void),uint32_t task_pr,
		uint32_t* task_stack_arr,uint32_t task_stack_size,TCB** ppTCB);
void OSStart(void);
void OSDelay(uint32_t waitTicks);
void T_Idle(void);

#endif /* KERNEL_RTOS_H_ */
