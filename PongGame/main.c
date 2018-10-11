#define F_CPU 16000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>


#include "lcd.h"
#include "adc.h"
#include "UART.h"
#include "gameControls.h"

#define FREQ 16000000
#define BAUD 9600
#define HIGH 1
#define LOW 0
#define BUFFER 1024
#define BLACK 0x000001
#define  WHITE 0x000000
#define PADDLE2 2
#define PADDLE1 1

char displayChar = 0;
double const gameSpeed = 5;



enum gameState state = HOMESCREEN;
enum gameSpeed gSpeed = EASY;
enum gameMode gMode = SINGLEPLAYER;



int main(void)
{
	//setting up the gpio for backlight
	DDRD |= 0x80;		//10000000
	PORTD &= ~0x80;		//10000000
	PORTD = 0x00;		// This is for Blue.
	DDRB |= 0x05;	//00001101
	PORTB &= ~0x05; //00000101
	PORTB = 0x04;	//	1 => Purpleish 4 => Green 5 => Red
	
	DDRB |= (1 << DDB1);
	PORTB |=  (1 << DDB1);
	OCR1A = 0x0000;

	
	TCCR1A |= (1 << COM1A1);
	// set non-inverting mode
	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	// set 10bit phase corrected PWM Mode
	TCCR1B |= (1 << CS11);
	//lcd initialisation
	lcd_init();
	uart_init();
	
	lcd_command(CMD_DISPLAY_ON);
	lcd_set_brightness(0x18);
	write_buffer(buff);
	adcInit();
	_delay_ms(3000);
	clear_buffer(buff);
	write_buffer(buff);
	
	int horizontalDirection = touchX % 2;
	if(!horizontalDirection){
		delX = delX * -1;
		delY = delY * -1;
	}
	
	while (1)
	{
		if(state == HOMESCREEN){
			PORTB = 0x04;		
			drawHomePage(BLACK);
			write_buffer(buff);
			waitForStart();
		}else if(state == MODESELECTION){
			drawModeSelection(BLACK);
			write_buffer(buff);
			waitForModeSelection();
			
		}else if(state == SPEEDSELECTION){	
			drawSpeedSelection(BLACK);
			write_buffer(buff);
			waitForSpeedSelection();
		}
		else if(state == PLAYGAME){	
			drawPaddle(PADDLE1, WHITE);
			drawPaddle(PADDLE2, WHITE);
			controlPaddle();	
			drawPaddle(PADDLE1, BLACK);
			drawPaddle(PADDLE2, BLACK);
			moveBall(BLACK);
			write_buffer(buff);
		}else if(state == PAUSEGAME){
			// Wait for pause to release.
			drawPauseScreen(BLACK);
			state = PLAYGAME;
			//drawPauseScreen(BLACK);
		}else if(state == ENDSCREEN){
			drawEndScreen(BLACK);
			state = HOMESCREEN;
		}
		adc_read();
		if(gSpeed == EASY){
			_delay_ms(100);
		}else if(gSpeed == HARD){
			_delay_ms(5);
		}else{
			_delay_ms(50);
		}
	}
}

