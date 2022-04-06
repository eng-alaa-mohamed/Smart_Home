/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     ADC                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 14-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/
#ifndef _ADC_CONFIG_H_
#define _ADC_CONFIG_H_

/*Set prescaler - These 3 bits determine the division factor between the XTAL frequency and the input clock to the ADC.
*values are 128,64,32,16,8,4,2*/
// CTRL Shift +  to zoom in code
#define ADC_PRESC				    128

//#define ADC_PRESC   DIV-BY-2
/*Reference voltage 5V    AVCC
 	 	 	 * 2.56V      Half_AVCC
 	 	 * External       External_AVCC */

#define Reference_Voltage		    AVCC


/*Resolution is either eight_bit
 	 	 	 	 	 * ten_bit */
#define Resolution				    10


/* Trigger   1- Enabled    1
             2- Disabled   0
if enabled, Trigger mode should be selected >> ADTS2 ADTS1 ADTS0 Trigger Source.
The trigger source is selected by setting the ADC Trigger Select bits, ADTS in SFIOR register   */
#define Auto_Trigger_Enable		    0       //Bit 5 – ADATE: ADC Auto Trigger Enable
/*
ADTS2 ADTS1 ADTS0 Trigger Source
Put the selection number (0 to 7)
0- Free_Running_mode
1- Analog_Comparator
2- External_Interrupt_Request_0
3- Timer_Counter0_Compare_Match
4- Timer_Counter0_Overflow
5- Timer_Counter_Compare_Match_B
6- Timer_Counter1_Overflow
7- Timer_Counter1_Capture_Event
*/
#define ADC_TRIG_SRC			    0

/* ADC Interrupt  1- Enabled   1
*				  2- Disabled  0
* the I-bit in SREG should be set too, so the ADC Conversion Complete Interrupt is activated.   */
#define ADC_INTERRUPT 			    0		//Bit 3 – ADIE: ADC Interrupt Enable


#define ADC_u32TIMEOUT             50000     //1 count roughly equals 2u sec

#endif

//for some reason  conditional directives #if doesn't accept #define macros in this way
//the workaround I found is to put numbers instead although it accepts enabled and disabled,
//anyone can explains that please also is there a right way to put 2 conditions statements #if
//or it has to be nested #if directive

