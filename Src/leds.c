//#include "main.h"
#include "leds.h"

extern SPI_HandleTypeDef hspi1;

uint8_t logicHigh[4] = {255,0xFE,0};
uint8_t logicLow[4]  = {255,0,0};
uint8_t fakeHigh[1]  = {0xFF};
uint8_t reset[1]     = {0x00};	

int num = 0;

const uint8_t ledsTotal = 24;
uint8_t ledframe[ledsTotal*3];
//uint8_t ledIndexReq = 0;
uint8_t ledClearReq = 1;
uint8_t manualControl = 0;

//LedColor_t ledColorReq = LedColorGreen;

void spiSendData(uint8_t *data, uint16_t size){
	__HAL_SPI_ENABLE(&hspi1);
	while(size--){
		if (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE)){
			*((__IO uint8_t *)&hspi1.Instance->DR) = (*data++);
		}
	}
}

void spiSendBit(uint8_t *data, uint16_t size){
	while(size--){
		if (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE)){
			*((__IO uint8_t *)&hspi1.Instance->DR) = (*data++);
		}
	}
}

void spiSendReset(){
	int size = 250;
	__HAL_SPI_ENABLE(&hspi1);
	while(size--){
		if (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE)){
			*((__IO uint8_t *)&hspi1.Instance->DR) = 0;
		}
	}
}


uint16_t bitsSent = 0;
uint16_t bitsSentLow = 0;
uint16_t bitsSentHigh = 0;
void spiSendFrame(uint8_t *data, uint8_t size){
	__HAL_SPI_ENABLE(&hspi1);
	spiSendReset();
	bitsSent = 0;
	bitsSentLow = 0;
	bitsSentHigh = 0;
	while(size--){
		uint8_t singleByte = *data++;
		uint8_t bitIndex = 8;
		while(bitIndex--){
			if(singleByte & (1<<bitIndex)){
				spiSendBit(logicHigh, 2);
				bitsSentHigh++;
			}else{
				spiSendBit(logicLow, 2);
				bitsSentLow++;
			}
			bitsSent++;
		}
	}
}



void ledAllOn(LedColor_t color, uint8_t brightness){

	//uint8_t offset_red = 3;
	uint8_t ledIndexStart  = 0;


	if(manualControl == 0 ){
	switch(color){
		
		case LedColorBlue:
			for(int i=0; i<=69; i+=3){
				ledframe[ledIndexStart+i+2] 	= brightness; //Blue
			}
			break;
		
		case LedColorGreen:
			for(int i=0; i<=69; i+=3){
				ledframe[ledIndexStart+i] 	= brightness; //Green
			}
			break;
		
		case LedColorRed:
			for(int i=0; i<=69; i+=3){
				ledframe[ledIndexStart+i+1] 	= brightness; //Red
			}
			break;
		
		case LedColorOrange:
			for(int i=0; i<=69; i+=3){
				ledframe[ledIndexStart+i] 	 = 0x64;    //Green
				ledframe[ledIndexStart+i+1]  = 0xD2;    //Red
				
			}
			break;
		
		case LedColorWhite:
			for(int i=0; i<=69; i+=3){
				ledframe[ledIndexStart+i] 	 = brightness; //Green
				ledframe[ledIndexStart+i+1]  = brightness; //Red
				ledframe[ledIndexStart+i+2]  = brightness; //Blue

	
			}
			break;
	}
}
	
	spiSendFrame(ledframe, 3*24);
	//spiSendData(fakeHigh, 1);
	HAL_Delay(1);
};

void ledAllOff(void){
	__HAL_SPI_ENABLE(&hspi1);
	spiSendReset();
	int leds = 24;
	while(leds--){
		uint16_t size = 24;
		while(size--){
			spiSendData(logicLow, 2);
		}
	}
		spiSendData(fakeHigh, 1);
}

void TurnOffAllLed(){
	//uint8_t offset_red = 3;
	uint8_t ledIndexStart  = 0;


	if(manualControl == 0 ){
			for(int i=0; i<=69; i+=3){
				ledframe[ledIndexStart+i] 	 = 0x00; //Green
				ledframe[ledIndexStart+i+1]  = 0x00; //Red
				ledframe[ledIndexStart+i+2]  = 0x00; //Blue

				}
	}
	
	spiSendFrame(ledframe, 3*24);
	//spiSendData(fakeHigh, 1); //when it's active, it starts form LED2
	HAL_Delay(1);
}

void ledSingleOn(uint8_t ledIndexReq, LedColor_t color,uint8_t brightness, bool enable){

	uint8_t offset = 3;
	ledIndexReq  = ledIndexReq * offset;
	
//Why do we have this?
	if(ledClearReq == 1){
		ledAllOff();
		ledClearReq = 0;
	}

	if(manualControl == 0 ){
	switch(color){
		
		case LedColorBlue:
			ledframe[ledIndexReq] 	= 0x00; //Green
			ledframe[ledIndexReq+1] = 0x00; //Red
			ledframe[ledIndexReq+2] = brightness; //Blue
			break;
		
		case LedColorGreen:
			ledframe[ledIndexReq] 	= brightness; //Green
			ledframe[ledIndexReq+1] = 0x00; //Red
			ledframe[ledIndexReq+2] = 0x00; //Blue
			break;
		
		case LedColorRed:
			ledframe[ledIndexReq] 	= 0x00; //Green
			ledframe[ledIndexReq+1] = brightness; //Red
			ledframe[ledIndexReq+2] = 0x00; //Blue
			break;
		
		case LedColorOrange:
			ledframe[ledIndexReq] 	= 0x64; //Green
			ledframe[ledIndexReq+1] = 0xD2; //Red
			ledframe[ledIndexReq+2] = 0x00; //Blue
			break;
		
		case LedColorWhite:
			ledframe[ledIndexReq] 	= brightness; //Green
			ledframe[ledIndexReq+1] = brightness; //Red
			ledframe[ledIndexReq+2] = brightness; //Blue
			break;
	}
}
	
	spiSendFrame(ledframe, 3*24);
	if(enable == 1){
		TurnOffAllLed();
	}
	//spiSendData(fakeHigh, 1);
	HAL_Delay(1);

	
}


