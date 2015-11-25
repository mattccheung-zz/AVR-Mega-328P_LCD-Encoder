/*
 * lcd.c
 *
 * Created: 8/2/2015 4:35:30 PM
 *  Author: Chiuyin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

/*============================== 4-bit LCD Functions ======================*/

/*
  Name:     lcd_init_4d
  Purpose:  initialize the LCD module for a 4-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/

void lcd_init_4d(void)
{
	// Power-up delay
	_delay_ms(100);                                 // initial 40 mSec delay

	// IMPORTANT - At this point the LCD module is in the 8-bit mode and it is expecting to receive
	//   8 bits of data, one bit on each of its 8 data lines, each time the 'E' line is pulsed.
	//
	// Since the LCD module is wired for the 4-bit mode, only the upper four data lines are connected to
	//   the microprocessor and the lower four data lines are typically left open.  Therefore, when
	//   the 'E' line is pulsed, the LCD controller will read whatever data has been set up on the upper
	//   four data lines and the lower four data lines will be high (due to internal pull-up circuitry).
	//
	// Fortunately the 'FunctionReset' instruction does not care about what is on the lower four bits so
	//   this instruction can be sent on just the four available data lines and it will be interpreted
	//   properly by the LCD controller.  The 'lcd_write_4' subroutine will accomplish this if the
	//   control lines have previously been configured properly.
	
	// configure the microprocessor pins for the data lines
	lcd_D7_ddr |= (1<<lcd_D7_bit);                  // 4 data lines - output
	lcd_D6_ddr |= (1<<lcd_D6_bit);
	lcd_D5_ddr |= (1<<lcd_D5_bit);
	lcd_D4_ddr |= (1<<lcd_D4_bit);

	// configure the microprocessor pins for the control lines
	lcd_E_ddr |= (1<<lcd_E_bit);                    // E line - output
	lcd_RS_ddr |= (1<<lcd_RS_bit);                  // RS line - output
	
	// configure the microprocessor pins for PWM line (for contrast adjustment)
	lcd_PWM_ddr |= (1<<lcd_PWM_bit);
	
	// configure the PWM settings for contrast adjustment (OCR0A values 0-255)
	lcd_PWM_timer_control_A = 0b10000011; // Phase Correct PWM 8 Bit, Clear OCA0/OCB0 on Compare Match, Set on TOP
	lcd_PWM_timer_control_B = 0b00000011; // Used 64 Prescaler
	lcd_PWM_timer_register = 0;           // Reset TCNT0
	lcd_PWM_output_compare = 90;           // Initial the Output Compare register A & B
	
	// Set up the RS and E lines for the 'lcd_write_4' subroutine.
	lcd_RS_port &= ~(1<<lcd_RS_bit);                // select the Instruction Register (RS low)
	lcd_E_port &= ~(1<<lcd_E_bit);                  // make sure E is initially low

	// Reset the LCD controller
	lcd_write_4(lcd_FunctionReset);                 // first part of reset sequence
	_delay_ms(10);                                  // 4.1 mS delay (min)

	lcd_write_4(lcd_FunctionReset);                 // second part of reset sequence
	_delay_us(200);                                 // 100uS delay (min)

	lcd_write_4(lcd_FunctionReset);                 // third part of reset sequence
	_delay_us(200);                                 // this delay is omitted in the data sheet

	// Preliminary Function Set instruction - used only to set the 4-bit mode.
	// The number of lines or the font cannot be set at this time since the controller is still in the
	//  8-bit mode, but the data transfer mode can be changed since this parameter is determined by one
	//  of the upper four bits of the instruction.
	
	lcd_write_4(lcd_FunctionSet4bit);               // set 4-bit mode
	_delay_us(80);                                  // 40uS delay (min)

	// Function Set instruction
	lcd_write_instruction_4d(lcd_FunctionSet4bit);   // set mode, lines, and font
	_delay_us(80);                                  // 40uS delay (min)

	// The next three instructions are specified in the data sheet as part of the initialization routine,
	//  so it is a good idea (but probably not necessary) to do them just as specified and then redo them
	//  later if the application requires a different configuration.

	// Display On/Off Control instruction
	lcd_write_instruction_4d(lcd_DisplayOff);        // turn display OFF
	_delay_us(80);                                  // 40uS delay (min)

	// Clear Display instruction
	lcd_write_instruction_4d(lcd_Clear);             // clear display RAM
	_delay_ms(4);                                   // 1.64 mS delay (min)

	// ; Entry Mode Set instruction
	lcd_write_instruction_4d(lcd_EntryMode);         // set desired shift characteristics
	_delay_us(80);                                  // 40uS delay (min)

	// This is the end of the LCD controller initialization as specified in the data sheet, but the display
	//  has been left in the OFF condition.  This is a good time to turn the display back ON.
	
	// Display On/Off Control instruction
	lcd_write_instruction_4d(lcd_DisplayOn);         // turn the display ON
	_delay_us(80);                                  // 40uS delay (min)
}

/*...........................................................................
  Name:     lcd_write_string_4d
; Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_write_string_4d(char theString[])
{
    volatile int i = 0;                             // character counter*/
    while (theString[i] != 0)
    {
        lcd_write_character_4d(theString[i]);
        i++;
        _delay_us(80);                              // 40 uS delay (min)
    }
}

