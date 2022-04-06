/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     TIMER                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 23-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/

#ifndef TIMER_REGISTER_H_
#define TIMER_REGISTER_H_

#define TCCR0			*((volatile u8*)0x53)    	// Timer/Counter Control Register for Timer0 (8-bit) timer
#define TCCR0_WGM00		6   //Waveform generation mode bit0
#define TCCR0_WGM01		3   //Waveform generation mode bit1
#define TCCR0_CS02		2   //Prescaler bit2
#define TCCR0_CS01		1   //Prescaler bit1
#define TCCR0_CS00		0   //Prescaler bit0
#define TCCR0_COM00     4   //Compare Output Mode, Fast PWM Mode(1)
#define TCCR0_COM01     5   //Compare Output Mode, Fast PWM Mode(1)



#define OCR0			  *((volatile u8*)0x5C)   //CTC (Clear Time on compare match)-output compare register 0


#define TCNT0		      *((volatile u8*)0x52)   //Overflow mode

#define TIMSK 		      *((volatile u8*)0x59)   //Timer/Counter Interrupt Mask Register – TIMSK
#define TIMSK_OCIE0       1                       //output compare 0 interrupt enable
#define TIMSK_TOIE0       0                       //TOIE0: Timer/Counter0 Overflow Interrupt Enable

#define TIFR 		      *((volatile u8*)0x58)   //


// Timer1 Register
#define TCCR1A 		      *((volatile u8*)0x4F)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define TCCR1B 		      *((volatile u8*)0x4E)   // Timer/Counter Control Register for Timer1 (16-bit) timer

#define TCCR1A_WGM10      0
#define TCCR1A_WGM11      1
#define TCCR1B_WGM12      3
#define TCCR1B_WGM13      4

#define TCCR1A_COM1A0     6
#define TCCR1A_COM1A1     7
#define TCCR1A_COM1B0     4
#define TCCR1A_COM1B1     5

#define TCCR1B_CS10       0
#define TCCR1B_CS11       1
#define TCCR1B_CS12       2


#define TCNT1H 		    *((volatile u8*)0x4D)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define TCNT1L 		    *((volatile u8*)0x4C)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define TCNT1 		    *((volatile u16*)0x4C)   // Timer/Counter Control Register for Timer1 (16-bit) timer

#define OCR1AH 		    *((volatile u8*)0x4B)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define OCR1AL 		    *((volatile u8*)0x4A)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define OCR1A 		    *((volatile u16*)0x4A)   // Timer/Counter Control Register for Timer1 (16-bit) timer

#define OCR1BH 		    *((volatile u8*)0x49)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define OCR1BL 		    *((volatile u8*)0x48)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define OCR1B 		    *((volatile u16*)0x48)   // Timer/Counter Control Register for Timer1 (16-bit) timer

#define ICR1H 		    *((volatile u8*)0x47)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define ICR1L 		    *((volatile u8*)0x46)   // Timer/Counter Control Register for Timer1 (16-bit) timer
#define ICR1 		    *((volatile u16*)0x46)   //


#endif
