// ***********************************************************************
// LCD menu
// ***********************************************************************



//************************************************************************
// MPLAB/HITECH-C includes
//************************************************************************
//#include <htc.h>
#define F_CPU 16000000UL
#include <stdlib.h>

//************************************************************************
// Projekt includes						// Include your own functions here
//************************************************************************

#include <util/delay.h>					// Delayfunctions
#include "lcd.h"					// Menu
#include "lcd_menu.h"

// ***********************************************************************
// Variable definitions
// ***********************************************************************
// General
static unsigned char selected = 1;			// Start with first entry (apart from header)

// Menu strings
const char menu_000[] = "  Left :  50   100  ";  	// 0
const char menu_001[] = "  Right:  50   100  ";  	// 1
const char menu_002[] = "  Save";  			// 2
const char menu_003[] = "  Load";  			// 3

const char menu_100[] = " Save Profile";  	// 4
const char menu_101[] = "  Profile 1";  	// 5
const char menu_102[] = "  Profile 2";  	// 6
const char menu_103[] = "  Profile 3";  	// 7
const char menu_104[] = "  Profile 4";  	// 8
const char menu_105[] = "  Profile 5";  	// 9
const char menu_106[] = "  Profile 6";  	// 10
const char menu_107[] = "  Profile 7";  	// 11
const char menu_108[] = "  Profile 8";  	// 12
const char menu_109[] = "  Profile 9";  	// 13
const char menu_110[] = "  Exit";  			// 14

const char menu_200[] = " Load Profile";  	// 15
const char menu_201[] = "  Profile 1";  	// 16
const char menu_202[] = "  Profile 2";  	// 17	
const char menu_203[] = "  Profile 2";  	// 18
const char menu_204[] = "  Profile 4";  	// 19
const char menu_205[] = "  Profile 5";  	// 20
const char menu_206[] = "  Profile 6";  	// 21
const char menu_207[] = "  Profile 7";  	// 22
const char menu_208[] = "  Profile 8";  	// 23
const char menu_209[] = "  Profile 9";  	// 24
const char menu_210[] = "  Exit";  			// 25


// Array of entries
MenuEntry menu[] =
{
//	TEXT		NUM		UP		DOWN	ENTER	FUNC
	{menu_000,	4,		3,		1,		0,		0}, 		// 0
	{menu_001,	4,		0,		2,		1,		0},
	{menu_002,	4,		1,		3,		4,		0},
	{menu_003,	4,		2,		0,		15,		0},			// 3
		
	{menu_100,  11,		14,		5,		4,		0},			// 4
	{menu_101,  11,		4,		6,		4,		0},
	{menu_102,  11,		5,		7,		6,		0},
	{menu_103,  11,		6,		8,		7,		0},
	{menu_104,  11,		7,		9,		8,		0},
	{menu_105,  11,		8,		10,		9,		0},
	{menu_106,  11,		9,		11,		10,		0},
	{menu_107,  11,		10,		12,		11,		0},
	{menu_108,  11,		11,		13,		12,		0},
	{menu_109,  11,		12,		14,		13,		0},
	{menu_110,  11,		13,		5,		0,		0},			// 14

	{menu_200,  11,		25,		16,		15,		0},			// 15
	{menu_201,  11,		15,		17,		16,		0},
	{menu_202,  11,		16,		18,		17,		0},
	{menu_203,  11,		17,		19,		18,		0},
	{menu_204,  11,		18,		20,		19,		0},
	{menu_205,  11,		19,		21,		20,		0},
	{menu_206,  11,		20,		22,		21,		0},
	{menu_207,  11,		21,		23,		22,		0},
	{menu_208,  11,		22,		24,		23,		0},
	{menu_209,  11,		23,		25,		24,		0},
	{menu_210,  11,		24,		15,		0,		0},			// 25

};


