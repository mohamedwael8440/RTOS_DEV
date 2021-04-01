/*
 * Semaphore.h
 *
 *  Created on: Dec 21, 2020
 *      Author: MahmoudH
 */

#ifndef RTOS_KERNEL_SEMAPHORE_H_
#define RTOS_KERNEL_SEMAPHORE_H_

#include "stm32f4xx.h"
#include "OSConfig.h"
#include "Linked_List.h"
#include "data_types.h"
#include "OSPort.h"


OSSem* 	OSSemCreate(SemType semType, uint32_t initValue);
void	OSSemGive(OSSem* sem);
Bool 	OSSemTake(OSSem* sem, uint32_t timeout);

#endif /* RTOS_KERNEL_SEMAPHORE_H_ */
