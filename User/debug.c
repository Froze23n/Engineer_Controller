#include "debug.h"
#include "usart.h"
#include "gpio.h"
#include "i2c.h"

void Debug_Init(void){
    HAL_GPIO_WritePin(Red_GPIO_Port, Red_Pin, GPIO_PIN_SET);
    // HAL_GPIO_WritePin(Blue_GPIO_Port, Blue_Pin, GPIO_PIN_SET);
}

void Debug_Task(void){
    HAL_GPIO_TogglePin(Red_GPIO_Port, Red_Pin);
    HAL_GPIO_TogglePin(Blue_GPIO_Port, Blue_Pin);
}