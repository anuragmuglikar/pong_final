#define F_CPU 16000000
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "gameControls.h"
#include "adc.h"

#define BLACK 0x000001
#define WHITE 0x000000

uint8_t volatile p1Score  = 0;
uint8_t volatile p2Score  = 0;

uint8_t volatile delX = 1;
uint8_t volatile delY = 1;
uint8_t volatile positionX = 64;
uint8_t volatile positionY = 32;
int8_t aMeterDelta = 0;
const float accelScale = 0.05f;



volatile uint8_t tot_overflow0r3;
uint8_t    numberInterrupts0r3 = 2;
uint8_t    counts0r3 = 111;
uint16_t TimeOnCompare_HIGH= 70;
uint16_t NumberPulses_HIGH = 30;
uint16_t TimeOnCompare_MEDIUM= 70;
uint16_t NumberPulses_MEDIUM = 30;
uint16_t TimeOnCompare_LOW= 70;
uint16_t NumberPulses_LOW = 30;
volatile uint16_t TimeOnCompare= 70;
volatile uint16_t NumberPulses = 30;
volatile char HiOrLo = 0;
volatile int pccnt = 0;




void moveBall(uint8_t color){
	
	// direction => -1 move to left; 1 => move to right.
	fillcircle(buff,positionX,positionY,3,WHITE);
	if(!hasP1Scored() && !hasP2Scored()){	
		if (positionY > 60 || positionY < 11) {
			delY = delY * -1;
			OCR1A = 0x01FF;
			_delay_ms(100);
			OCR1A = 0x0000;
		}
	
		if (checkCollision(positionX, positionY, paddle1Y, paddle2Y, 3, 10)) {
			delX = delX * -1;
			write_buffer(buff);
		}
	
		positionX = positionX + delX;
		positionY = positionY + delY;
		fillcircle(buff,positionX,positionY,3,color);
		drawrect(buff,3,8,122,54, color);
	}else{
		OCR1A = 0x01FF;
		_delay_ms(200);
		OCR1A = 0x0000;
		handlePointDrop(color);		
	}
	
	//write_buffer(buff);
	
}

bool checkCollision(uint8_t ballX, uint8_t ballY, uint8_t pad1Y, uint8_t pad2Y, uint8_t rectWidth, uint8_t rectHeight) {
	bool result = false;

	if (ballX <= 10 && (ballY <= pad1Y + 10 && ballY >= pad1Y)) {
		
		result = true;
	}
	
	if (ballX >= 119 && (ballY <= pad2Y + 10 && ballY >= pad2Y)) {
		result = true;
	}

	return result;
}

bool hasP1Scored(){
	bool result = false;
		if (positionX > 122) {
			p1Score++;
			if(p1Score == 3){
				state = ENDSCREEN;
			}
			result = true;
		}
		return result;
}

bool hasP2Scored(){
	bool result = false;
	if(positionX < 6){
		p2Score++;
		//drawstring(buff,67,0,p2Score);
		if(p2Score == 3){
			state = ENDSCREEN;
		}
		result = true;
	}
	return result;
}


void waitForStart(){
	uint8_t yPos = 0;
	uint8_t xPos  = 0;
	do{
		adc_read();
		xPos = mapTocuhValues('x');
		yPos = mapTocuhValues('y');
	}while(!(xPos >32 && xPos <= 100 && yPos >= 54 && yPos <=66));
	printf("LCD Pos X: %u ---------- POs Y:%u\n", xPos,yPos);
	_delay_ms(500);
	state = MODESELECTION;
};

void waitForModeSelection(){
	int mode = 5;
	uint8_t yPos = 0;
	uint8_t xPos  = 0;
	touchX = 0;
	touchY = 0;
	_delay_ms(2000);
	do{
		adc_read();
		xPos = mapTocuhValues('x');
		yPos = mapTocuhValues('y');
		if(xPos > 5 && xPos <= 60 && yPos > 4 && yPos <= 15){
			mode = 1;
			gMode = SINGLEPLAYER;
			}else if(xPos > 80 && xPos <= 122 && yPos > 4 && yPos <= 15){
			mode = 2;
			gMode = MULTIPLAYER;
		}
		
	}while(mode == 5);
	printf("%u ------------%u\n", xPos, yPos);
	printf("%d ------------%d\n", mode, mode);
	
	state = SPEEDSELECTION;
};

void waitForSpeedSelection(){
	int speed = 5;
	uint8_t yPos = 0;
	uint8_t xPos  = 0;
	touchX = 0;
	touchY = 0;
	_delay_ms(2000);
	do{
		adc_read();
		xPos = mapTocuhValues('x');
		yPos = mapTocuhValues('y');
		
		if(xPos > 3 && xPos <= 40 && yPos > 4 && yPos <= 18){
			speed = 1;
			gSpeed = EASY;
			}else if(xPos > 42 && xPos <= 83 && yPos > 4 && yPos <= 18){
			speed = 2;
			gSpeed = MEDIUM;
			}else if(xPos > 83 && xPos <= 122 && yPos > 4 && yPos <= 18){
			speed = 3;
			gSpeed = HARD;
		}
		
	}while(speed == 5);
	printf("%u ------------%u\n", xPos, yPos);
	printf("%d ------------%d\n", speed, speed);
	startGame(BLACK);
	_delay_ms(500);
	state = PLAYGAME;
};

void checkGameStatus(){
	if(p1Score == 3 || p2Score == 3){
		//Game Over. P1 Won.
		state = ENDSCREEN;
	}
};

void controlPaddle(){
	uint8_t x =   mapTocuhValues('x');
	uint8_t y  = mapTocuhValues('y');
	
	if(x > 110 && gMode == MULTIPLAYER){
		paddle2Y = y;
		}else if(x < 20  && gMode == MULTIPLAYER){
		paddle1Y = y;
		}else if(gMode == SINGLEPLAYER){
		if(positionX > 64)
		paddle2Y = positionY;
	}
	if(gMode == SINGLEPLAYER){
		int16_t temp = ((int16_t)aMeter - 328);
		float rate = (float) temp *  accelScale;
		aMeterDelta = (int8_t) rate;
		paddle1Y = paddle1Y + aMeterDelta;
	}
}

void handlePointDrop(uint8_t color){
	positionX = 64;
	positionY = 32;
	PORTB = 0x05;
	_delay_ms(1000);
	//init_interrupts();
	clear_buffer(buff);
	PORTB = 0x04;
	startGame(color);
}

void init_interrupts(){
	//Wave generation with CTC Mode
	TCCR0A = 0b01000010; //CTC Mode
	TCCR0B = 0b10000100;
	TIMSK0 = 0b00000011; 
	TCNT0 = 0;
	OCR0A = TCNT0 + 4;
	PORTB |= (1 << 4);
	sei();				// Enable global interrupt

}

ISR(TIMER0_COMPA_vect){
	PORTB |= ~(1 << 4);
}