/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     TIMER                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 23-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/

#ifndef _TIMER_INTERFACE_H_
#define _TIMER_INTERFACE_H_

void TIMER0_voidInit(void);

u8 TIMER0_u8SetCallBack(void (*Copy_pvCallBackFunc) (void));

void TIMER0_voidSetCompareMatchValue(u8 Copy_u8OCR0Value);

void TIMER1_voidInit(void);

void TIMER1_voidSetCompareMatchValue(u16 Copy_u8OCR1AValue);

#endif
