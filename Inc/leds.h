/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LEDS_H
#define __LEDS_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
//#include "main.h"
//#include <string.h>
#include <stdbool.h> 


/* Typedef ------------------------------------------------------------------*/
typedef enum {
	LedColorRed = 0,
	LedColorGreen,
	LedColorBlue,
	LedColorWhite,
	LedColorOrange
	
}LedColor_t;


typedef enum {
	Led1 = 0,
	Led2 = 24,
	Led3 = 48,
	Led4 = 72,
	Led5 = 96,
	Led6 = 120,
	Led7 = 144,
	Led8 = 168,
	Led9 = 192,
	Led10 = 216,
	Led11 = 240,
	Led12 = 264,
	Led13 = 288,
	Led14 = 312,
	Led15 = 336,
	Led16 = 360,
	Led17 = 384,
	Led18 = 408,
	Led19 = 432,
	Led20 = 456,
	Led21 = 480,
	Led22 = 504,
	Led23 = 528,
	Led24 = 552, 	
	Led25 = 576,
}LedNum_t;


/* Function Decleration ------------------------------------------------------------------*/
void ledAllOn(LedColor_t ledColor, uint8_t brightness); 
void ledAllOff(void); 					
void TurnOffAllLed(void);         

void ledSingleOn(uint8_t ledIndexReq, LedColor_t color,uint8_t brightness, bool enable); 
void ledSingleOff(uint8_t ledIndexReq);									

void ledRunAnimation1();
void ledRunAnimation2();
void ledRunAnimation3();
void ledRunAnimation4(LedColor_t color_rgb);
void ledRunAnimation5();
void ledRunAnimation6();
void ledDemo();
	
	
void spiSendData(uint8_t *data, uint16_t size); //it is working fine.
void spiSendReset();														//it is working fine.
void spiSendFrame(uint8_t *data, uint8_t size); //it is working fine.

void ledSingleonTest(LedNum_t lednum);


/* USER CODE END Private defines */
#endif /* __LEDS_H */