/*...........................................................................
  Name:     lcd_write_character_4d
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/

void lcd_write_character_4d(uint8_t theData)
{
    lcd_RS_port |= (1<<lcd_RS_bit);                 // select the Data Register (RS high)
    lcd_E_port &= ~(1<<lcd_E_bit);                  // make sure E is initially low
    lcd_write_4(theData);                           // write the upper 4-bits of the data
    lcd_write_4(theData << 4);                      // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_instruction_4d
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/

void lcd_write_instruction_4d(uint8_t theInstruction)
{
    lcd_RS_port &= ~(1<<lcd_RS_bit);                // select the Instruction Register (RS low)
    lcd_E_port &= ~(1<<lcd_E_bit);                  // make sure E is initially low
    lcd_write_4(theInstruction);                    // write the upper 4-bits of the data
    lcd_write_4(theInstruction << 4);               // write the lower 4-bits of the data
	_delay_us(120);
}

/*...........................................................................
  Name:     lcd_write_4
  Purpose:  send a byte of information to the LCD module
  Entry:    (theByte) is the information to be sent to the desired LCD register
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    use either time delays or the busy flag
*/

void lcd_write_4(uint8_t theByte)
{
    lcd_D7_port &= ~(1<<lcd_D7_bit);                        // assume that data is '0'
    if (theByte & 1<<7) lcd_D7_port |= (1<<lcd_D7_bit);     // make data = '1' if necessary
	_delay_us(1); 
	
    lcd_D6_port &= ~(1<<lcd_D6_bit);                        // repeat for each data bit
    if (theByte & 1<<6) lcd_D6_port |= (1<<lcd_D6_bit);
	_delay_us(1);
	
    lcd_D5_port &= ~(1<<lcd_D5_bit);
    if (theByte & 1<<5) lcd_D5_port |= (1<<lcd_D5_bit);
	_delay_us(1);

    lcd_D4_port &= ~(1<<lcd_D4_bit);
    if (theByte & 1<<4) lcd_D4_port |= (1<<lcd_D4_bit);
	_delay_us(1);

// write the data
                                                    // 'Address set-up time' (40 nS)
    lcd_E_port |= (1<<lcd_E_bit);                   // Enable pin high
    _delay_us(1);                                   // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    lcd_E_port &= ~(1<<lcd_E_bit);                  // Enable pin low
    _delay_us(1);                                   // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}

/*...........................................................................
  Name:     lcd_puts
  Purpose:  put string onto LCD directly without 
  Entry:    (const char *s) is string in " " to be displayed
  Exit:     no parameters
  Notes:    uses time delays
*/

void lcd_puts(const char *s)
/* print string on lcd (no auto linefeed) */
{
	register char c;

	while ( (c = *s++) ) {
		lcd_write_character_4d(c);
		_delay_us(40);
	}

}

/*...........................................................................
  Name:     lcd_gotoxy
  Purpose:  place cursor at (x,y) on LCD 
  Entry:    x (column) y (row)
  Exit:     no parameters
  Notes:    uses time delays
*/

void lcd_gotoxy(uint8_t x, uint8_t y) {
	if (y==0) {
		lcd_write_instruction_4d(lcd_SetCursor|(lcd_LineOne+x));
	}
	else if (y==1) {
		lcd_write_instruction_4d(lcd_SetCursor|(lcd_LineTwo+x));
	}
	else if (y==2) {
		lcd_write_instruction_4d(lcd_SetCursor|(lcd_LineThree+x));
	}
	else if (y==3) {
		lcd_write_instruction_4d(lcd_SetCursor|(lcd_LineFour+x));
	}
	
}

/*...........................................................................
  Name:     lcd_clrsc
  Purpose:  Clear LCD screen 
  Entry:    None
  Exit:     no parameters
  Notes:    uses time delays
*/

void lcd_clrsc() {
	
	lcd_write_instruction_4d(lcd_Clear);
	
}

void lcd_clrln(uint8_t line) {
	
	lcd_gotoxy(0,line);
	lcd_puts("                    ");
	
}

void lcd_write_string_const(char const theString[]) {
	
	    volatile int i = 0;                             // character counter*/
	    while (theString[i] != 0)
	    {
	    lcd_write_character_4d(theString[i]);
	    i++;
	    _delay_us(80);                              // 40 uS delay (min)
	    }
	
}