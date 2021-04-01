/*
 * OSConfig.h
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */

#ifndef CONFIG_OSCONFIG_H_
#define CONFIG_OSCONFIG_H_

#include "stm32f4xx.h"

extern uint32_t SystemCoreClock;

#define MAX_TASKS_NO			10
#define MAX_SEMAPHORE_NO		5
#define IDLE_STACK_SIZE			100
#define TICK_RATE				1000
#define CPU_CLOCK				(SystemCoreClock)

#endif /* CONFIG_OSCONFIG_H_ */