// ***********************************************************************
// Show menu function
// ***********************************************************************
void show_menu(void)
{	unsigned char line_cnt;					// Count up lines on LCD
 	unsigned char from;
 	unsigned char till = 0;

	unsigned char temp;						// Save "from" temporarily for always visible header and cursor position
 
	 // Get beginning and end of current (sub)menu
 	while (till <= selected)
 	{	till += menu[till].num_menupoints;
 	}
	from = till - menu[selected].num_menupoints;
 	till--;

	temp = from;							// Save from for later use

	lcd_clrsc();
	_delay_ms(40);
	//--------------------------------------------------------------------
	// Always visible Header
	//--------------------------------------------------------------------
#ifdef USE_ALWAYS_VISIBLE_HEADER

	line_cnt = 1;							// Set line counter to one since first line is reserved for header

	// Write header
	lcd_gotoxy(0,0);
	lcd_write_string_4d(menu[temp].text);

	// Output for two row display becomes fairly easy
	#ifdef USE_TWO_ROW_DISPLAY

		lcd_gotoxy(0,UPPER_SPACE);
		lcd_write_string_4d(menu[selected].text);
		lcd_gotoxy(0,UPPER_SPACE);
		lcd_write_character_4d(SELECTION_CHAR);

	#endif	// USE_TWO_ROW_DISPLAY


	// Output for four row display
	#ifdef USE_FOUR_ROW_DISPLAY	||	#ifdef		USE_THREE_ROW_DISPLAY

		// Output formatting for selection somewhere in between (sub)menu top and bottom
		if ( (selected >= (from + UPPER_SPACE)) && (selected <= (till - LOWER_SPACE)) )
    	{	from = selected - (UPPER_SPACE - 1);
			till = from + (DISPLAY_ROWS - 2);

	 		for (from; from<=till; from++)
	 		{	
				lcd_gotoxy(0,line_cnt);
				lcd_write_string_4d(menu[from].text);
	 	 	 	line_cnt++;
			}
	
			lcd_gotoxy(0, UPPER_SPACE);
			lcd_write_character_4d(SELECTION_CHAR);
		}

		// Output formatting for selection close to (sub)menu top and bottom 
		// (distance between selection and top/bottom defined as UPPER- and LOWER_SPACE)
		else
		{	// Top of (sub)menu
			if (selected < (from + UPPER_SPACE))
			{	from = selected;
				till = from + (DISPLAY_ROWS - 2);
	 			
				for (from; from<=till; from++)
	 			{	
					lcd_gotoxy(0,line_cnt);
					lcd_write_string_4d(menu[from].text);
	 	 	 		line_cnt++;
				}
	
				lcd_gotoxy(0, (UPPER_SPACE-1));
				lcd_write_character_4d(SELECTION_CHAR);
			}
		
			// Bottom of (sub)menu
			if (selected == till)
			{	from = till - (DISPLAY_ROWS - 2);
	
				for (from; from<=till; from++)
	 			{	
					lcd_gotoxy(0,line_cnt);
					lcd_write_string_4d(menu[from].text);
	 	 	 		line_cnt++;
				}
	
				lcd_gotoxy(0, (DISPLAY_ROWS - 1));
				lcd_write_character_4d(SELECTION_CHAR);
			}
		}

	#endif	// USE_FOUR_ROW_DISPLAY


	//--------------------------------------------------------------------
	// Header not always visible
	//--------------------------------------------------------------------
#else	// !defined USE_ALWAYS_VISIBLE_HEADER

	line_cnt = 0;							// Set line counter to zero since all rows will be written

	// Output formatting for selection somewhere in between (sub)menu top and bottom
	if ( (selected >= (from + UPPER_SPACE)) && (selected <= (till - LOWER_SPACE)) )
    {	from = selected - UPPER_SPACE;
		till = from + (DISPLAY_ROWS - 1);

 		for (; from<=till; from++)
 		{	
			lcd_gotoxy(0,line_cnt);
			lcd_write_string_const(menu[from].text);
 	 	 	line_cnt++;
		}
	
		lcd_gotoxy(0, UPPER_SPACE);
		lcd_write_character_4d(SELECTION_CHAR);
	}

	// Output formatting for selection close to (sub)menu top and bottom 
	// (distance between selection and top/bottom defined as UPPER- and LOWER_SPACE)
	else
	{	// Top of (sub)menu
		if (selected < (from + UPPER_SPACE))
		{	till = from + (DISPLAY_ROWS - 1);
 			
			for (; from<=till; from++)
 			{	
				lcd_gotoxy(0,line_cnt);
				//lcd_clrln(line_cnt);
				lcd_write_string_const(menu[from].text);
 	 	 		line_cnt++;
			}

			lcd_gotoxy(0, (selected-temp));
			lcd_write_character_4d(SELECTION_CHAR);
		}
	
		// Bottom of (sub)menu
		if (selected == till)
		{	from = till - (DISPLAY_ROWS - 1);

			for (; from<=till; from++)
 			{	
				lcd_gotoxy(0,line_cnt);
				lcd_write_string_const(menu[from].text);
 	 	 		line_cnt++;
			}

			lcd_gotoxy(0, (DISPLAY_ROWS - 1));
			lcd_write_character_4d(SELECTION_CHAR);
		}
	}

#endif	// !defined USE_ALWAYS_VISIBLE_HEADER

}

// ***********************************************************************
// Browse menu function
// ***********************************************************************
void browse_menu(uint8_t action)
{
	
		//show_menu();
	 	if (action == 1) { //"UP" COMMAND
		 
			selected = menu[selected].up;
		 
		 }
		 
		else if (action == 2) { //"DOWN" COMMAND
			
			selected = menu[selected].down;
			
		}

	 	else if (action == 3) //"ENTER" COMMAND
	 	{
	   		if (menu[selected].fp != 0)
	  		menu[selected].fp();

	  		selected = menu[selected].enter;
	 	}
		show_menu();
	 	_delay_ms(50);

}



// ***********************************************************************
// Add user functions & variables
// ***********************************************************************
