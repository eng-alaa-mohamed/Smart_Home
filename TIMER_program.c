/*****************************************************************/
 /****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     TIMER                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 23-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "TIMER_interface.h"
#include "TIMER_register.h"
#include "TIMER_private.h"
#include "TIMER_config.h"

static void (*TIMER0_pvCallBackFunc)(void) = NULL;
void TIMER0_voidInit(void)
{

#if Timer0_Mode == CTC
	/*Choose CTC mode */
	CLR_BIT(TCCR0,TCCR0_WGM00);
	SET_BIT(TCCR0,TCCR0_WGM01);
	/*Output compare match interrupt enable */
	SET_BIT(TIMSK,TIMSK_OCIE0);

	/*Set compare match value to 250  250 is an example which is less than 256 > 2^8  (8bit timer) */
	OCR0 = 125;

	/*Timer Prescaler which acts as timer enable - any enable should be the last command in any initialization */
	/*Prescaler division by 64*/
	SET_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);

#elif Timer0_Mode == Normal_Mode
	/*Choose CTC mode */
	CLR_BIT(TCCR0,TCCR0_WGM00);
	CLR_BIT(TCCR0,TCCR0_WGM01);

	/*Timer Prescaler - enable Overflow Interrupt */
	SET_BIT(TIMSK,TIMSK_TOIE0);

	//Freq = 8mhZ    Prescaler = 8  Tick Time = 1uS  time to overflow = 256*1us = 256us  required time =1s
	//no. of overflows = 1,000,000us/256us= 3906.25 overflow   (counter of overflow = 3907)
	//0.25 overflow = 0.25 * 256 = 64	Preload= 256-64 = 192

	TCNT0 = 192;  // set preload value i.e start count from 192 to 256
	//set prescaler value CLK=8//
	CLR_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);

#elif Timer0_Mode == Counter    //Falling edge
	CLR_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	SET_BIT(TCCR0,TCCR0_CS02);

#elif Timer0_Mode == Fast_PWM
	/*Choose Fast_PWM mode */
	SET_BIT(TCCR0,TCCR0_WGM00);
	SET_BIT(TCCR0,TCCR0_WGM01);
	//set prescaler value CLK=8//
	CLR_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);
	if (PWM_Mode == Non_Inverting)
	{CLR_BIT(TCCR0,TCCR0_COM00);
	SET_BIT(TCCR0,TCCR0_COM01);}
	else if (PWM_Mode == Inverting)
	{SET_BIT(TCCR0,TCCR0_COM00);
	SET_BIT(TCCR0,TCCR0_COM01);}

#elif Timer0_Mode == Phase_Correct_PWM
	/*Choose CTC mode */
	SET_BIT(TCCR0,TCCR0_WGM00);
	CLR_BIT(TCCR0,TCCR0_WGM01);
	//set prescaler value CLK=8//
	CLR_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);
	if (PWM_Mode == Non_Inverting)
	{CLR_BIT(TCCR0,TCCR0_COM00);
	SET_BIT(TCCR0,TCCR0_COM01);}
	else if (PWM_Mode == Inverting)
	{SET_BIT(TCCR0,TCCR0_COM00);
	SET_BIT(TCCR0,TCCR0_COM01);}

#endif
}

/*function writes number in OCR0 the compare match value used for PWM*/
void TIMER0_voidSetCompareMatchValue(u8 Copy_u8OCR0Value)
{
	OCR0 = Copy_u8OCR0Value;
}


u8 TIMER0_u8SetCallBack(void (*Copy_pvCallBackFunc)(void))
{
	u8 Local_u8ErrorStatus = OK;
	if(Copy_pvCallBackFunc!=NULL)
		TIMER0_pvCallBackFunc= Copy_pvCallBackFunc;
	else
		Local_u8ErrorStatus=NULL_POINTER;

	return Local_u8ErrorStatus;
}

//ISR Timer/Counter0 compare match
void __vector_10 (void) __attribute__((signal));
void __vector_10 (void)
{
	if(TIMER0_pvCallBackFunc!= NULL)
	{
		TIMER0_pvCallBackFunc();
	}
}

//ISR Timer/Counter0 overflow
void __vector_11 (void) __attribute__((signal));
void __vector_11 (void)
{
	if(TIMER0_pvCallBackFunc!= NULL)
	{
		TIMER0_pvCallBackFunc();   //call timer0 function
	}
}

void TIMER1_voidInit(void)
{

	/*select Timer1 mode >> mode: 14 - in this mode TOP value is configurable i.e it is not necessary to be 2^8 */
	CLR_BIT(TCCR1A,TCCR1A_WGM10);
	SET_BIT(TCCR1A,TCCR1A_WGM11);
	SET_BIT(TCCR1B,TCCR1B_WGM12);
	SET_BIT(TCCR1B,TCCR1B_WGM13);


	/* Select hardware action on OC1A PIN => Non-Inverting */
	CLR_BIT(TCCR1A,TCCR1A_COM1A0);
	SET_BIT(TCCR1A,TCCR1A_COM1A1);
	/* Select hardware action on OC1B PIN => Non-Inverting */
	CLR_BIT(TCCR1A,TCCR1A_COM1B0);
	SET_BIT(TCCR1A,TCCR1A_COM1B1);

	/* Set Top Value in ICR1 */
	//ICR1 = 19999;
	/* Set Angle of Servo Motor */
	//OCR1A = 999;   //Angle = 0    OCR1A = 1999 angle 180
	//set prescaler value CLK=8//
	CLR_BIT(TCCR1B,TCCR1B_CS10);
	SET_BIT(TCCR1B,TCCR1B_CS11);
	CLR_BIT(TCCR1B,TCCR1B_CS12);
}

void TIMER1_voidSetCompareMatchValue(u16 Copy_u8OCR1AValue)
{
	OCR1A = Copy_u8OCR1AValue;
}

/*Assignment-1: code that count number of switch hits on switch connected to T0 (PB0)
 * add counter in Timer driver - define PB0 as input, enable pull up resistor
 * in main function initialize LCD with value 0 then for each interrupt increment the counter
 */ //void LCD_vidWriteNumber(u16 Counter)


//Assignment: Piano each switch on keypad has a different tone on buzzer using PWM
//Read datasheet - chapter (Timer1)


//and display this count on LCD external clock on this pin, activate pull up resistor on PB0
//Assignment-2 stop watch


