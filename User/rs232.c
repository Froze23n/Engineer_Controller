#include "rs232.h"
#include "usart.h"
#include "tim.h" //分配tim7 1000Hz

void RS232_Init(void){
    HAL_TIM_Base_Start_IT(&htim7);
}

void RS232_Task(void){
    
}