/* ###################################################################
**     Filename    : main.c
**     Project     : Final_Project
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2022-11-15, 14:35, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "PIT_Init.h"
#if CPU_INIT_CONFIG
  #include "Init_Config.h"
#endif
/* User includes (#include below this line is not maintained by Processor Expert) */

/* display timings */
#define HSYNC_START 43
#define HSYNC_END 49
#define VSYNC_START 490
#define VSYNC_END 491

#define PADDLE_SIZE 7

/* state machine states */
enum States{INIT, COIN, GAME, END};

unsigned long long p = 0;
unsigned char vga_array[26250] = {0};

unsigned char coin = 0;

char alphabet[36][5] = {
		{ //0
		0b11100000,
		0b10100000,
		0b10100000,
		0b10100000,
		0b11100000
		},
		{ //1
		0b11000000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b11100000
		},
		{ //2
		0b11100000,
		0b00100000,
		0b11100000,
		0b10000000,
		0b11100000
		},
		{ //3
		0b11100000,
		0b00100000,
		0b11100000,
		0b00100000,
		0b11100000
		},
		{ //4
		0b10100000,
		0b10100000,
		0b11100000,
		0b00100000,
		0b00100000
		},
		{ //5
		0b11100000,
		0b10000000,
		0b11100000,
		0b00100000,
		0b11100000
		},
		{ //6
		0b11100000,
		0b10000000,
		0b11100000,
		0b10100000,
		0b11100000
		},
		{ //7
		0b11100000,
		0b00100000,
		0b00100000,
		0b01000000,
		0b01000000
		},
		{ //8
		0b11100000,
		0b10100000,
		0b11100000,
		0b10100000,
		0b11100000
		},
		{ //9
		0b11100000,
		0b10100000,
		0b11100000,
		0b00100000,
		0b11100000
		},
		{ //A
		0b01000000,
		0b10100000,
		0b11100000,
		0b10100000,
		0b10100000
		},
		{ //B
		0b11000000,
		0b10100000,
		0b11000000,
		0b10100000,
		0b11000000
		},
		{ //C
		0b01100000,
		0b10000000,
		0b10000000,
		0b10000000,
		0b01100000
		}, //D
		{
		0b11000000,
		0b10100000,
		0b10100000,
		0b10100000,
		0b11000000
		},
		{ //E
		0b11100000,
		0b10000000,
		0b11100000,
		0b10000000,
		0b11100000
		},
		{ //F
		0b11100000,
		0b10000000,
		0b11100000,
		0b10000000,
		0b10000000
		},
		{ //G
		0b11100000,
		0b10000000,
		0b10100000,
		0b10100000,
		0b11100000
		},
		{ //H
		0b10100000,
		0b10100000,
		0b11100000,
		0b10100000,
		0b10100000
		},
		{ //I
		0b11100000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b11100000
		},
		{ //J
		0b11100000,
		0b00100000,
		0b00100000,
		0b00100000,
		0b11000000
		},
		{ //K
		0b10000000,
		0b10000000,
		0b10100000,
		0b11000000,
		0b10100000
		},
		{ //L
		0b10000000,
		0b10000000,
		0b10000000,
		0b10000000,
		0b11100000
		},
		{ //M
		0b10100000,
		0b11100000,
		0b10100000,
		0b10100000,
		0b10100000
		},
		{ //N
		0b11000000,
		0b10100000,
		0b10100000,
		0b10100000,
		0b10100000
		},
		{ //O
		0b01000000,
		0b10100000,
		0b10100000,
		0b10100000,
		0b01000000
		},
		{ //P
		0b11100000,
		0b10100000,
		0b11000000,
		0b10000000,
		0b10000000
		},
		{ //Q
		0b11100000,
		0b10100000,
		0b01100000,
		0b00100000,
		0b00100000
		},
		{ //R
		0b11000000,
		0b10100000,
		0b11000000,
		0b10100000,
		0b10100000
		},
		{ //S
		0b01100000,
		0b10000000,
		0b11100000,
		0b00100000,
		0b11000000
		},
		{ //T
		0b11100000,
		0b01000000,
		0b01000000,
		0b01000000,
		0b01000000
		},
		{ //U
		0b10100000,
		0b10100000,
		0b10100000,
		0b10100000,
		0b11100000
		},
		{ //V
		0b10100000,
		0b10100000,
		0b10100000,
		0b10100000,
		0b01000000
		},
		{ //W
		0b10100000,
		0b10100000,
		0b11100000,
		0b11100000,
		0b10100000
		},
		{ //X
		0b10100000,
		0b10100000,
		0b01000000,
		0b10100000,
		0b10100000
		},
		{ //Y
		0b10100000,
		0b10100000,
		0b01000000,
		0b01000000,
		0b01000000
		},
		{ //Z
		0b11100000,
		0b00100000,
		0b01000000,
		0b10000000,
		0b11100000
		}
};

