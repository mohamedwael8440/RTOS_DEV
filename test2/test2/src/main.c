
/* Includes */

#include "RTOS.h"
#include "board.h"
#include "uart.h"
#include "Semaphore.h"

#define T1_STACK_SIZE	100
#define T2_STACK_SIZE	100
#define T3_STACK_SIZE	100

uint32_t t1_stack[T1_STACK_SIZE];
uint32_t t2_stack[T2_STACK_SIZE];
uint32_t t3_stack[T3_STACK_SIZE];

void T_T1(void);
void T_T2(void);
void T_T3(void);
void system_init(void);

OSSem*	sem;
TCB* tcb = NULL;
int main(void)
{
	system_init();

	//sem = OSSemCreate(SEM_BINARY, 1);
	sem = OSSemCreate(SEM_MUTEX, 1);

	OSCreateTask(T_T1,1,t1_stack,T1_STACK_SIZE,&tcb);


	OSStart();
}

void system_init(void){
	RCC_DeInit();
	SystemCoreClockUpdate();
	/* 16 MHz */
	Leds_Init(0xFF);
	Btn_Init(BTN_RIGHT);
	Uart_Init(USART2,9600);
	Uart_SendString(USART2,"Started ...\n");
}
uint8_t buffer[100];
void T_T1(void){
	while(1){
		OSSemTake(sem,1000);
		Uart_SendString(USART2,"Low takes Sem\n");
		sprintf(buffer,"Low Pr = %d\n",tcb->taskPri);
		Uart_SendString(USART2,buffer);
		OSCreateTask(T_T3,3,t3_stack,T3_STACK_SIZE,NULL);
		sprintf(buffer,"Low Pr = %d\n",tcb->taskPri);
		Uart_SendString(USART2,buffer);
		OSCreateTask(T_T2,2,t2_stack,T2_STACK_SIZE,NULL);
		OSSemGive(sem);
	}
}
void T_T2(void){
	while(1){
		Uart_SendString(USART2,"Medium Works\n");
		OSDelay(10000);
	}
}
void T_T3(void){
	while(1){
		OSSemTake(sem,10000);
		Uart_SendString(USART2,"High takes Semaphore\n");
		sprintf(buffer,"Low Pr = %d\n",tcb->taskPri);
		Uart_SendString(USART2,buffer);
		while(1);
	}
}
