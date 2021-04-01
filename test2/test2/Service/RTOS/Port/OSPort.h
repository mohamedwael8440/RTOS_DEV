/*
 * OSPort.h
 *
 *  Created on: Dec 17, 2020
 *      Author: MahmoudH
 */

#ifndef PORT_OSPORT_H_
#define PORT_OSPORT_H_

#include "stm32f4xx.h"
#include "OSConfig.h"
#include "Linked_List.h"
#include "data_types.h"


#define OSTaskSwitch()									\
{														\
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;				\
}
#define disableInterrupts()   __set_BASEPRI(80)
#define enableInterrupts()    __set_BASEPRI(0)

void OSSysTickSetup(void);

void SVC_Handler(void) __attribute__ (( naked ));
void PendSV_Handler(void) __attribute__ (( naked ));
void SysTick_Handler(void);

#endif /* PORT_OSPORT_H_ */