struct Ball {
	unsigned char x;
	unsigned char y;
	unsigned char x_prev;
	unsigned char y_prev;

	char v_x;
	char v_y;
} ball;

void init_VGA(unsigned char* array) {
	unsigned int i;
	for (i=0;i<26250;i++) {
		//horizontal sync
		if ((i % 50) >= HSYNC_START && (i % 50) <= HSYNC_END) {
			array[i] |= (1 << 2);
		}
		//vertical sync
		if (i >= (VSYNC_START*50) && i <= (VSYNC_END*50)) {
			array[i] |= (1 << 3);
		}
	}
}

void clear_screen(unsigned char* array) {
	for (unsigned int i=0;i<26250;i++) {
		array[i] &= (0b11 << 2);
	}
}

void toggle_pixel(unsigned char* array, unsigned char x, unsigned char y) {
	unsigned int i;
	for (i=0;i<16;i++) {
		array[x + (((y * 16) + i) * 50)] ^= (0b1011 << 4); //white pixel
	}
}

void toggle_paddle(unsigned char* array, unsigned char x, unsigned char y, unsigned char l) {
	unsigned char i;
	for (i=y;i<(y+l);i++) {
		toggle_pixel(array, x, i);
	}
}

/* accepts an 1x5 array where each bit is a horizontal pixel */
void toggle_char(unsigned char* array, char* chararray, const unsigned char x_pos, const unsigned char y_pos) {
	unsigned char i;
	unsigned char pixel;

	for (i=0;i<40;i++) {
		pixel = (chararray[(i/8)] >> (7-(i % 8))) & 0x01;
		if (pixel) {
			toggle_pixel(array, x_pos + (i%8), y_pos + (i/8));
		}
	}
}

void toggle_string(unsigned char* array, char* string, const unsigned char x_pos, const unsigned char y_pos) {
	unsigned int i = 0;
	while(string[i] != '\0') {
		if (string[i] >= 65) { //alpha
			toggle_char(array, alphabet[string[i] - 65 + 10], x_pos + (i*4), y_pos);
		}
		else { //numeric
			toggle_char(array, alphabet[string[i] - 48], x_pos + (i*4), y_pos);
		}
		i++;
	}
}

void PORTA_IRQHandler(void) {
	GPIOC_PDOR = (int)vga_array[p];

	p++;
	if (p==26250) {
		p = 0;
	}

	PORTA_ISFR = (1 << 1);
}

void PORTB_IRQHandler(void) {
	coin++;
	PORTB_ISFR = (1 << 9);
}


unsigned short ADC0_0_read16b(void) {
	ADC0_SC1A = 0b00000; //Write to SC1A to start conversion from ADC_0 (port number)
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK);    // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

unsigned short ADC0_1_read16b(void) {
	ADC0_SC1A = 0b00001; //Write to SC1A to start conversion from ADC_1 (port number)
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK);    // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

