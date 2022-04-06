/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     ADC                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 14-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/
#ifndef _ADC_PRIVATE_H_
#define _ADC_PRIVATE_H_

#define ADC_PRESC_MASK 			0b11111000
#define MUX_Clear	   			0b11100000

#define TRIG_SOURCE_MASK 	    0b00011111
//0b00011111
//bit masking method: in prog.c  ADCSRA |=ADC_PRESC;   Oring of the whole ADCSRA register with prescaler 111
// but we have to make sure 3bits of this register were initially = 0   ADCSRA &= 0b11111000

#define AVCC		 				5

#define IDLE		 				0
#define BUSY		 				1

#define SINGLE_CHANNEL_ASYNCH    	0
#define CHAIN_ASYNCH    			1

#endif
