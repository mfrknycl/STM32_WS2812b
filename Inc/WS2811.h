/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WS2811_H
#define __WS2811_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdbool.h> 

/* Function Decleration ------------------------------------------------------------------*/
void draw_red(void);
void Delay_us (int us);

void delayUS(uint32_t us);





/* USER CODE END Private defines */
#endif /* __WS2811_H */