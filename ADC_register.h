/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     ADC                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 14-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/

#ifndef ADC_REGISTER_H_
#define ADC_REGISTER_H_

#define ADMUX   	*((volatile u8*)0x27)   	//ADC Multiplexer selection register
#define ADMUX_REFS1		7                   	//Reference Selection Register bit 1
#define ADMUX_REFS0		6                   	//Reference Selection Register bit 0
#define ADMUX_ADLAR		5						// ADC Left adjust result
#define ADMUX_MUX4		4
#define ADMUX_MUX3		3
#define ADMUX_MUX2		2
#define ADMUX_MUX1		1
#define ADMUX_MUX0		0


#define ADCSRA  	*((volatile u8*)0x26)      	//ADC control and status register A
#define ADCSRA_ADEN		7   				    //ADC enable
#define ADCSRA_ADSC		6						//start conversion
#define ADCSRA_ADATE	5						//auto trigger enable
#define ADCSRA_ADIF		4  						//Interrupt Flag
#define ADCSRA_ADIE		3						//Interrupt Enable
#define ADCSRA_ADPS2	2						//Prescaler Bit2
#define ADCSRA_ADPS1	1						//Prescaler Bit1
#define ADCSRA_ADPS0	0						//Prescaler Bit0


#define ADCH   		*((volatile u8*)0x25)    	//ADC High register
#define ADCL  		*((volatile u8*)0x24)    	//ADC Low Register
#define ADC			*((volatile u16*)0x24)    	//pointer to u16 reads ADCL so it will read 0x24 & 0x25



#define SFIOR       *((volatile u8*)0x50)       //Special FunctionIO Register SFIOR
#define ADTS2          7
#define ADTS1          6
#define ADTS0          5


#endif
