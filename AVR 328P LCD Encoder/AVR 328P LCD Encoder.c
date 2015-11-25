/*
 * AVR_328P_LCD_Encoder.c
 *
 * Created: 8/4/2015 7:37:05 AM
 *  Author: Chiuyin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

#include "lcd.h"
#include "lcd_menu.h"

//volatile int pulses = 0;

int main(void)
{
	lcd_init_4d();
	show_menu();
	//lcd_gotoxy(0,0);
	//lcd_puts("Microlambda Pte Ltd");
	
	DDRD &= ~(1<<PORTD2);
	DDRB &= ~(1<<PORTB3);
	PORTD |= (1<< PORTD2);
	PORTB |= (1<<PORTB3);
	
	EICRA |= (1 << ISC01)|(1 << ISC00);  // set INT0 to trigger on change
	EIMSK |= (1 << INT0);     // Turns on INT0
	
	
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT4);
	
	DDRB &= ~(1<<PINB4);
	PORTB |= (1<<PORTB4);
	
	sei();
	
    while(1)
    {
        _delay_ms(1);//TODO:: Please write your application code 
    }
}

ISR (INT0_vect) {
	//char *pulse_display = (char*)calloc(5, sizeof(char));
	//lcd_gotoxy(0,2);
	//lcd_puts("Start");
	if((PINB>>PINB3)&1) //check if pin state of PD2 and PD4 are equal
	{

		browse_menu(1);
		//pulses--;
		//itoa(pulses,pulse_display,10);
		//lcd_clrln(1);
		//lcd_gotoxy(0,1);
		//lcd_write_string_4d(pulse_display);
			
	}
	else 
	{

		browse_menu(2);
		//pulses++;
		//itoa(pulses,pulse_display,10); 
		//lcd_clrln(1);
		//lcd_gotoxy(0,1);
		//lcd_write_string_4d(pulse_display);
	}
	//free(pulse_display);
	
}

ISR (PCINT0_vect) {
	
	//_delay_ms(5);
	if ((PINB>>PINB4)&1) {

		browse_menu(3);
		
	}
	
}