void ledSingleOff(uint8_t ledIndexReq){
		uint8_t offset = 3;
	ledIndexReq  = ledIndexReq * offset;
	
//Why do we have this?
	if(ledClearReq == 1){
		ledAllOff();
		ledClearReq = 0;
	}

	if(manualControl == 0 ){	
			ledframe[ledIndexReq+1] = 0x00;
			ledframe[ledIndexReq+2] = 0x00;
			ledframe[ledIndexReq] 	= 0x00;
}
	spiSendFrame(ledframe, 3*24);
	spiSendData(fakeHigh, 1);
	HAL_Delay(100);
}


//Test
void ledSingleonTest(LedNum_t lednum){
		
	__HAL_SPI_ENABLE(&hspi1);
		spiSendReset();
			
			
			uint16_t size1 = lednum;
			uint16_t size2 = 24;
			uint16_t result = 576-(size1 + size2);
	
			while(size1--){
				spiSendData(logicLow, 2);
			}
			while(size2--){
				spiSendData(logicHigh, 2);
			}
			while(result--){
				spiSendData(logicLow, 2);
			}
		
		ledAllOff();
		spiSendData(fakeHigh, 1);
		HAL_Delay(1);
}


void ledRunAnimation1(){
	//1sec All green On
	//1sec All Red on
	//1sec All Blue on
	//1sec All White on
	
	ledAllOn(LedColorGreen, 255);
	HAL_Delay(1000);
	TurnOffAllLed();
	HAL_Delay(1000);
	
	ledAllOn(LedColorRed, 255);
	HAL_Delay(1000);
	TurnOffAllLed();
	HAL_Delay(1000);
	
	ledAllOn(LedColorBlue, 255);
	HAL_Delay(1000);
	TurnOffAllLed();
	HAL_Delay(1000);
	
	ledAllOn(LedColorWhite, 255);
	HAL_Delay(1000);
	TurnOffAllLed();
	HAL_Delay(1000);	
	
}

void ledRunAnimation2(){
	ledAllOn(LedColorGreen, 255);
	HAL_Delay(500);
	TurnOffAllLed();
	
	ledAllOn(LedColorWhite, 255);
	HAL_Delay(200);
	TurnOffAllLed();
	
	ledAllOn(LedColorGreen, 255);
	HAL_Delay(800);
	TurnOffAllLed();
	
	for(int i=24; i>=0; i--){
		ledSingleOn(i, LedColorWhite,255, 1);
	}
}

void ledRunAnimation3(){
	for(int i=24; i>=0; i--){
		ledSingleOn(i, LedColorGreen,255, 0);
		HAL_Delay(150);
	}
	HAL_Delay(200);
	TurnOffAllLed();
}

void ledRunAnimation4(LedColor_t color_rgb){
uint8_t color = color_rgb;
uint8_t enable = 0;	
	
		for(int i=0; i<24; i++){
				
				ledSingleOn(i,    color, 10,  enable);
				ledSingleOn(i+1,  color, 30,  enable);
				ledSingleOn(i+2,  color, 50,  enable);
				ledSingleOn(i+3,  color, 70,  enable);
				ledSingleOn(i+4,  color, 90,  enable);
				ledSingleOn(i+5,  color, 110, enable);
				ledSingleOn(i+6,  color, 130, enable);
				ledSingleOn(i+7,  color, 150, enable);
				ledSingleOn(i+8,  color, 170, enable);
				ledSingleOn(i+9,  color, 190, enable);
				ledSingleOn(i+10, color, 210, enable);

			HAL_Delay(1);
		}
		

		
	//	TurnOffAllLed();
	
	
	
	
}


void ledDemo(){	//Run each animation for 5 seconds

	ledRunAnimation4(LedColorRed);
	HAL_Delay(5000);
	ledAllOn(LedColorRed, 255);
	HAL_Delay(5000);
	TurnOffAllLed();
	
	ledRunAnimation4(LedColorGreen);
	HAL_Delay(5000);
	ledAllOn(LedColorGreen, 255);
	HAL_Delay(5000);
	TurnOffAllLed();
	
	
		ledRunAnimation4(LedColorBlue);
	HAL_Delay(5000);
	ledAllOn(LedColorBlue, 255);
	HAL_Delay(5000);
	TurnOffAllLed();
	
	
		ledRunAnimation4(LedColorOrange);

	HAL_Delay(5000);
	TurnOffAllLed();
	
	
		ledRunAnimation4(LedColorWhite);
	HAL_Delay(5000);
	ledAllOn(LedColorWhite, 255);
	HAL_Delay(5000);
	TurnOffAllLed();
	

	
	
}

