/*
 * lcd.h
 *
 * Created: 8/2/2015 4:35:37 PM
 *  Author: Chiuyin
 */ 


#ifndef LCD_H_
#define LCD_H_

// LCD interface (should agree with the diagram above)
//   make sure that the LCD RW pin is connected to GND
#define lcd_D7_port     PORTD                   // lcd D7 connection
#define lcd_D7_bit      PORTD7
#define lcd_D7_ddr      DDRD

#define lcd_D6_port     PORTD                   // lcd D6 connection
#define lcd_D6_bit      PORTD6
#define lcd_D6_ddr      DDRD

#define lcd_D5_port     PORTD                   // lcd D5 connection
#define lcd_D5_bit      PORTD5
#define lcd_D5_ddr      DDRD

#define lcd_D4_port     PORTD                   // lcd D4 connection
#define lcd_D4_bit      PORTD4
#define lcd_D4_ddr      DDRD

#define lcd_E_port      PORTB                   // lcd Enable pin
#define lcd_E_bit       PORTB2
#define lcd_E_ddr       DDRB

#define lcd_RS_port     PORTB                   // lcd Register Select pin
#define lcd_RS_bit      PORTB0
#define lcd_RS_ddr      DDRB

#define lcd_PWM_port	PORTB					// lcd PWM pin
#define lcd_PWM_bit		PORTB1
#define lcd_PWM_ddr		DDRB

#define lcd_PWM_timer_control_A TCCR1A
#define lcd_PWM_timer_control_B TCCR1B
#define lcd_PWM_timer_register TCNT1
#define lcd_PWM_output_compare OCR1A


// LCD module information
#define		lcd_LineOne     0x00                    // start of line 1
#define		lcd_LineTwo     0x40                    // start of line 2
#define		lcd_LineThree   0x14                  // start of line 3 (20x4)
#define		lcd_LineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#ifndef lcd_Clear
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#endif
#ifndef lcd_Home
#define lcd_Home            0b00000010          // return cursor to first position on first line
#endif
#ifndef lcd_EntryMode
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#endif
#ifndef lcd_DisplayOff
#define lcd_DisplayOff      0b00001000          // turn display off
#endif
#ifndef lcd_DisplayOn
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#endif
#ifndef lcd_FunctionReset
#define lcd_FunctionReset   0b00110000          // reset the LCD
#endif
#ifndef lcd_FunctionSet4bit
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#endif
#ifndef lcd_SetCursor
#define lcd_SetCursor       0b10000000          // set cursor position
#endif
// Function Prototypes
extern void lcd_write_4(uint8_t theByte);
extern void lcd_write_instruction_4d(uint8_t theInstruction);
extern void lcd_write_character_4d(uint8_t theData);
extern void lcd_write_string_4d(char theString[]);
extern void lcd_write_string_const(char const theString[]);
extern void lcd_init_4d(void);
extern void lcd_puts(const char *s);
extern void lcd_gotoxy(uint8_t x, uint8_t y);
extern void lcd_clrsc();
extern void lcd_clrln(uint8_t line);

#endif /* LCD_H_ */