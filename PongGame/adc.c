#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "adc.h"

uint16_t xMin = 182;
uint16_t xMax = 886;
uint16_t yMin = 337;
float xConversionFactor = 0.1803977f;

uint16_t yMax  = 817;
float yConversionFactor  = 0.13125f;

uint16_t volatile touchX = 0;
uint16_t volatile touchY = 0;
uint16_t volatile aMeter = 0;
uint8_t volatile ADCchannel = 0;

extern uint16_t  adcResult = 0;
volatile uint16_t paddle1Y = 36;
volatile uint16_t paddle2Y = 32;

void adcInit(){
	ADMUX = 0;
	ADMUX = (1<<REFS0); // should use external capacitor on ARef
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); // enable ADC and set rate to 1/128
	ADCSRB |= 0x00; // free running
	ADCSRA |= (1 << ADIE);
	sei();
	//DDRC = 0x0;  // initialize to read
	
}

uint8_t adc_read(){	//1110
	
	//ADCchannel &= 0x07; // zero out high bits
	ADMUX = (ADMUX & 0xF8) | ADCchannel; //00000011 & 00001111 => 00000011
	
	if(ADCchannel == 0){
		DDRC = 0x0A;		//0b00001010 Setting X- & X+ as Outputs
		PORTC= 0x0D;		//0b00001101 Setting X- High and X+ Low. & Setting Y- & Y+ in internal Pull up mode.
	}else if(ADCchannel == 3){
		DDRC = 0x05;
		PORTC = 0x0B;
	}else if(ADCchannel == 4){
		DDRC = 0x00;
		PORTC = 0x04;
	}
	ADCSRA |= (1<<ADSC);
	//ADCSRA |= (1<< ADATE);
	//ADCSRB = 0x00;
	return 0;
}


uint8_t mapTocuhValues(char axis){
	float result = 0;
	if(axis == 'x'){
		if(touchX < xMin){
			touchX = xMin;
		}
		if(touchX > xMax){
			touchX = xMax;
		}
		result = xConversionFactor * (float) (touchX - xMin);	
	}else{
		if(touchY < yMin){
			touchY = yMin;
		}
		if(touchY > yMax){
			touchY = yMax;
		}
		result = yConversionFactor * (float) (yMax - touchY);
	}
	uint8_t res = (uint8_t) (result + 0.5f);
	return res;
}

ISR(ADC_vect){
	adcResult = ADC;
	//printf("ACH : %u\t%u\n", ADCchannel, adcResult);
	if(ADCchannel == 0){
		touchX = adcResult;
		ADCchannel = 3;
	}
	else if(ADCchannel == 3){
		touchY = adcResult;
		ADCchannel = 4;
	}else if(ADCchannel == 4){
		aMeter = adcResult;
		ADCchannel = 0;
	}
	
}