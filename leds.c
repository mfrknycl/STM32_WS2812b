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


void LedRed(){
	
	__HAL_SPI_ENABLE(&hspi1);
	spiSendReset();
	int leds = 24;
	
	while(leds--){
		uint16_t Red8bits   = 8;  //0
		uint16_t Green8bits = 8;  //1
		uint16_t Blue8bits  = 8;  //0
		while(Red8bits--){
			spiSendData(logicLow, 2);
			
		}
		while(Green8bits--){
			spiSendData(logicHigh, 2);
		}
		while(Blue8bits--){
			spiSendData(logicLow, 2);
		}
	}
	ledAllOff();
	spiSendData(fakeHigh, 1);
	HAL_Delay(1);
}

void LedGreen(){
		__HAL_SPI_ENABLE(&hspi1);
		spiSendReset();
		int leds = 24;
		while(leds--){
			uint16_t Red8bits   = 8;  //1
			uint16_t Green8bits = 8;  //0
			uint16_t Blue8bits  = 8;  //0
			while(Red8bits--){
				spiSendData(logicHigh, 2);
			}
			while(Green8bits--){
				spiSendData(logicLow, 2);
			}
			while(Blue8bits--){
				spiSendData(logicLow, 2);
			}
		}
		ledAllOff();
		spiSendData(fakeHigh, 1);
		HAL_Delay(1);
}



void LedBlue(){
	__HAL_SPI_ENABLE(&hspi1);
	spiSendReset();
	int leds = 24;
	
	while(leds--){
		uint16_t Red8bits   = 8;  //0
		uint16_t Green8bits = 8;  //0
		uint16_t Blue8bits  = 8;  //1
		while(Red8bits--){
			spiSendData(logicLow, 2);
		}
		while(Green8bits--){
			spiSendData(logicLow, 2);
		}
		while(Blue8bits--){
			spiSendData(logicHigh, 2);
		}
	}
	ledAllOff();
	spiSendData(fakeHigh, 1);
	HAL_Delay(1);
}

void LedWhite(){
		__HAL_SPI_ENABLE(&hspi1);
		spiSendReset();
		int leds = 24;
		while(leds--){
			uint16_t sizeOne = 24 ;
			while(sizeOne--){
				spiSendData(logicHigh, 2);
			}
		}
		ledAllOff();
		spiSendData(fakeHigh, 1);
		HAL_Delay(1);
}

void LedOrange(){
	__HAL_SPI_ENABLE(&hspi1);
	spiSendReset();
	int leds = 24;
	
	while(leds--){
		uint16_t Red8bits   = 8;  //1
		uint16_t Green8bits = 8;  //0
		uint16_t Blue8bits  = 8;  //1
		while(Red8bits--){
			spiSendData(logicHigh, 2);
		}
		while(Green8bits--){
			spiSendData(logicLow, 2);
		}
		while(Blue8bits--){
			spiSendData(logicHigh, 2);
		}
	}
	ledAllOff();
	spiSendData(fakeHigh, 1);
	HAL_Delay(1);
}


void ledAllOn(LedColor_t ledColor){

	
	switch (ledColor)
  {
  	case 0:
			LedRed();
  		break;
		
  	case 1:
			LedGreen();
  		break;
			
		case 2:
			LedBlue();
  		break;
		
  	case 3:
			LedWhite();
  		break;
		
		case 4:
			LedOrange();
  		break;
		
  	default:
  		break;
  }
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
};



void ledSingleOn2(uint8_t ledIndexReq, LedColor_t color){

	uint8_t offset = 3;
	ledIndexReq  = ledIndexReq* offset;
	
//Why do we have this?
	if(ledClearReq == 1){
		ledAllOff();
		ledClearReq = 0;
	}

	if(manualControl == 0 ){
	switch(color){
		
		case LedColorBlue:
			ledframe[ledIndexReq+1] = 0x00;
			ledframe[ledIndexReq+2] = 0xFF;
			ledframe[ledIndexReq] 	= 0x00;
			break;
		
		case LedColorGreen:
			ledframe[ledIndexReq+1] = 0x00;
			ledframe[ledIndexReq+2] = 0x00;
			ledframe[ledIndexReq]   = 0xFF;
			break;
		
		case LedColorRed:
			ledframe[ledIndexReq+1] = 0xFF;
			ledframe[ledIndexReq+2] = 0x00;
			ledframe[ledIndexReq]   = 0x00;
			break;
		
		case LedColorOrange:
			break;
		
		case LedColorWhite:
			ledframe[ledIndexReq+1] = 0xFF;
			ledframe[ledIndexReq+2] = 0xFF;
			ledframe[ledIndexReq]   = 0xFF;
			break;
	}
}
	
	spiSendFrame(ledframe, 3*24);
	spiSendData(fakeHigh, 1);
	HAL_Delay(100);

	
}
void ledSingleOn(LedNum_t ledIndex, LedColor_t ledColor){	
			__HAL_SPI_ENABLE(&hspi1);
			spiSendReset();
			
			uint16_t size1 = ledIndex;
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
}

void ledSingleOff(uint16_t ledIndex, LedColor_t ledColor){
	
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


//Test2
void ledSingleonTest2(uint8_t led){
		__HAL_SPI_ENABLE(&hspi1);
		spiSendReset();
		int leds = led;
		while(leds--){
			uint16_t size1 = 24 ;
			uint16_t size2 = 24 ;
			while(size1--){
				spiSendData(logicLow, 2);
			}
			while(size2--){
				spiSendData(logicHigh, 2);
			}
		}
		ledAllOff();
		spiSendData(fakeHigh, 1);
}

void ledRunAnimation1(){
	//1sec All green On
	//1sec All Red on
	//1sec All Blue on
	//1sec All White on
	/*
	ledAllOn(LedColorRed);
	HAL_Delay(500);
	ledAllOn(LedColorGreen);
	HAL_Delay(500);
	ledAllOn(LedColorBlue);
	HAL_Delay(500);
	ledAllOn(LedColorWhite);
	HAL_Delay(500);
	*/
	//ledAllOff();
	ledSingleOn2(4, 1);
	//HAL_Delay(100);
}

void ledRunAnimation2(void){
	
}

void ledRunAnimation3(void){
	
}

void ledRunAnimation4(void){
	
}

void ledDemo(){	//Run each animation for 5 seconds

}