int main(void) {
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /*Enable Port A Clock Gate Control*/
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /*Enable Port B Clock Gate Control*/
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; /*Enable Port C Clock Gate Control*/
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; /*Enable Port D Clock Gate Control*/

	/* GPIO */
	PORTC_PCR2 = 0x100;
	PORTC_PCR3 = 0x100;
	PORTC_PCR4 = 0x100;
	PORTC_PCR5 = 0x100;
	PORTC_PCR7 = 0x100;

	PORTD_PCR0 = 0x100;
	PORTD_PCR1 = 0x100;
	PORTD_PCR2 = 0x100;

	GPIOC_PDDR |= (1 << 2);
	GPIOC_PDDR |= (1 << 3);
	GPIOC_PDDR |= (1 << 4);
	GPIOC_PDDR |= (1 << 5);
	GPIOC_PDDR |= (1 << 7);

	GPIOD_PDDR |= (1 << 0);
	GPIOD_PDDR |= (1 << 1);
	GPIOD_PDDR |= (1 << 2);

	/* port A interrupt */
	PORTA_PCR1 = 0xA0100; //configure for interrupt
	GPIOA_PDDR &= (0xFFFFFFFF ^ (1 << 1)); //set A1 to input
	GPIOA_PDDR |= (0 << 1);
	PORTA_ISFR = (1 << 1);

	/* port B interrupt */
	PORTB_PCR9 = 0xA0100; //configure for interrupt
	GPIOB_PDDR &= ~(1 << 9); //set A1 to input
	GPIOB_PDDR &= ~(1 << 9);
	PORTB_ISFR = (1 << 9);


	/* ADC Config */
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; // 0x8000000u; Enable ADC0 Clock
	ADC0_CFG1 = 0x0C; // 16bits ADC; Bus Clock
	ADC0_SC1A = 0x1F; // Disable the module, ADCH = 11111

	//fiducials
	/*
	toggle_pixel(vga_array,0,0);
	toggle_pixel(vga_array,39,0);
	toggle_pixel(vga_array,0,29);
	toggle_pixel(vga_array,39,29);
	*/

	unsigned char p1_pos_current = 0;
	unsigned char p1_pos = 5;
	unsigned char p1_score_current = 0;
	char p1_score[2] = {48,'\0'};

	unsigned char p2_pos_current = 0;
	unsigned char p2_pos = 5;
	unsigned char p2_score_current = 0;
	char p2_score[2] = {48,'\0'};

	struct Ball b;

	char state = INIT;
	for(;;) {
		/* State Machine */
		//actions
		switch(state) {
		case INIT:
			NVIC_DisableIRQ(PORTA_IRQn);
			NVIC_DisableIRQ(PORTB_IRQn);

			init_VGA(vga_array);

			p1_pos_current = 0;
			p1_pos = 5;
			p1_score_current = 0;
			p1_score[0] = 48;
			p1_score[0] = '\0';

			p2_pos_current = 0;
			p2_pos = 5;
			p2_score_current = 0;
			p2_score[0] = 48;
			p2_score[0] = '\0';

			b.v_x = 1;
			b.v_y = 0;
			b.x = 20;
			b.y = 14;
			b.x_prev = 0;
			b.y_prev = 0;

			coin = 0;

			NVIC_EnableIRQ(PORTA_IRQn);
			NVIC_EnableIRQ(PORTB_IRQn);

			state = COIN;
			break;
		case COIN:
			//await coin
			toggle_string(vga_array, "INSERT\0", 8, 5);
			toggle_string(vga_array, "COIN\0", 12, 11);
			for (unsigned int i=0;i<500000;i++);

			toggle_string(vga_array, "INSERT\0", 8, 5);
			toggle_string(vga_array, "COIN\0", 12, 11);
			for (unsigned int i=0;i<500000;i++);

			if (coin > 0) {
				//game init
				toggle_pixel(vga_array, 0, 0);
				toggle_paddle(vga_array, 2, p1_pos_current, PADDLE_SIZE);
				toggle_paddle(vga_array, 37, p2_pos_current, PADDLE_SIZE);
				toggle_string(vga_array,p1_score,5,2);
				toggle_string(vga_array,p2_score,32,2);
				state = GAME;
			}
			break;
		case GAME:
			p1_pos = ((ADC0_0_read16b() >> 11) & 0xFF);
			p2_pos = ((ADC0_1_read16b() >> 11) & 0xFF);

			if (p1_pos != p1_pos_current) {
				toggle_paddle(vga_array, 2, p1_pos_current, PADDLE_SIZE);
				toggle_paddle(vga_array, 2, p1_pos, PADDLE_SIZE);
				p1_pos_current = p1_pos;
			}
			if (p2_pos != p2_pos_current) {
				toggle_paddle(vga_array, 37, p2_pos_current, PADDLE_SIZE);
				toggle_paddle(vga_array, 37, p2_pos, PADDLE_SIZE);
				p2_pos_current = p2_pos;
			}

			if (p1_score[0] - 48 != p1_score_current) {
				toggle_string(vga_array,p1_score,5,2);
				p1_score[0] = (48 + p1_score_current);
				toggle_string(vga_array,p1_score,5,2);
			}
			if (p2_score[0] - 48 != p2_score_current) {
				toggle_string(vga_array,p2_score,32,2);
				p2_score[0] = (48 + p2_score_current);
				toggle_string(vga_array,p2_score,32,2);
			}

			/* ball code */
			toggle_pixel(vga_array, b.x_prev, b.y_prev);
			toggle_pixel(vga_array, b.x, b.y);
			b.x_prev = b.x;
			b.y_prev = b.y;
			b.x += b.v_x;
			b.y += b.v_y;

			if (b.y >= 29 || b.y == 0) { //y bounce
				b.v_y *= -1;
			}
			if (b.x == 0) { //P1 out of bounds
				b.v_x = 1;
				b.v_y = 0;
				b.x = 20;
				b.y = 14;
				p2_score_current++;
				if (p2_score_current > 9) {
					state = END;
				}
			}
			if (b.x >= 39) { //P2 out of bounds
				b.v_x = -1;
				b.v_y = 0;
				b.x = 20;
				b.y = 14;
				p1_score_current++;
				if (p1_score_current > 9) {
					state = END;
				}
			}

			if (b.x == 2 && b.y >= p1_pos) { //left paddle
				if (b.y < p1_pos + (PADDLE_SIZE/2)) { //top hit
					b.v_x *= -1;
					b.v_y = -1;
				}
				else if (b.y == p1_pos + (PADDLE_SIZE/2)) { //center hit
					b.v_x *= -1;
					b.v_y *= 0;
				}
				else if (b.y < p1_pos + (PADDLE_SIZE)) { //bottom hit
					b.v_x *= -1;
					b.v_y = 1;
				}
			}

			if (b.x == 37 && b.y >= p2_pos) { //right paddle
				if (b.y < p2_pos + (PADDLE_SIZE/2)) { //top hit
					b.v_x *= -1;
					b.v_y = -1;
				}
				else if (b.y == p2_pos + (PADDLE_SIZE/2)) { //center hit
					b.v_x *= -1;
					b.v_y *= 0;
				}
				else if (b.y < p2_pos + (PADDLE_SIZE)) { //bottom hit
					b.v_x *= -1;
					b.v_y = 1;
				}
			}

			unsigned int i;
			for(i=0;i<35000;i++); //software delay

			break;
		case END:
			clear_screen(vga_array);
			if (p1_score_current == 10) {
				toggle_string(vga_array, "PLAYER1\0", 8, 5);
			}
			else if (p2_score_current == 10) {
				toggle_string(vga_array, "PLAYER2\0", 8, 5);
			}
			toggle_string(vga_array, "WINS\0", 5, 11);
			for(unsigned int i=0;i<2000000;i++); //software delay
			clear_screen(vga_array);
			state = INIT;
			break;
		}

	} //infinite loop

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
